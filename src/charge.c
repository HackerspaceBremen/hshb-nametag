#include "charge.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "adc.h"
#include "animations.h"
#include "button.h"
#include "display.h"
#include "logo.h"
#include "timer.h"
#include "uart.h"

volatile struct ChargeData charge_data = {
    .charging = 0,
    .low_battery = 0,
    .charge_on = 0,
    .last_charge = 0,
};

void charge_display_state() {
  logo_fill_percent(adc_values.percent, 19, 32, 1);
  if (charge_data.charging) {
    if (!(PINA & (1 << 7))) {
      logo_draw_line(0, 0, 14, 0, 32);
      snprintf((char*)display_state.sysmsg_buffer, SYSMSG_BUFFER_SIZE,
               STR_BATTERY_FULL);
      display_write_text(0, (char*)display_state.sysmsg_buffer, 1, 0, 64, 0);
    } else {
      snprintf((char*)display_state.sysmsg_buffer, SYSMSG_BUFFER_SIZE,
               STR_BATTERY_CHRG);
      display_write_text(3, (char*)display_state.sysmsg_buffer, 1, 2, 64, 0);
    }
  } else {
    snprintf((char*)display_state.sysmsg_buffer, SYSMSG_BUFFER_SIZE,
             STR_BATTERY_PERC, adc_values.percent);
    display_write_text(0, (char*)display_state.sysmsg_buffer, 1, 0, 64, 0);
  }
}

void charge_battery_check() {
  static uint32_t lastBatCheck = 0;
  if ((uint32_t)(global_millis - lastBatCheck) >= 1000) {
    lastBatCheck = global_millis;
    if (!charge_data.charging && adc_values.voltage <= 3500) {
      charge_data.charge_on = 0;
      charge_data.low_battery = 1;
      animations_reset();
      clear_vram();
      snprintf((char*)display_state.sysmsg_buffer, SYSMSG_BUFFER_SIZE,
               STR_BATTERY_LOW);
      // REDUCE BRIGHTNESS TO MINIMUM VALUE 1 FOR LOGO, KEEP BRIGHTNESS OF TEXT
      // AT 64
      display_write_text(3, (char*)display_state.sysmsg_buffer, 1, 0, 64, 0);
      _delay_ms(1000);
      button_check();  // Will go to sleep if lowBat==1
    }
  }
}

void charge_handle_state() {
  if (!CHARGING_PIN_HIGH) {
    if (!charge_data.charging) {
      charge_data.charging = 1;
      charge_data.low_battery = 0;
      // Enable receiver and transmitter
      uart_enable();
    }
  } else if (CHARGING_PIN_HIGH) {
    if (charge_data.charging) {
      charge_data.charging = 0;
      // Disable receiver and transmitter
      uart_disable();
      charge_data.last_charge = global_millis;
    }
    if (charge_data.charge_on &&
        global_millis - charge_data.last_charge > 3000) {
      charge_data.charge_on = 0;
      snprintf((char*)display_state.sysmsg_buffer, SYSMSG_BUFFER_SIZE,
               STR_DEVICE_OFF);
      clear_vram();
      animations_reset();
      for (uint8_t i = 64; i > 0; i--) {
        display_write_text(3, (char*)display_state.sysmsg_buffer, 1, 2, i, 0);
        _delay_ms(20);
      }
      deep_sleep();
    }
  }
}

void deep_sleep() {
  // Ensure Voltage Divider is Off
  PORTA |= (1 << 6);
  // Ensure ADC is disabled
  adc_disable();
  // Ensure UART receiver and transmitter are disabled
  uart_disable();

  MCUCR |= (1 << SM1);  // Set sleep mode to Power Off
  GICR |= (1 << INT0);  // Enable INT0 for wake up
  GICR |= (1 << INT1);  // Enable INT1 for wake up

  // Go to Sleep

  MCUCR |= (1 << SE);        // Set sleep enable bit
  asm volatile("sleep" ::);  // Sleep
  MCUCR &= ~(1 << SE);       // Clear sleep enable bit

  // Wake Up
  button_update();

  GICR &= ~(1 << INT0);  // Disable INT0
  GICR &= ~(1 << INT1);  // Disable INT1
}

ISR(INT1_vect) {
  // Needed for wakeup on charging
  charge_data.charge_on = 1;
  charge_data.low_battery = 0;
}
