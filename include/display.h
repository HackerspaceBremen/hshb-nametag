#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

// HARD CODED FIRMWARE VERSION INFO
#define FIRMWARE_VERSION 1.0

// SEVERAL CONSTANTS WHICH EASE UNDERSTANDING THE CODE
#define TEXT_START 0    // FIRST ADRESSABLE LED
#define TEXT_END 419    // 7 ROWS x 60 LEDs (420 LEDs)
#define LOGO_START 420  // 102 LEDs DISTRIBUTED ON 19 ROWS
#define LOGO_END 522    // NUMBER OF OVERALL ADRESSABLE LEDs
#define LOGO_ROW_FIRST 0
#define LOGO_ROW_LAST 18  // 0..18 (19 ROWS)
#define LOGO_COL_FIRST 0
#define LOGO_COL_LAST 14  // 0..14 (15 COLUMNS)

#define SYSMSG_BUFFER_SIZE 15

struct DisplayState {
  uint8_t display_on : 1;
  uint8_t scroll_interval;
  char sysmsg_buffer[SYSMSG_BUFFER_SIZE];

  uint8_t animation_brightness_on;
  uint8_t animation_brightness_off;

  uint8_t text_brightness_on;
  uint8_t text_brightness_off;
};

extern volatile struct DisplayState display_state;

extern volatile uint8_t vRAM[522];

extern char text[100];

void display_write_text(uint16_t x, char t[], uint8_t s, uint8_t c,
                        uint8_t onBright, uint8_t offBright);
void display_write_scroll(char t[], uint8_t s, uint8_t c, uint8_t onBright,
                          uint8_t offBright);

uint8_t display_scroll();
uint8_t display_scrolling();
void display_scroll_stop();

void display_fade_text(const char* text, uint8_t fade_out);

void clear_vram();
void clear_vram_text();
void clear_vram_logo();

#endif
