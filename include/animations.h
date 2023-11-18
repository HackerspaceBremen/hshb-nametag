#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <stdint.h>

enum ANIMATIONS {
  LOGO_OFF,
  LOGO_ON,
  MATRIX,
  SWEEP,
  WAVE,
  LINES,
  LINES_FILLED,
  ROTATE,
  ROTATE_FILLED,
  CIRCLES,

  // Needed to identify last animation value
  INVALID_ANIMATION,
};

void animations_reset();

void animation_line(uint8_t fill);
void animation_rotate(uint8_t fill);
void animation_circles();
void animation_matrix();
void animation_sweep();
void animation_wave();

#endif
