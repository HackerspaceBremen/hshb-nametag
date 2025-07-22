#include "logo.h"

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "display.h"

void logo_fill_percent(int8_t perc, uint8_t full, uint8_t onB, uint8_t offB) {
  uint8_t leds = (perc * full + 50) / 100;
  if (perc < 0) {
    leds = 0;
  } else if (perc > 100) {
    leds = full;
  }
  for (uint8_t i = 0; i <= LOGO_ROW_LAST; i++) {
    if (i < leds) {
      logo_draw_line(0, LOGO_ROW_LAST - i, LOGO_COL_LAST, LOGO_ROW_LAST - i,
                     onB);
    } else {
      logo_draw_line(0, LOGO_ROW_LAST - i, LOGO_COL_LAST, LOGO_ROW_LAST - i,
                     offB);
    }
  }
}

// DRAWS A LINE FROM POINT AT P(X1,Y1) TO P(X2,Y2) WITH BRIGHTNESS
void logo_draw_line(int8_t x1, int8_t y1, int8_t x2, int8_t y2,
                    uint8_t brightness) {
  int8_t dx = x2 - x1;
  int8_t dy = y2 - y1;
  if (dx < 0) dx *= -1;
  if (dy < 0) dy *= -1;
  int8_t sx = x1 < x2 ? 1 : -1;
  int8_t sy = y1 < y2 ? 1 : -1;
  int8_t err = dx - dy;
  while (x1 != x2 || y1 != y2) {
    logo_set_xy(x1, y1, brightness);
    int8_t e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
  logo_set_xy(x2, y2, brightness);
}

void logo_draw_circle(int8_t x, int8_t y, int8_t r, uint8_t brightness) {
  int8_t x1 = 0;
  int8_t y1 = r;
  int8_t d = 3 - 2 * r;
  while (x1 <= y1) {
    logo_set_xy(x + x1, y + y1, brightness);
    logo_set_xy(x + x1, y - y1, brightness);
    logo_set_xy(x - x1, y + y1, brightness);
    logo_set_xy(x - x1, y - y1, brightness);
    logo_set_xy(x + y1, y + x1, brightness);
    logo_set_xy(x + y1, y - x1, brightness);
    logo_set_xy(x - y1, y + x1, brightness);
    logo_set_xy(x - y1, y - x1, brightness);
    x1++;
    if (d < 0) {
      d += 4 * x1 + 6;
    } else {
      y1--;
      d += 4 * (x1 - y1) + 10;
    }
  }
}

// STARTS AT 420 .. 521 TO MAP X/Y-COORD TO PIXEL ADRESS
const uint16_t pixel[] PROGMEM = {
    5,   19,  20,  23,  24,  35,  36,  37,  28,  39,  40,  51,  52,  53,  64,
    67,  71,  80,  81,  82,  83,  84,  85,  96,  98,  99,  108, 109, 110, 111,
    112, 113, 114, 116, 124, 125, 126, 127, 128, 129, 130, 136, 141, 144, 151,
    152, 156, 159, 166, 167, 168, 170, 171, 172, 173, 174, 175, 176, 180, 181,
    182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 195, 196, 199, 200,
    201, 202, 203, 204, 205, 206, 207, 208, 214, 215, 216, 217, 218, 221, 222,
    223, 229, 230, 237, 239, 244, 252, 258, 267, 272, 273, 281};

// TRANSLATES X/Y-COORD TO PIXEL ADRESS ON LOGO
void logo_set_xy(uint8_t x, uint8_t y, uint8_t brightness) {
  if (x > LOGO_COL_LAST || y > LOGO_ROW_LAST) return;
  uint16_t pix = x + y * (LOGO_COL_LAST + 1);
  uint16_t current_pix = 0;
  uint16_t adress = LOGO_START;
  for (uint8_t i = 0; i < 101; i++) {
    current_pix = pgm_read_word(&pixel[i]);
    if (current_pix == pix) {
      adress = adress + i;
      vRAM[adress] = brightness;
    }
  }
}
