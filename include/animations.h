#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <stdint.h>

// DEFINES KNOWN ANIMATION-IDs
enum ANIMATIONS {
  LOGO_OFF = 0,
  LOGO_ON = 1,
  MATRIX = 2,
  SWEEP = 3,
  WAVE = 4,
  LINES = 5,
  LINES_FILLED = 6,
  ROTATE = 7,
  ROTATE_FILLED = 8,
  CIRCLES = 9,
  SPARKLE = 10,
  STRIPES = 11,
  INVALID_ANIMATION = 12,  // Needed to identify last animation value
};

void animations_reset();

void animation_line(uint8_t fill);
void animation_rotate(uint8_t fill);
void animation_matrix();
void animation_sweep();
void animation_wave();
void animation_circles();
void animation_stripes(uint8_t length);
void animation_sparkle();

#endif
