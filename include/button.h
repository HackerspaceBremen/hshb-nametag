#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>

#define BUTTON_MEDIUM_PRESS_TIME 1000
#define BUTTON_LONG_PRESS_TIME 2000

struct ButtonState {
  uint8_t button;
  uint8_t last_button;

  uint8_t new_button_event;
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
