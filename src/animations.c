#include <avr/pgmspace.h>

#include "display.h"
#include "logo.h"
#include "timer.h"

int8_t i = 0;
int8_t j = 0;
uint8_t step = 0;
uint8_t quarter = 0;
uint8_t counter0 = 0;
uint8_t counter1 = 0;
uint8_t counter2 = 0;

const uint8_t circleXPos[] PROGMEM = {6, 7, 9, 2, 12};
const uint8_t circleYPos[] PROGMEM = {2, 12, 7, 11, 14};
const uint8_t bArray[] PROGMEM = {
    5,  5,  7,  10, 13, 18, 23, 28, 34, 39, 44, 49, 53, 57, 59, 61, 63, 64,
    63, 61, 59, 57, 53, 49, 44, 39, 34, 28, 23, 18, 13, 10, 7,  5,  5};
uint8_t logoLines[15] = {4, 13, 6, 16, 2, 15, 7, 11, 15, 8, 10, 1, 17, 9, 18};

void animations_reset() {
  i = 0;
  j = 0;
  step = 0;
  quarter = 0;
  counter0 = 0;
  counter1 = 0;
  counter2 = 0;
}

void animation_line(uint8_t fill) {
  uint8_t on = display_state.animation_brightness_on;
  uint8_t off = display_state.animation_brightness_off;
  if (counter0 < 1) {
    counter0++;
  } else {
    counter0 = 0;
    if (step == 0) {
      if (!fill) {
        clear_vram_logo();
      }
      logo_draw_line(i, 0, i, 18, on);
      i++;
      if (i >= 15) {
        i = 0;
        step++;
      }
    } else if (step == 1) {
      if (fill) {
        logo_draw_line(0, i, 14, i, off);
      } else {
        clear_vram_logo();
        logo_draw_line(0, i, 14, i, on);
      }
      i++;
      if (i >= 19) {
        i = 0;
        step++;
      }
    } else if (step == 2) {
      if (!fill) {
        clear_vram_logo();
      }
      logo_draw_line(14 - i, 0, 14 - i, 18, on);
      i++;
      if (i >= 15) {
        i = 0;
        step++;
      }
    } else if (step == 3) {
      if (fill) {
        logo_draw_line(0, 18 - i, 14, 18 - i, off);
      } else {
        clear_vram_logo();
        logo_draw_line(0, 18 - i, 14, 18 - i, on);
      }
      i++;
      if (i >= 19) {
        i = 0;
        step = 0;
        return;
      }
    }
  }
}

void animation_rotate(uint8_t fill) {
  uint8_t on = display_state.animation_brightness_on;
  uint8_t off = display_state.animation_brightness_off;
  if (quarter % 2 == 0) {
    if (fill) {
      if (quarter == 0) {
        logo_draw_line(i, 0, 14 - i, 18, on);
      } else {
        logo_draw_line(i, 0, 14 - i, 18, off);
      }
    } else {
      clear_vram_logo();
      logo_draw_line(i, 0, 14 - i, 18, on);
    }
    i++;
    if (i >= 15) {
      i = 0;
      quarter++;
    }
  } else {
    if (fill) {
      if (quarter == 1) {
        logo_draw_line(0, 18 - i, 14, i, on);
      } else {
        logo_draw_line(0, 18 - i, 14, i, off);
      }
    } else {
      clear_vram_logo();
      logo_draw_line(0, 18 - i, 14, i, on);
    }
    i++;
    if (i >= 19) {
      i = 1;
      quarter++;
    }
  }
  if (quarter >= 4) {
    quarter = 0;
  }
}

void animation_circles() {
  if (counter0 < 2) {
    counter0++;
  } else {
    counter0 = 0;
    for (uint16_t k = LOGO_START; k < LOGO_END; k++)
      vRAM[k] = display_state.animation_brightness_off;
    logo_draw_circle(pgm_read_byte(&circleXPos[i]),
                     pgm_read_byte(&circleYPos[i]), j,
                     display_state.animation_brightness_on);

    j++;
    if (j >= 25) {
      j = 0;
      i++;
      if (i >= 5) {
        i = 0;
      }
    }
  }
}

void animation_matrix() {
  if (counter0 > 6) {
    counter0 = 0;

    for (uint8_t i = 0; i < 5; i++) {
      uint8_t t = i * 3;
      logo_set_xy(t, logoLines[t], 255);
      logo_set_xy(t, logoLines[t] - 1, 128);
      logo_set_xy(t, logoLines[t] - 2, 64);
      logo_set_xy(t, logoLines[t] - 3, 32);
      logo_set_xy(t, logoLines[t] - 4, 16);
      logo_set_xy(t, logoLines[t] - 5, 8);
      logo_set_xy(t, logoLines[t] - 6, 0);
      logoLines[t]++;
      if (logoLines[t] >= 25) logoLines[t] = 0;
    }
  }

  if (counter1 > 5) {
    counter1 = 0;

    for (uint8_t i = 0; i < 5; i++) {
      uint8_t t = i * 3 + 1;
      logo_set_xy(t, logoLines[t], 255);
      logo_set_xy(t, logoLines[t] - 1, 128);
      logo_set_xy(t, logoLines[t] - 2, 64);
      logo_set_xy(t, logoLines[t] - 3, 32);
      logo_set_xy(t, logoLines[t] - 4, 16);
      logo_set_xy(t, logoLines[t] - 5, 8);
      logo_set_xy(t, logoLines[t] - 6, 0);
      logoLines[t]++;
      if (logoLines[t] >= 25) logoLines[t] = 0;
    }
  }

  if (counter2 > 7) {
    counter2 = 0;

    for (uint8_t i = 0; i < 5; i++) {
      uint8_t t = i * 3 + 2;
      logo_set_xy(t, logoLines[t], 255);
      logo_set_xy(t, logoLines[t] - 1, 128);
      logo_set_xy(t, logoLines[t] - 2, 64);
      logo_set_xy(t, logoLines[t] - 3, 32);
      logo_set_xy(t, logoLines[t] - 4, 16);
      logo_set_xy(t, logoLines[t] - 5, 8);
      logo_set_xy(t, logoLines[t] - 6, 0);
      logoLines[t]++;
      if (logoLines[t] >= 25) logoLines[t] = 0;
    }
  }

  counter0++;
  counter1++;
  counter2++;
}

void animation_sweep() {
  uint8_t on = display_state.animation_brightness_on;
  uint8_t off = display_state.animation_brightness_off;
  j--;
  if (j == -127) {
    j = 18;
    i = !i;
  }

  for (int x = 0; x < 15; x++) {
    for (int y = 0; y < 19; y++) {
      if (j > y) {
        if (i == 1) {
          logo_set_xy(x, y, off);
        } else {
          logo_set_xy(x, y, on);
        }
      } else {
        if (i == 1) {
          logo_set_xy(x, y, off);
        } else {
          logo_set_xy(x, y, on);
        }
      }
    }
  }
}

void animation_wave() {
  if (counter0 < 1) {
    counter0++;
  } else {
    counter0 = 0;

    i = j;
    for (uint8_t k = 0; k <= 18; k++) {
      logo_draw_line(0, k, 14, k, pgm_read_byte(&bArray[i]) + 10);
      i++;
      if (i >= 35) i = 0;
    }
    j++;
    if (j >= 35) j = 0;
  }
}
