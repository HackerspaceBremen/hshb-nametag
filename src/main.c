#define MEDIUM_PRESS_TIME 1000
#define LONG_PRESS_TIME 2000

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "animations.h"
#include "calibration_data.h"
#include "charlieplex.h"
#include "cmd.h"
#include "display.h"
#include "logo.h"
#include "slots.h"
#include "uart.h"
#define BAUD 300

// Calculate the UBRR value for the desired baud rate
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1)

void checkButton();
void deepSleep();

uint8_t button = 1;
uint8_t lastButton = 1;
uint8_t newButtonEvent = 0;
uint32_t buttonPressMillis;

uint8_t volatile vRAM[522];
volatile uint32_t millis = 0;
volatile uint8_t adcTimerCounter = 0;
volatile uint8_t scrollTimerCounter = 0;
volatile uint16_t ADCvalue = 0;
volatile uint8_t displayOn = 1;

volatile uint16_t voltage;
volatile int8_t percent;
uint8_t charging = 0;
uint8_t chargeOn = 0;
uint32_t lastCharge = 0;

struct Slot currentSlot = {
    .slot_no = SLOT_MAX,
    .slot_loaded_millis = 0,
    .scroll_counter = 0,
    .last_animation = 0,
};

// Set to 1 to advance slot after finishing current drawing
uint8_t advanceSlot = 0;

uint8_t scrollInterval = 0;
#define BATT_BUFFER_SIZE 15
char battBuffer[BATT_BUFFER_SIZE];

uint8_t lowBat = 0;

void advance_slot() {
  scrollStop();
  uint8_t attempts = 0;
  while (1) {
    currentSlot.slot_no++;
    if (currentSlot.slot_no >= SLOT_MAX) {
      currentSlot.slot_no = 0;
    }
    load_slot((struct Slot*)&currentSlot);
    if (currentSlot.enabled) {
      break;
    }

    attempts++;
    if (attempts >= SLOT_MAX) {
      // No active slots found.
      break;
    }
  }
}

void handle_slot() {
  // Draws one frame of the current slot, sets advanceSlot when it is time to
  // advance to the next slot
  if (currentSlot.enabled) {
    switch (currentSlot.animation) {
      case LOGO_ON:
        for (uint16_t i = LOGO_START; i < LOGO_END; i++) vRAM[i] = 32;
        break;
      case MATRIX:
        matrixAnimation();
        break;
      case SWEEP:
        sweepAnimation(32, 1);
        break;
      case WAVE:
        waveAnimation();
        break;
      case LINES:
        linesAnimation(0);
        break;
      case LINES_FILLED:
        linesAnimation(1);
        break;
      case ROTATE:
        rotateAnimation(0);
        break;
      case ROTATE_FILLED:
        rotateAnimation(1);
        break;
      case CIRCLES:
        circlesAnimation();
        break;
      default:  // Includes LOGO_OFF
        for (uint16_t i = LOGO_START; i < LOGO_END; i++) vRAM[i] = 0;
        break;
    }

    switch (currentSlot.text_type) {
      case STATIC:
        writeText(currentSlot.offset_scroll_speed, currentSlot.text,
                  currentSlot.scaler, currentSlot.char_space,
                  display_settings.textOnBrightness,
                  display_settings.textOffBrightness);
        if (currentSlot.seconds_scrolls &&
            (millis - currentSlot.slot_loaded_millis) / 1000 >=
                currentSlot.seconds_scrolls)
          advanceSlot = 1;
        break;
      case SCROLL:
        if (!scrollingDisplay()) {
          currentSlot.scroll_counter++;
          if (currentSlot.seconds_scrolls &&
              currentSlot.scroll_counter > currentSlot.seconds_scrolls) {
            scrollInterval = 0;
            advanceSlot = 1;
          } else {
            for (uint16_t i = TEXT_START; i < TEXT_END; i++) vRAM[i] = 0;
            if (currentSlot.offset_scroll_speed) {
              scrollInterval = currentSlot.offset_scroll_speed;
            } else {
              scrollInterval = 2;
            }
            writeScroll(currentSlot.text, currentSlot.scaler,
                        currentSlot.char_space,
                        display_settings.textOnBrightness,
                        display_settings.textOffBrightness);
          }
        }
        break;
      default:
        for (uint16_t i = TEXT_START; i < TEXT_END; i++) vRAM[i] = 0;
        if (currentSlot.seconds_scrolls &&
            (millis - currentSlot.slot_loaded_millis) / 1000 >=
                currentSlot.seconds_scrolls)
          advanceSlot = 1;
        break;
    }

  } else {
    if (millis - currentSlot.slot_loaded_millis > 1000) advanceSlot = 1;
  }
}

void battery_check() {
  static uint32_t lastBatCheck = 0;
  if ((uint32_t)(millis - lastBatCheck) >= 1000) {
    lastBatCheck = millis;
    if (!charging && voltage <= 3500) {
      chargeOn = 0;
      lowBat = 1;
      resetAnimations(0);
      for (uint16_t i = TEXT_START; i < LOGO_END; i++) vRAM[i] = 0;
      snprintf(battBuffer, BATT_BUFFER_SIZE, "Low Battery");
      writeText(3, battBuffer, 1, 0, 64, 0);
      _delay_ms(1000);
      checkButton();  // Will go to sleep if lowBat==1
    }
  }
}
void handle_state() {
  if (!(PIND & (1 << 3))) {
    if (!charging) {
      charging = 1;
      lowBat = 0;
      // Enable receiver and transmitter
      UCSRB |= 1 << RXEN | 1 << TXEN;
    }
  } else if ((PIND & (1 << 3))) {
    if (charging) {
      charging = 0;
      // Disable receiver and transmitter
      UCSRB &= ~(1 << RXEN | 1 << TXEN);
      lastCharge = millis;
    }
    if (chargeOn && millis - lastCharge > 3000) {
      chargeOn = 0;
      snprintf(battBuffer, BATT_BUFFER_SIZE, "Shutdown");
      for (uint16_t i = TEXT_START; i < LOGO_END; i++) vRAM[i] = 0;
      resetAnimations(0);
      for (uint8_t i = 64; i > 0; i--) {
        writeText(3, battBuffer, 1, 2, i, 0);
        _delay_ms(20);
      }
      deepSleep();
    }
  }
}
int main(void) {
  // Initialize all pixels to zero
  for (uint16_t i = TEXT_START; i < LOGO_END; i++) vRAM[i] = 0;

  // ######### Initialize UART #########
  // Set baud rate
  UBRRH = (unsigned char)(UBRR_VAL >> 8);
  UBRRL = (unsigned char)UBRR_VAL;
  // Set frame format: 8data, 1stop bit
  UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

  // ######### Initialize ADC #########
  // Set ADC MUX to ADC2 (pin PC2)
  ADMUX |= (1 << MUX2) | (1 << MUX0);
  // Enable ADC interrupt and set prescaler to 128 (8 MHz / 128 = 62.5 kHz)
  ADCSRA |= (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // ######### Initialize Timer 1 #########
  TCCR1B = (1 << WGM12) | (1 << CS11);  // CTC Mode
  TIMSK = (1 << OCIE1A);                // Enable Comp Match Interrupts

  OCR1A = 10000;

  SPCR &= ~(1 << SPE);   // Disable SPI
  TWCR &= ~(1 << TWEN);  // Disable TWI (I²C)
  ACSR |= (1 << ACD);    // Disable analog comparator

  sei();  // Enable global interrupts

  // Initialize pins
  PORTD |= (1 << 2);  // D2 Pullup (Button)
  PORTD |= (1 << 3);  // D3 Pullup (charging)
  DDRA |= (1 << 6);   // A6 to output (voltage divider)
  PORTA |= (1 << 6);  // A6 HIGH	(active low)
  PORTA |= (1 << 7);  // A7 Pullup (fully charged)

  advance_slot();

  while (1) {
    static uint32_t lastButton = 0;
    if ((uint32_t)(millis - lastButton) >= 10) {
      lastButton = millis;
      checkButton();
    }

    static uint32_t lastRefresh = 0;
    if ((uint32_t)(millis - lastRefresh) >= 20) {
      lastRefresh = millis;

      if (chargeOn) {
        fillLogoPercent(percent, 19, 32, 1);
        if (charging) {
          if (!(PINA & (1 << 7))) {
            logoDrawLine(0, 0, 14, 0, 32);
            snprintf(battBuffer, BATT_BUFFER_SIZE, "Battery Full");
            writeText(0, battBuffer, 1, 0, 64, 0);
          } else {
            snprintf(battBuffer, BATT_BUFFER_SIZE, "Charging");
            writeText(3, battBuffer, 1, 2, 64, 0);
          }
        } else {
          snprintf(battBuffer, BATT_BUFFER_SIZE, "Battery %d%%", percent);
          writeText(0, battBuffer, 1, 0, 64, 0);
        }
      } else {
        handle_slot();

        if (advanceSlot) {
          advance_slot();
          currentSlot.slot_loaded_millis = millis;
          if (!currentSlot.enabled) {
            // No enabled slot was found
            for (uint16_t i = TEXT_START; i < LOGO_END; i++) vRAM[i] = 0;
            writeText(7, "No Data", 1, 2, 5, 0);
          } else {
            if (currentSlot.animation != currentSlot.last_animation) {
              clearVRAM();
              resetAnimations(0);
            }
            currentSlot.last_animation = currentSlot.animation;
          }
          advanceSlot = 0;
        }
      }
    }

    handle_command_input();
    handle_state();
    battery_check();
  }
  return 0;
}

void updateButton() {
  button = (PIND & (1 << 2));
  if (!button && lastButton) {
    buttonPressMillis = millis;
    newButtonEvent = 1;
  } else if (button && !lastButton) {
    newButtonEvent = 1;
  }
  lastButton = button;
}

void checkButton() {
  updateButton();

  if (lowBat) goto lowBattery;

  if (button && newButtonEvent && millis - buttonPressMillis > 100 &&
      millis - buttonPressMillis < 1000) {
    newButtonEvent = 0;
    advanceSlot = 1;
  } else if (!button && newButtonEvent &&
             millis - buttonPressMillis > LONG_PRESS_TIME) {
    newButtonEvent = 0;

    snprintf(battBuffer, BATT_BUFFER_SIZE, "Shutdown");
    for (uint16_t i = TEXT_START; i < LOGO_END; i++) vRAM[i] = 0;
    resetAnimations(0);
    for (uint8_t i = 64; i > 0; i--) {
      writeText(3, battBuffer, 1, 2, i, 0);
      _delay_ms(20);
    }

  shutdownAfterButton:
    displayOn = 0;  // Disable Display
    while (!button) updateButton();
    newButtonEvent = 0;
    chargeOn = 0;
    _delay_ms(50);
    // Fake Deepsleep
    while (charging) {
      updateButton();

      if (!button && newButtonEvent &&
          millis - buttonPressMillis > LONG_PRESS_TIME) {
        newButtonEvent = 0;
        goto skipDeepSleep;  // Skip Deepsleep
      }

      if ((PIND & (1 << 3))) {
        charging = 0;
      }

      _delay_ms(50);
    }
    // Real Deepsleep
    _delay_ms(50);

  beforeDeepSleep:
    deepSleep();
    updateButton();
    while (millis - buttonPressMillis < LONG_PRESS_TIME) {
      updateButton();
      if (button) goto beforeDeepSleep;
    }
    newButtonEvent = 0;

    if (lowBat) {
      snprintf(battBuffer, BATT_BUFFER_SIZE, "Low Battery");
      for (uint16_t i = TEXT_START; i < LOGO_END; i++) vRAM[i] = 0;

      displayOn = 1;  // Enable Display

      for (uint8_t i = 0; i <= 64; i++) {
        writeText(3, battBuffer, 1, 0, i, 0);
        _delay_ms(20);
      }

    lowBattery:
      _delay_ms(1000);

      for (uint8_t i = 64; i > 0; i--) {
        writeText(3, battBuffer, 1, 0, i, 0);
        _delay_ms(20);
      }

      displayOn = 0;  // Disable Display

      goto shutdownAfterButton;
    }

  skipDeepSleep:
    snprintf(battBuffer, BATT_BUFFER_SIZE, "Start %03d", BOARD_ID);
    for (uint16_t i = TEXT_START; i < LOGO_END; i++) vRAM[i] = 0;
    displayOn = 1;  // Enable Display
    for (uint8_t i = 0; i <= 64; i++) {
      writeText(4, battBuffer, 1, 1, i, 0);
      _delay_ms(20);
    }
    // After Real Deepsleep
  } else if (!button && newButtonEvent &&
             millis - buttonPressMillis > MEDIUM_PRESS_TIME) {
    fillLogoPercent(percent, 19, 32, 1);
    snprintf(battBuffer, BATT_BUFFER_SIZE, "Battery %d%%", percent);
    writeText(0, battBuffer, 1, 0, 64, 0);
    _delay_ms(500);
    snprintf(battBuffer, BATT_BUFFER_SIZE, "%d Volt", voltage + 5);
    battBuffer[3] = battBuffer[2];
    battBuffer[2] = battBuffer[1];
    battBuffer[1] = '.';
    writeText(4, battBuffer, 1, 1, 64, 0);
    _delay_ms(500);
  }
}

void deepSleep() {
  // Ensure Voltage Divider is Off
  PORTA |= (1 << 6);
  // Ensure ADC is disabled
  ADCSRA &= ~(1 << ADEN);
  // Ensure internal voltage reference is disabled
  ADMUX &= ~(1 << REFS0 | 1 << REFS1);
  // Ensure UART receiver and transmitter are disabled
  UCSRB &= ~(1 << RXEN | 1 << TXEN);

  MCUCR |= (1 << SM1);  // Set sleep mode to Power Off
  GICR |= (1 << INT0);  // Enable INT0 for wake up
  GICR |= (1 << INT1);  // Enable INT1 for wake up

  // Go to Sleep

  MCUCR |= (1 << SE);        // Set sleep enable bit
  asm volatile("sleep" ::);  // Sleep
  MCUCR &= ~(1 << SE);       // Clear sleep enable bit

  // Wake Up

  updateButton();

  GICR &= ~(1 << INT0);  // Disable INT0
  GICR &= ~(1 << INT1);  // Disable INT1

  // ADCSRA |= (1 << ADEN);	// Enable ADC
}

ISR(INT0_vect) {
  // Needed for wakeup on button press
}

ISR(INT1_vect) {
  // Needed for wakeup on charging
  chargeOn = 1;
  lowBat = 0;
}

ISR(TIMER1_COMPA_vect) {
  if (displayOn) refreshDisplay();

  scrollTimerCounter++;

  if (scrollTimerCounter >= scrollInterval) {
    scrollTimerCounter = 0;
    scrollDisplay();
  }

  adcTimerCounter++;
  if (adcTimerCounter >= 100) {
    adcTimerCounter = 0;
    // Voltage Divider On (active low)
    PORTA &= ~(1 << 6);
    ADMUX |= (1 << REFS0) | (1 << REFS1);  // Select internal 2.56V reference
    ADCSRA |= (1 << ADEN);                 // Enable ADC
    _delay_us(600);
    ADCSRA |= (1 << ADSC);
  }

  handleUART();

  millis += 10;
}

ISR(ADC_vect) {
  ADCvalue = ADC;
  // Voltage Divider Off (active low)
  PORTA |= (1 << 6);
  voltage = (uint32_t)(ADCvalue + ADC_OFFSET) * ADC_GAIN / 1000;

  if (charging) {
    if (ADCvalue < CHARGE_MIN_ADC) {
      // percent = pgm_read_byte(&chargeLUT[0]);
      percent = 0;
    } else if (ADCvalue <= CHARGE_MAX_ADC) {
      percent = pgm_read_byte(&chargeLUT[ADCvalue - CHARGE_MIN_ADC]);
    } else {
      // percent = pgm_read_byte(&chargeLUT[CHARGE_MAX_ADC-DISCHARGE_MIN_ADC]);
      percent = 100;
    }
  } else {
    if (ADCvalue < DISCHARGE_MIN_ADC) {
      // percent = pgm_read_byte(&dischargeLUT[0]);
      percent = 0;
    } else if (ADCvalue <= DISCHARGE_MAX_ADC) {
      percent = pgm_read_byte(&dischargeLUT[ADCvalue - DISCHARGE_MIN_ADC]);
    } else {
      // percent =
      // pgm_read_byte(&dischargeLUT[DISCHARGE_MAX_ADC-DISCHARGE_MIN_ADC]);
      percent = 100;
    }
  }

  ADCSRA &= ~(1 << ADEN);               // Disable ADC
  ADMUX &= ~(1 << REFS0 | 1 << REFS1);  // Disable internal voltage reference
}
