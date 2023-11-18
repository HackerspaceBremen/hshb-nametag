#include "button.h"

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <util/delay.h>

#include "adc.h"
#include "animations.h"
#include "charge.h"
#include "display.h"
#include "logo.h"
#include "slots.h"
#include "timer.h"

volatile struct ButtonState button_state = {
    .button = 1,
    .last_button = 1,
    .new_button_event = 0,
    .button_press_millis = 0,
};

void button_update() {
  button_state.button = (PIND & (1 << 2));
  if (button_state.button != button_state.last_button) {
    if (!button_state.button) button_state.button_press_millis = global_millis;
    button_state.new_button_event = 1;
  }
  button_state.last_button = button_state.button;
}

void button_check() {
  button_update();

  if (charge_data.low_battery) goto lowBattery;

  if (button_state.button && button_state.new_button_event &&
      global_millis - button_state.button_press_millis > 100 &&
      global_millis - button_state.button_press_millis < 1000) {
    button_state.new_button_event = 0;
    slot_advance();
  } else if (!button_state.button && button_state.new_button_event &&
             global_millis - button_state.button_press_millis >
                 BUTTON_LONG_PRESS_TIME) {
    button_state.new_button_event = 0;

    snprintf((char*)display_state.sysmsg_buffer, SYSMSG_BUFFER_SIZE,
             "Shutdown");
    clear_vram();
    resetAnimations(0);
    for (uint8_t i = 64; i > 0; i--) {
      display_write_text(3, (char*)display_state.sysmsg_buffer, 1, 2, i, 0);
      _delay_ms(20);
    }

  shutdownAfterButton:
    display_state.display_on = 0;  // Disable Display
    while (!button_state.button) button_update();
    button_state.new_button_event = 0;
    charge_data.charge_on = 0;
    _delay_ms(50);
    // Fake Deepsleep
    while (charge_data.charging) {
      button_update();

      if (!button_state.button && button_state.new_button_event &&
          global_millis - button_state.button_press_millis >
              BUTTON_LONG_PRESS_TIME) {
        button_state.new_button_event = 0;
        goto skipDeepSleep;  // Skip Deepsleep
      }

      if ((PIND & (1 << 3))) {
        charge_data.charging = 0;
      }

      _delay_ms(50);
    }
    // Real Deepsleep
    _delay_ms(50);

  beforeDeepSleep:
    deep_sleep();
    button_update();
    while (global_millis - button_state.button_press_millis <
           BUTTON_LONG_PRESS_TIME) {
      button_update();
      if (button_state.button) goto beforeDeepSleep;
    }
    button_state.new_button_event = 0;

    if (charge_data.low_battery) {
      snprintf((char*)display_state.sysmsg_buffer, SYSMSG_BUFFER_SIZE,
               "Low Battery");
      clear_vram();

      display_state.display_on = 1;  // Enable Display

      for (uint8_t i = 0; i <= 64; i++) {
        display_write_text(3, (char*)display_state.sysmsg_buffer, 1, 0, i, 0);
        _delay_ms(20);
      }

    lowBattery:
      _delay_ms(1000);

      for (uint8_t i = 64; i > 0; i--) {
        display_write_text(3, (char*)display_state.sysmsg_buffer, 1, 0, i, 0);
        _delay_ms(20);
      }

      display_state.display_on = 0;  // Disable Display

      goto shutdownAfterButton;
    }

  skipDeepSleep:
    snprintf((char*)display_state.sysmsg_buffer, SYSMSG_BUFFER_SIZE,
             "Start %03d", BOARD_ID);
    clear_vram();
    display_state.display_on = 1;  // Enable Display
    for (uint8_t i = 0; i <= 64; i++) {
      display_write_text(4, (char*)display_state.sysmsg_buffer, 1, 1, i, 0);
      _delay_ms(20);
    }
    // After Real Deepsleep
  } else if (!button_state.button && button_state.new_button_event &&
             global_millis - button_state.button_press_millis >
                 BUTTON_MEDIUM_PRESS_TIME) {
    fillLogoPercent(adc_values.percent, 19, 32, 1);
    snprintf((char*)display_state.sysmsg_buffer, SYSMSG_BUFFER_SIZE,
             "Battery %d%%", adc_values.percent);
    display_write_text(0, (char*)display_state.sysmsg_buffer, 1, 0, 64, 0);
    _delay_ms(500);
    snprintf((char*)display_state.sysmsg_buffer, SYSMSG_BUFFER_SIZE, "%d Volt",
             adc_values.voltage + 5);
    display_state.sysmsg_buffer[3] = display_state.sysmsg_buffer[2];
    display_state.sysmsg_buffer[2] = display_state.sysmsg_buffer[1];
    display_state.sysmsg_buffer[1] = '.';
    display_write_text(4, (char*)display_state.sysmsg_buffer, 1, 1, 64, 0);
    _delay_ms(500);
  }
}

ISR(INT0_vect) {
  // Needed for wakeup on button press
}
