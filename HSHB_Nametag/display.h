#ifndef DISPLAY_H_
#define DISPLAY_H_

char text[100];

/*uint8_t scaler;
uint8_t charWidth;

uint8_t textLength;
uint16_t textWidth;
uint16_t pos;
uint8_t lineVal;
uint16_t offset;
uint8_t dispOnBright;
uint8_t dispOffBright;
uint8_t line;
uint8_t scaleCounter;
uint8_t scrolling;*/

void writeText(uint16_t x, char t[], uint8_t s, uint8_t c, uint8_t onBright, uint8_t offBright);
void writeScroll(char t[], uint8_t s, uint8_t c, uint8_t onBright, uint8_t offBright);
uint8_t scrollDisplay();
uint8_t scrollingDisplay();
void scrollStop();

//uint8_t charset[95][5];

#endif // DISPLAY_H_