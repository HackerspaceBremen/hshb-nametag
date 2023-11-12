#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <stdint.h>

void resetAnimations(uint8_t clearScreen);

uint8_t linesAnimation(uint8_t fill, uint8_t on, uint8_t off);
uint8_t rotateAnimation(uint8_t fill, uint8_t on, uint8_t off);
uint8_t circlesAnimation(uint8_t on, uint8_t off);
void matrixAnimation();
void sweepAnimation(uint8_t on, uint8_t off);
void waveAnimation();

#endif  // ANIMATIONS_H_
