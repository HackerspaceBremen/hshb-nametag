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

void resetAnimations();

uint8_t linesAnimation(uint8_t fill);
uint8_t rotateAnimation(uint8_t fill);
uint8_t circlesAnimation();
void matrixAnimation();
void sweepAnimation();
void waveAnimation();

#endif
