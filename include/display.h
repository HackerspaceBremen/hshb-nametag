#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

#define TEXT_START 0
#define TEXT_END 420
#define LOGO_START 420
#define LOGO_END 522

struct DisplaySettings {
  uint8_t animationOnBrightness;
  uint8_t animationOffBrightness;

  uint8_t textOnBrightness;
  uint8_t textOffBrightness;
};

extern struct DisplaySettings display_settings;

extern char text[100];

void writeText(uint16_t x, char t[], uint8_t s, uint8_t c, uint8_t onBright,
               uint8_t offBright);
void writeScroll(char t[], uint8_t s, uint8_t c, uint8_t onBright,
                 uint8_t offBright);
uint8_t scrollDisplay();
uint8_t scrollingDisplay();
void scrollStop();

void clearVRAM();

#endif
