#define MEDIUM_PRESS_TIME 1000
#define LONG_PRESS_TIME 2000

#define F_CPU 8000000UL
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
#include "display.h"
#include "logo.h"
#include "uart.h"
#define BAUD 300

#define F(string_literal) (PSTR(string_literal))

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

uint8_t newSlot = 1;
uint8_t scrollInterval = 0;
#define TEXT_BUFFER_SIZE 90
char textBuffer[TEXT_BUFFER_SIZE];
#define BATT_BUFFER_SIZE 15
char battBuffer[BATT_BUFFER_SIZE];

uint8_t animationOnBrightness = 32;
uint8_t animationOffBrightness = 0;

uint8_t textOnBrightness = 64;
uint8_t textOffBrightness = 0;

uint8_t lowBat = 0;

int main(void) {
  // Initialize all pixels to zero
  for (uint16_t i = 0; i < 522; i++) vRAM[i] = 0;

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

  uint8_t slot = 9;
  uint8_t lastSlot = 9;
  uint8_t slotVals[7] = {0, 0, 0, 0, 0, 0, 0};

  uint32_t slotLoadMillis = 0;
  uint8_t scrollCounter = 0;
  uint8_t lastAnimation = 0;

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
        if (slotVals[0] == 1) {
          switch (slotVals[1]) {
            case 1:
              for (uint16_t i = 420; i < 522; i++) vRAM[i] = 32;
              break;
            case 2:
              matrixAnimation();
              break;
            case 3:
              sweepAnimation(32, 1);
              break;
            case 4:
              waveAnimation();
              break;
            case 5:
              linesAnimation(0, animationOnBrightness, animationOffBrightness);
              break;
            case 6:
              linesAnimation(1, animationOnBrightness, animationOffBrightness);
              break;
            case 7:
              rotateAnimation(0, animationOnBrightness, animationOffBrightness);
              break;
            case 8:
              rotateAnimation(1, animationOnBrightness, animationOffBrightness);
              break;
            case 9:
              circlesAnimation(animationOnBrightness, animationOffBrightness);
              break;
            default:
              for (uint16_t i = 420; i < 522; i++) vRAM[i] = 0;
              break;
          }

          if (slotVals[3] == 1) {
            writeText(slotVals[4], textBuffer, slotVals[5], slotVals[6],
                      textOnBrightness, textOffBrightness);
            if (slotVals[2] && (millis - slotLoadMillis) / 1000 >= slotVals[2])
              newSlot = 1;
          } else if (slotVals[3] == 2) {
            if (!scrollingDisplay()) {
              scrollCounter++;
              if (slotVals[2] && scrollCounter > slotVals[2]) {
                scrollInterval = 0;
                newSlot = 1;
              } else {
                for (uint16_t i = 0; i < 420; i++) vRAM[i] = 0;
                if (slotVals[4]) {
                  scrollInterval = slotVals[4];
                } else {
                  scrollInterval = 2;
                }
                writeScroll(textBuffer, slotVals[5], slotVals[6],
                            textOnBrightness, textOffBrightness);
              }
            }
          } else {
            for (uint16_t i = 0; i < 420; i++) vRAM[i] = 0;
            if (slotVals[2] && (millis - slotLoadMillis) / 1000 >= slotVals[2])
              newSlot = 1;
          }
        } else {
          if (millis - slotLoadMillis > 1000) newSlot = 1;
        }

        if (newSlot) {
          slotLoadMillis = millis;
          do {
            slot++;
            if (slot > 9) slot = 0;
            slotVals[0] = eeprom_read_byte((const uint8_t *)(slot * 100));
          } while (slotVals[0] != 1 && slot != lastSlot);
          if (slotVals[0] != 1) {
            slot = lastSlot;
            slotVals[0] = eeprom_read_byte((const uint8_t *)(slot * 100));
          }
          scrollCounter = 0;
          scrollStop();
          if (slotVals[0] == 1) {
            for (uint8_t i = 1; i < 7; i++)
              slotVals[i] = eeprom_read_byte((const uint8_t *)(slot * 100 + i));
            if (slotVals[3]) {
              uint8_t skipByte = 0;
              for (uint8_t i = 0; i < 90; i++) {
                textBuffer[i] = eeprom_read_byte(
                    (const uint8_t *)(slot * 100 + 7 + i + skipByte));
                if (textBuffer[i] == 195) {
                  skipByte++;
                  textBuffer[i] = eeprom_read_byte(
                      (const uint8_t *)(slot * 100 + 7 + i + skipByte));
                }
                if (textBuffer[i] == 0) break;
              }
            }
            /*if(!uart_tx) {
                tx_len=snprintf(tx_buf, TX_BUFFER_SIZE, "Slot %d Loaded\r\n",
            slot); uart_tx=1;
            }*/

            lastSlot = slot;
          } else {
            /*if(!uart_tx) {
                uartWriteLn(F("No Slot Loaded"));
            }*/
            for (uint16_t i = 0; i < 522; i++) vRAM[i] = 0;
            writeText(7, "No Data", 1, 2, 5, 0);
          }
          if (slotVals[1] != lastAnimation) resetAnimations(0);
          lastAnimation = slotVals[1];
          newSlot = 0;
        }
      }
    }

    if (uart_rx) {
      uint8_t msg_pos = 0;
      /*if(rx_buf[0]=='H' || rx_buf[0]=='h') {
          if(rx_buf[1]==' ' && (rx_buf[2]=='W' || rx_buf[2]=='w')) {
              uartWriteLn(F("W <Slot> <Animation> <Duration (in s)> <Text Type>
      <X Offset> <Scaler> <Char Width> <Text>")); } else if(rx_buf[1]==' ' &&
      (rx_buf[2]=='R' || rx_buf[2]=='r')) { uartWriteLn(F("R <Slot>")); } else
      if(rx_buf[1]==' ' && (rx_buf[2]=='D' || rx_buf[2]=='d')) {
              uartWriteLn(F("D <Slot>"));
          } else if(rx_buf[1]==' ' && (rx_buf[2]=='E' || rx_buf[2]=='e')) {
              uartWriteLn(F("E <Slot>"));
          } else if(rx_buf[1]==' ' && (rx_buf[2]=='F' || rx_buf[2]=='f')) {
              uartWriteLn(F("F <Slot/All>"));
          } else if(rx_buf[1]==' ' && (rx_buf[2]=='L' || rx_buf[2]=='l')) {
              uartWriteLn(F("L"));
          } else if(rx_buf[1]==' ' && (rx_buf[2]=='L' || rx_buf[2]=='l')) {
              uartWriteLn(F("B <Animation On Brightness> <Animation Off
      Brightness> <Text On Brightness> <Text Off Brightness>")); } else {
              uartWriteLn(F("W: Write slot\r\nR: Read slot\r\nD: Disable
      slot\r\nE: Enable slot\r\nF: Format\r\nL: List active slots\r\nB:
      Brightness"));
          }
      } else */
      if (rx_buf[0] == 'W' || rx_buf[0] == 'w') {
        msg_pos += 2;
        if (msg_pos >= rx_len || rx_buf[1] != ' ') goto skipW;
        uint8_t values[7] = {0, 0, 0, 0, 0, 0, 0};
        // [0] Slot, [1] Animation, [2] Duration, [3] Text Type
        // [4] X Offset, [5] Scaler, [6] Char Width

        for (uint8_t i = 0; i < 7; i++) {
          // uint8_t prev_msg_pos = msg_pos;
          while (rx_buf[msg_pos] != ' ') {
            values[i] = values[i] * 10 + (rx_buf[msg_pos] - '0');
            msg_pos++;
            if (msg_pos >= rx_len) goto skipW;
          }

          msg_pos++;
          if (msg_pos >= rx_len) goto skipW;
        }

        eeprom_write_byte((uint8_t *)(values[0] * 100), (uint8_t)1);
        for (uint8_t i = 1; i < 7; i++) {
          eeprom_write_byte((uint8_t *)(values[0] * 100 + i), values[i]);
          if (i == 3 && !values[i]) goto skip_string;
        }

        for (uint8_t i = 0; i < rx_len - msg_pos; i++) {
          eeprom_write_byte((uint8_t *)(values[0] * 100 + 7 + i),
                            rx_buf[msg_pos + i]);
          eeprom_write_byte((uint8_t *)(values[0] * 100 + 7 + rx_len - msg_pos),
                            0);
        }

        if (values[3]) {
          tx_len = snprintf(tx_buf, TX_BUFFER_SIZE,
                            "Slot %d:\r\n%d %d %d %d %d %d %d\r\n", values[0],
                            values[0], values[1], values[2], values[3],
                            values[4], values[5], values[6]);
          for (uint8_t i = 0; i < 90; i++) {
            tx_len++;
            tx_buf[tx_len] =
                eeprom_read_byte((const uint8_t *)(values[0] * 100 + 7 + i));
            if (tx_buf[tx_len] == 0 || i == 94) {
              tx_buf[tx_len] = '\r';
              tx_buf[tx_len + 1] = '\n';
              tx_len += 2;
              break;
            }
          }
        } else {
        skip_string:
          tx_len = snprintf(tx_buf, TX_BUFFER_SIZE,
                            "Slot %d:\r\n%d %d\r\nNo Text\r\n", values[0],
                            values[1], values[2]);
        }

        uart_tx = 1;

        if (0) {
        skipW:
          uartWriteLn(
              F("W <Slot> <Animation> <Seconds/Scrolls> <Text Type> "
                "<Offset/Scroll Speed> <Scaler> <Char Space> <Text>"));
        }

      } else if (rx_buf[0] == 'R' || rx_buf[0] == 'r') {
        if (rx_len == 3 && rx_buf[1] == ' ' && rx_buf[2] >= '0' &&
            rx_buf[2] <= '9') {
          uint8_t slotNum = (rx_buf[2] - '0');
          if (eeprom_read_byte((const uint8_t *)(slotNum * 100)) != 1) {
            tx_len = snprintf(tx_buf, TX_BUFFER_SIZE,
                              "Slot %d is not active.\r\n", slotNum);
          } else {
            uint8_t values[7] = {0, 0, 0, 0, 0, 0, 0};
            // [0] Slot, [1] Animation, [2] Duration, [3] Text Type
            // [4] X Offset, [5] Scaler, [6] Char Width
            values[0] = slotNum;
            for (uint8_t i = 1; i < 7; i++)
              values[i] =
                  eeprom_read_byte((const uint8_t *)(slotNum * 100 + i));

            if (values[3] != 0) {
              tx_len = snprintf(tx_buf, TX_BUFFER_SIZE,
                                "Slot %d:\r\n%d %d %d %d %d %d %d\r\n", slotNum,
                                values[0], values[1], values[2], values[3],
                                values[4], values[5], values[6]);
              for (uint8_t i = 0; i < 90; i++) {
                tx_len++;
                tx_buf[tx_len] =
                    eeprom_read_byte((const uint8_t *)(slotNum * 100 + 7 + i));
                if (tx_buf[tx_len] == 0 || i == 89) {
                  tx_buf[tx_len] = '\r';
                  tx_buf[tx_len + 1] = '\n';
                  tx_len += 2;
                  break;
                }
              }
            } else {
              tx_len = snprintf(tx_buf, TX_BUFFER_SIZE,
                                "Slot %d:\r\n%d %d\r\nNo Text...\r\n", slotNum,
                                values[1], values[2]);
            }
          }
          uart_tx = 1;
        } else {
          uartWriteLn(F("R <Slot>"));
        }

      } else if (rx_buf[0] == 'D' || rx_buf[0] == 'd') {
        if (rx_len == 3 && rx_buf[1] == ' ' && rx_buf[2] >= '0' &&
            rx_buf[2] <= '9') {
          uint8_t slot = (rx_buf[2] - '0');
          eeprom_write_byte((uint8_t *)(slot * 100), (uint8_t)0);
          tx_len =
              snprintf(tx_buf, TX_BUFFER_SIZE, "Slot %d disabled.\r\n", slot);
        } else {
          uartWriteLn(F("D <Slot>"));
        }

        uart_tx = 1;
      } else if (rx_buf[0] == 'E' || rx_buf[0] == 'e') {
        if (rx_len == 3 && rx_buf[1] == ' ' && rx_buf[2] >= '0' &&
            rx_buf[2] <= '9') {
          uint8_t slot = (rx_buf[2] - '0');
          eeprom_write_byte((uint8_t *)(slot * 100), (uint8_t)1);
          tx_len =
              snprintf(tx_buf, TX_BUFFER_SIZE, "Slot %d enabled.\r\n", slot);
          uart_tx = 1;
        } else {
          uartWriteLn(F("E <Slot>"));
        }
      } else if (rx_buf[0] == 'F' || rx_buf[0] == 'f') {
        if (rx_len >= 3 && rx_buf[1] == ' ') {
          if (rx_buf[2] >= '0' && rx_buf[2] <= '9') {
            uint8_t slot = (rx_buf[2] - '0');
            for (uint16_t i = slot * 100; i < slot * 100 + 100; i++) {
              eeprom_write_byte((uint8_t *)(i), (uint8_t)0);
            }
            tx_len =
                snprintf(tx_buf, TX_BUFFER_SIZE, "Slot %d cleared\r\n", slot);
          } else if ((rx_buf[2] >= 'A' || rx_buf[2] <= 'a') &&
                     (rx_buf[3] >= 'L' || rx_buf[3] <= 'l') &&
                     (rx_buf[4] >= 'L' || rx_buf[4] <= 'l')) {
            for (uint16_t i = 0; i < 1024; i++) {
              eeprom_write_byte((uint8_t *)(i), (uint8_t)0);
            }
            tx_len = snprintf(tx_buf, TX_BUFFER_SIZE, "Memory cleared\r\n");
          } else {
            goto invalidF;
          }
          uart_tx = 1;
        } else {
        invalidF:
          uartWriteLn(F("F <Slot/All>"));
        }
      } else if (rx_len == 1 && (rx_buf[0] == 'L' || rx_buf[0] == 'l')) {
        tx_len = 0;
        for (uint8_t i = 0; i < 10; i++) {
          if (eeprom_read_byte((const uint8_t *)(i * 100)) == 1) {
            tx_len +=
                snprintf(tx_buf + tx_len, TX_BUFFER_SIZE - tx_len, "%d: ", i);
            uint8_t values[7] = {0, 0, 0, 0, 0, 0, 0};
            // [0] Slot, [1] Animation, [2] Duration, [3] Text Type
            // [4] X Offset, [5] Scaler, [6] Char Width
            values[0] = i;
            for (uint8_t j = 1; j < 7; j++)
              values[j] = eeprom_read_byte((const uint8_t *)(i * 100 + j));

            if (values[3] != 0) {
              for (uint8_t j = 0; j < 8; j++) {
                tx_len++;
                tx_buf[tx_len] =
                    eeprom_read_byte((const uint8_t *)(i * 100 + 7 + j));
                if (tx_buf[tx_len] == 0) {
                  tx_buf[tx_len] = '\r';
                  tx_buf[tx_len + 1] = '\n';
                  tx_len += 2;
                  break;
                } else if (j == 7) {
                  if (tx_buf[tx_len] == 195) tx_len--;
                  tx_buf[tx_len + 1] = '\r';
                  tx_buf[tx_len + 2] = '\n';
                  tx_len += 3;
                }
              }
            } else {
              tx_len +=
                  snprintf(tx_buf + tx_len, TX_BUFFER_SIZE - tx_len, "-\r\n");
            }
          }
        }
        if (tx_len == 0) {
          uartWriteLn(F("No Data..."));
        } else {
          uart_tx = 1;
        }
      } else if (rx_buf[0] == 'B' || rx_buf[0] == 'b') {
        msg_pos += 2;
        if (msg_pos >= rx_len) goto skipB;
        uint8_t values[4] = {0, 0, 0, 0};
        // [0] Animation On Brightness, [1] Animation Off Brightness,
        // [2] Text On Brightness, [3] Text Off Brightness

        for (uint8_t i = 0; i < 4; i++) {
          while (rx_buf[msg_pos] != ' ') {
            values[i] = values[i] * 10 + (rx_buf[msg_pos] - '0');
            msg_pos++;
            if (msg_pos >= rx_len && i < 3) goto skipB;
          }

          msg_pos++;
          if (msg_pos >= rx_len && i < 3) goto skipB;
        }

        animationOnBrightness = values[0];
        animationOffBrightness = values[1];

        textOnBrightness = values[2];
        textOffBrightness = values[3];

        if (0) {
        skipB:
          uartWriteLn(
              F("B <Logo Bright> <Logo Dark> <Text Bright> <Text Dark>"));
        }
      } else {
        // uartWriteLn(F("Invalid command. Available commands:\r\nW: Write
        // Slot\r\nR: Read Slot\r\nE: Enable Slot\r\nD: Disable Slot\r\nF:
        // Format\r\nL: List\r\nB: Brightness"));
        uartWriteLn(
            F("Invalid command.\r\nW: Write\r\nR: Read\r\nE: Enable\r\nD: "
              "Disable\r\nL: List\r\nF: Format\r\nB: Brightness"));
      }

      uart_rx = 0;
      rx_len = 0;
    }

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
        for (uint16_t i = 0; i < 522; i++) vRAM[i] = 0;
        resetAnimations(0);
        for (uint8_t i = 64; i > 0; i--) {
          writeText(3, battBuffer, 1, 2, i, 0);
          _delay_ms(20);
        }
        deepSleep();
      }
    }

    static uint32_t lastBatCheck = 0;
    if ((uint32_t)(millis - lastBatCheck) >= 1000) {
      lastBatCheck = millis;
      if (!charging && voltage <= 3500) {
        chargeOn = 0;
        lowBat = 1;
        resetAnimations(0);
        for (uint16_t i = 0; i < 522; i++) vRAM[i] = 0;
        snprintf(battBuffer, BATT_BUFFER_SIZE, "Low Battery");
        writeText(3, battBuffer, 1, 0, 64, 0);
        _delay_ms(1000);
        checkButton();  // Will go to sleep if lowBat==1
      }
    }
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
    newSlot = 1;
  } else if (!button && newButtonEvent &&
             millis - buttonPressMillis > LONG_PRESS_TIME) {
    newButtonEvent = 0;

    snprintf(battBuffer, BATT_BUFFER_SIZE, "Shutdown");
    for (uint16_t i = 0; i < 522; i++) vRAM[i] = 0;
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
      for (uint16_t i = 0; i < 522; i++) vRAM[i] = 0;

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
    for (uint16_t i = 0; i < 522; i++) vRAM[i] = 0;
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
