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
  // != 0 because .button is actually a single bit, and writing 1<<2 (4) into it
  // results in always writing 0
  button_state.button = (PIND & (1 << 2)) != 0;
  if (button_state.button != button_state.last_button) {
    if (!button_state.button) button_state.button_press_millis = global_millis;
    button_state.new_button_event = 1;
  }
  button_state.last_button = button_state.button;
}

void on_low_battery() {
  display_fade_text("Low Battery", 0);
  display_state.display_on = 0;  // Disable Display
}

void wake_up_routine() {
  snprintf((char*)display_state.sysmsg_buffer, SYSMSG_BUFFER_SIZE, "Start %03d",
           BOARD_ID);
  display_fade_text("", 0);
}

void enter_deepsleep() {
  // Waits for the button to be released and then enters depsleep
  display_state.display_on = 0;  // Disable Display
  while (!button_state.button) button_update();
  button_state.new_button_event = 0;
  charge_data.charge_on = 0;
  _delay_ms(50);

  // Fake deep sleep while charging
  while (charge_data.charging) {
    // Do not enter deep sleep while charging
    button_update();

    if (!button_state.button && button_state.new_button_event &&
        global_millis - button_state.button_press_millis >
            BUTTON_LONG_PRESS_TIME) {
      // Button was pressed, wake back up
      button_state.new_button_event = 0;
      wake_up_routine();
      return;
    }

    if (CHARGING_PIN_HIGH) {
      // Continue to deep sleep
      charge_data.charging = 0;
      break;
    }

    _delay_ms(50);
  }

  // Real deep sleep
  _delay_ms(50);

  while (1) {
    deep_sleep();

    // Deep sleep can only be exited by pressing the button for the long press
    // time
    button_update();
    while (global_millis - button_state.button_press_millis <
           BUTTON_LONG_PRESS_TIME) {
      button_update();
      if (!button_state.button) break;
    }

    if (button_state.button) {
      // The button is still pressed, wake up
      break;
    }
  }

  button_state.new_button_event = 0;
  if (charge_data.low_battery) {
    on_low_battery();
  }

  wake_up_routine();
}

void button_check() {
  button_update();

  if (charge_data.low_battery) {
    on_low_battery();
    enter_deepsleep();
  };

  uint32_t button_press_duration =
      global_millis - button_state.button_press_millis;
  if (button_state.button && button_state.new_button_event &&
      button_press_duration > BUTTON_SHORT_PRESS_TIME &&
      button_press_duration < BUTTON_MEDIUM_PRESS_TIME) {
    button_state.new_button_event = 0;
    slot_advance();
  } else if (!button_state.button && button_state.new_button_event &&
             button_press_duration > BUTTON_LONG_PRESS_TIME) {
    button_state.new_button_event = 0;
    display_fade_text("Shutdown", 1);
    enter_deepsleep();
  } else if (!button_state.button && button_state.new_button_event &&
             button_press_duration > BUTTON_MEDIUM_PRESS_TIME) {
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
