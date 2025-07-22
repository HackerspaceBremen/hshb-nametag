#ifndef LOGO_H_
#define LOGO_H_

#include <stdint.h>

void logo_draw_line(int8_t x1, int8_t y1, int8_t x2, int8_t y2,
                    uint8_t brightness);
void logo_draw_circle(int8_t x, int8_t y, int8_t r, uint8_t brightness);
void logo_set_xy(uint8_t x, uint8_t y, uint8_t brightness);
void logo_draw_xy(uint8_t x, uint8_t y, uint8_t brightness);
void logo_fill_percent(int8_t perc, uint8_t full, uint8_t onB, uint8_t offB);

#endif
