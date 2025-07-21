#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "adc.h"
#include "animations.h"
#include "button.h"
#include "charge.h"
#include "charlieplex.h"
#include "cmd.h"
#include "display.h"
#include "logo.h"
#include "slots.h"
#include "timer.h"
#include "uart.h"

int main(void) {
  // Initialize all pixels to zero
  clear_vram();

  // ######### Initialize UART #########
  uart_initialize();

  // ######### Initialize ADC #########
  adc_init();

  // ######### Initialize Timer 1 #########
  timer_init();

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

  slot_advance();

  // MAIN LOOP
  while (1) {
    static uint32_t lastButton = 0;
    if ((uint32_t)(global_millis - lastButton) >= 10) {
      lastButton = global_millis;
      button_check();
    }

    static uint32_t lastRefresh = 0;
    if ((uint32_t)(global_millis - lastRefresh) >= 20) {
      lastRefresh = global_millis;

      if (charge_data.charge_on) {
        charge_display_state();
      } else {
        slot_handle();
      }
    }

    handle_command_input();
    charge_handle_state();
    charge_battery_check();
  }
  return 0;
}
