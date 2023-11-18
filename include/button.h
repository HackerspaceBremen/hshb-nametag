#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>

/**
 * Button press duration
 * - Lower than short press time: ignore (debouncing)
 * - Between short and medium press time: advance to next slot immediately
 * - Between medium and long press time: Display battery state (percent &
 * voltage)
 * - Higher than long press time: Shutdown
 */
#define BUTTON_SHORT_PRESS_TIME 100
#define BUTTON_MEDIUM_PRESS_TIME 1000
#define BUTTON_LONG_PRESS_TIME 2000

struct ButtonState {
  uint8_t button : 1;
  uint8_t last_button : 1;

  uint8_t new_button_event : 1;
  uint32_t button_press_millis;

  uint8_t animation_brightness_on;
  uint8_t animation_brightness_off;

  uint8_t text_brightness_on;
  uint8_t text_brightness_off;
};

extern volatile struct ButtonState button_state;

void button_update();
void button_check();

#endif
