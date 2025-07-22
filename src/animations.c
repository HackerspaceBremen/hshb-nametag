#include <avr/pgmspace.h>
#include <util/delay.h>

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
uint8_t x = 0;
uint8_t y = 0;
uint8_t pseudo_random = 0;

// GETS CALLED BEFORE EACH NEW ANIMATION
void animations_reset() {
  // PROVIDES GLOBAL ITERATION-VARS TO EACH ANIMATION METHOD FOR FREE USE
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
      logo_draw_line(i, LOGO_ROW_FIRST, i, LOGO_ROW_LAST, on);
      i++;
      if (i >= 15) {
        i = 0;
        step++;
      }
    } else if (step == 1) {
      if (fill) {
        logo_draw_line(LOGO_COL_FIRST, i, LOGO_COL_LAST, i, off);
      } else {
        clear_vram_logo();
        logo_draw_line(LOGO_COL_FIRST, i, LOGO_COL_LAST, i, on);
      }
      i++;
      if (i > LOGO_ROW_LAST) {
        i = 0;
        step++;
      }
    } else if (step == 2) {
      if (!fill) {
        clear_vram_logo();
      }
      logo_draw_line(LOGO_COL_LAST - i, LOGO_ROW_FIRST, LOGO_COL_LAST - i,
                     LOGO_ROW_LAST, on);
      i++;
      if (i >= 15) {
        i = 0;
        step++;
      }
    } else if (step == 3) {
      if (fill) {
        logo_draw_line(LOGO_COL_FIRST, LOGO_ROW_LAST - i, LOGO_COL_LAST,
                       LOGO_ROW_LAST - i, off);
      } else {
        clear_vram_logo();
        logo_draw_line(LOGO_COL_FIRST, LOGO_ROW_LAST - i, LOGO_COL_LAST,
                       LOGO_ROW_LAST - i, on);
      }
      i++;
      if (i > LOGO_ROW_LAST) {
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
        logo_draw_line(i, LOGO_ROW_FIRST, LOGO_COL_LAST - i, LOGO_ROW_LAST, on);
      } else {
        logo_draw_line(i, LOGO_ROW_FIRST, LOGO_COL_LAST - i, LOGO_ROW_LAST,
                       off);
      }
    } else {
      clear_vram_logo();
      logo_draw_line(i, LOGO_ROW_FIRST, LOGO_COL_LAST - i, LOGO_ROW_LAST, on);
    }
    i++;
    if (i >= 15) {
      i = 0;
      quarter++;
    }
  } else {
    if (fill) {
      if (quarter == 1) {
        logo_draw_line(0, LOGO_ROW_LAST - i, LOGO_COL_LAST, i, on);
      } else {
        logo_draw_line(0, LOGO_ROW_LAST - i, LOGO_COL_LAST, i, off);
      }
    } else {
      clear_vram_logo();
      logo_draw_line(0, LOGO_ROW_LAST - i, LOGO_COL_LAST, i, on);
    }
    i++;
    if (i > LOGO_ROW_LAST) {
      i = 1;
      quarter++;
    }
  }
  if (quarter >= 4) {
    quarter = 0;
  }
}

// CIRCLE POSITIONS
const uint8_t circleXPos[] PROGMEM = {6, 7, 9, 2, 12};
const uint8_t circleYPos[] PROGMEM = {2, 12, 7, 11, 14};

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

// DEFINED LINE POSITIONS
uint8_t logoLines[15] = {4, 13, 6, 16, 2, 15, 7, 11, 15, 8, 10, 1, 17, 9, 18};

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
    j = LOGO_ROW_LAST;
    i = !i;
  }

  for (int x = LOGO_COL_FIRST; x <= LOGO_COL_LAST; x++) {
    for (int y = LOGO_ROW_FIRST; y <= LOGO_ROW_LAST; y++) {
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

// BRIGHTNESS VALUES FOR WAVES
const uint8_t b1Array[] PROGMEM = {
    5,  5,  7,  10, 13, 18, 23, 28, 34, 39, 44, 49, 53, 57, 59, 61, 63, 64,
    63, 61, 59, 57, 53, 49, 44, 39, 34, 28, 23, 18, 13, 10, 7,  5,  5};
const uint8_t b2Array[] PROGMEM = {1,   2,  4,  8,  16, 24, 32, 64, 128,
                                   255, 64, 32, 24, 16, 12, 8,  4,  2};

void animation_wave(uint8_t speed) {
  if (counter0 < 1) {
    counter0++;
  } else {
    counter0 = 0;
    i = j;
    if (speed == 0) {  // WAVE
      for (uint8_t k = LOGO_ROW_FIRST; k <= LOGO_ROW_LAST; k++) {
        logo_draw_line(LOGO_COL_FIRST, k, LOGO_COL_LAST, k,
                       pgm_read_byte(&b1Array[i]) + 10);
        i++;
        if (i > 35) i = 0;
      }
      j++;
      if (j > 35) j = 0;
    } else {  // FAST WAVE
      for (int8_t k = LOGO_ROW_LAST; k >= LOGO_ROW_FIRST; k--) {
        logo_draw_line(LOGO_COL_FIRST, k, LOGO_COL_LAST, k,
                       pgm_read_byte(&b2Array[i]));
        i++;
        if (i > 17) i = 0;
      }
      j++;
      if (j > 17) j = 0;
    }
  }
}

// PREDEFINED STAR COORDINATES
const uint8_t sparkY[] PROGMEM = {12, 3,  8,  11, 2, 7,  12, 14, 14, 1,
                                  7,  11, 15, 11, 8, 13, 12, 6,  18, 7};
const uint8_t sparkX[] PROGMEM = {8, 7, 6, 2, 8, 8, 1,  12, 5, 5,
                                  6, 6, 4, 9, 9, 8, 11, 8,  3, 4};
const uint8_t bright1[] PROGMEM = {255, 230, 210, 200, 190, 180, 165,
                                   150, 140, 128, 90,  64,  50,  42,
                                   32,  24,  16,  11,  8,   4,   2};
const uint8_t bright2[] PROGMEM = {255, 200, 150, 128, 115, 100, 95,
                                   90,  75,  60,  45,  30,  16,  12,
                                   8,   6,   4,   2,   1,   0,   0};

// CREATES PSEUDO-RANDOMLY APPEARING STARS ON THE LOGO AREA
void animation_stars() {
  uint8_t b1 = 0;
  uint8_t b2 = 0;
  x = pgm_read_byte(&sparkX[step]);
  y = pgm_read_byte(&sparkY[step]);

  if (counter0 < 1) {
    counter0++;
  } else {
    counter0 = 0;
    if (i == 0) {
      clear_vram_logo();  // CLEAN CANVAS
      pseudo_random = global_counter % 60;
      if (pseudo_random < 20) {
        _delay_ms(750);
      } else if (pseudo_random >= 20 && pseudo_random < 40) {
        _delay_ms(1000);
      } else {
        _delay_ms(1250);
      }
    }
    b1 = pgm_read_byte(&bright1[i]);
    b2 = pgm_read_byte(&bright2[i]);
    logo_set_xy(x, y, b1);
    logo_set_xy(x - 1, y, b2);
    logo_set_xy(x + 1, y, b2);
    logo_set_xy(x, y + 1, b2);
    logo_set_xy(x, y - 1, b2);
    // _delay_ms(50);
    i++;           // BRIGHTNESS
    if (i > 20) {  // LEN BRIGHTNESSES
      i = 0;
      // ADD SOME RANDOMNESS FOR STEP FORWARD BASED ON STEPCOUNTER
      pseudo_random = global_counter % 60;
      if (pseudo_random < 20) {
        step = step + 1;
      } else if (pseudo_random >= 20 && pseudo_random < 40) {
        step = step + 2;
      } else {
        step = step + 3;
      }
      if (step > 19) {  // LEN SPARKS
        step = step - 19;
      }
      // PICK NEW STAR POSITION
      x = pgm_read_byte(&sparkX[step]);
      y = pgm_read_byte(&sparkY[step]);
    }
  }
}

// CREATES STRIPES BEEING MOVED TOWARDS EACH OTHER
void animation_stripes(uint8_t length) {
  uint8_t off = display_state.animation_brightness_off;
  uint8_t onUse = display_state.animation_brightness_on;
  int8_t stripe_length = length;
  int8_t stripe_start = -stripe_length;
  int8_t x_start = 0;
  int8_t x_end = 0;
  uint8_t overflow = length;

  if (counter0 < 1) {
    counter0++;
  } else {
    counter0 = 0;
    for (uint8_t k = LOGO_ROW_FIRST; k <= LOGO_ROW_LAST;
         k++) {  // ITERATE OVER ROWS
      // CLEAR LINE
      logo_draw_line(LOGO_COL_FIRST, k, LOGO_COL_LAST, k, off);
      if (k % 2 == 0) {  // ODD LINES
        onUse = display_state.animation_brightness_on;
        x_start = i + stripe_start;
        x_end = i + stripe_start + stripe_length;
      } else {  // EVEN LINES
        onUse = 8;
        x_start = LOGO_COL_LAST - i;
        x_end = LOGO_COL_LAST - i + stripe_length;
      }
      if (x_start < LOGO_COL_FIRST) {
        x_start = LOGO_COL_FIRST;
      }
      if (x_start > LOGO_COL_LAST) {
        x_start = LOGO_COL_LAST;
      }
      if (x_end > LOGO_COL_LAST) {
        x_end = LOGO_COL_LAST;
      }
      if (x_end < LOGO_COL_FIRST) {
        x_end = LOGO_COL_FIRST;
      }
      // DRAW LINE
      if (i > LOGO_COL_FIRST - stripe_length + 1 &&
          i < LOGO_COL_LAST + stripe_length - 1) {  // ONLY DRAW LINES VALID
        logo_draw_line(x_start, k, x_end, k, onUse);
      }
    }
    if (j == 0) {
      i++;  // X-ITERATOR FROM LEFT TO RIGHT
      if (i >= LOGO_COL_LAST + stripe_length + overflow) {
        clear_vram_logo();  // CLEAN CANVAS
        pseudo_random = global_counter % 60;
        if (pseudo_random < 20) {
          _delay_ms(10);
        } else if (pseudo_random >= 20 && pseudo_random < 40) {
          _delay_ms(500);
        } else {
          _delay_ms(2000);
        }
        j = 1;
      }
    } else {
      i--;  // X-ITERATOR FROM RIGHT TO LEFT
      if (i <= LOGO_COL_FIRST - stripe_length - overflow) {
        clear_vram_logo();  // CLEAN CANVAS
        pseudo_random = global_counter % 60;
        if (pseudo_random < 20) {
          _delay_ms(10);
        } else if (pseudo_random >= 20 && pseudo_random < 40) {
          _delay_ms(500);
        } else {
          _delay_ms(2000);
        }
        i = 0;
        j = 0;
      }
    }
  }
}
