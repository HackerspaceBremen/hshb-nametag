#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

extern char text[100];

void writeText(uint16_t x, char t[], uint8_t s, uint8_t c, uint8_t onBright,
               uint8_t offBright);
void writeScroll(char t[], uint8_t s, uint8_t c, uint8_t onBright,
                 uint8_t offBright);
uint8_t scrollDisplay();
uint8_t scrollingDisplay();
void scrollStop();

#endif  // DISPLAY_H_
