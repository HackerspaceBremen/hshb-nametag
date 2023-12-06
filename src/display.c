#include "display.h"

#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "animations.h"

char text[100];

uint8_t scaler = 1;
uint8_t charWidth = 7;

uint8_t textLength = 0;
uint16_t textWidth = 0;
volatile uint16_t pos = 0;
uint8_t dispOnBright = 64;
uint8_t dispOffBright = 0;
volatile uint8_t line = 0;
volatile uint8_t scaleCounter = 0;
volatile uint8_t scrolling = 0;

volatile struct DisplayState display_state = {
    .display_on = 1,
    .scroll_interval = 0,
    .sysmsg_buffer = "",
    .animation_brightness_on = 32,
    .animation_brightness_off = 0,
    .text_brightness_on = 64,
    .text_brightness_off = 0,
};

volatile uint8_t vRAM[522];

// clang-format off
const uint8_t charset[][5] PROGMEM = {
    {	 // Space
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000
    }, { // !
        0b00000000,
        0b00000000,
        0b01001111,
        0b00000000,
        0b00000000
    }, { // "
        0b00000000,
        0b00000111,
        0b00000000,
        0b00000111,
        0b00000000
    }, { // #
        0b00010100,
        0b01111111,
        0b00010100,
        0b01111111,
        0b00010100
    }, { // $
        0b00100100,
        0b00101010,
        0b01111111,
        0b00101010,
        0b00010010
    }, { // %
        0b00100011,
        0b00010011,
        0b00001000,
        0b01100100,
        0b01100010
    }, { // &
        0b00110110,
        0b01001001,
        0b01010101,
        0b00100010,
        0b01010000
    }, { // '
        0b00000000,
        0b00000101,
        0b00000011,
        0b00000000,
        0b00000000
    }, { // (
        0b00000000,
        0b00011100,
        0b00100010,
        0b01000001,
        0b00000000
    }, { // )
        0b00000000,
        0b01000001,
        0b00100010,
        0b00011100,
        0b00000000
    }, { // *
        0b00010100,
        0b00001000,
        0b00111110,
        0b00001000,
        0b00010100
    }, { // +
        0b00001000,
        0b00001000,
        0b00111110,
        0b00001000,
        0b00001000
    }, { // ,
        0b00000000,
        0b01010000,
        0b00110000,
        0b00000000,
        0b00000000
    }, { // -
        0b00001000,
        0b00001000,
        0b00001000,
        0b00001000,
        0b00001000
    }, { // .
        0b01100000,
        0b01100000,
        0b00000000,
        0b00000000,
        0b00000000
    }, { // /
        0b00100000,
        0b00010000,
        0b00001000,
        0b00000100,
        0b00000010
    }, { // 0
        0b00111110,
        0b01000101,
        0b01001001,
        0b01010001,
        0b00111110
    }, { // 1
        0b00000000,
        0b01000010,
        0b01111111,
        0b01000000,
        0b00000000
    }, { // 2
        0b01000010,
        0b01100001,
        0b01010001,
        0b01001001,
        0b01000110
    }, { // 3
        0b00100001,
        0b01000001,
        0b01000101,
        0b01001011,
        0b00110001
    }, { // 4
        0b00011000,
        0b00010100,
        0b00010010,
        0b01111111,
        0b00010000
    }, { // 5
        0b00100111,
        0b01000101,
        0b01000101,
        0b01000101,
        0b00111001
    }, { // 6
        0b00111100,
        0b01001010,
        0b01001001,
        0b01001001,
        0b00110000
    }, { // 7
        0b00000001,
        0b01110001,
        0b00001001,
        0b00000101,
        0b00000011
    }, { // 8
        0b00110110,
        0b01001001,
        0b01001001,
        0b01001001,
        0b00110110
    }, { // 9
        0b00000110,
        0b01001001,
        0b01001001,
        0b00101001,
        0b00011110
    }, { // :
        0b00000000,
        0b00110110,
        0b00110110,
        0b00000000,
        0b00000000
    }, { // ;
        0b00000000,
        0b01010110,
        0b00110110,
        0b00000000,
        0b00000000
    }, { // <
        0b00001000,
        0b00010100,
        0b00100010,
        0b01000001,
        0b00000000
    }, { // =
        0b00010100,
        0b00010100,
        0b00010100,
        0b00010100,
        0b00010100
    }, { // >
        0b00000000,
        0b01000001,
        0b00100010,
        0b00010100,
        0b00001000
    }, { // ?
        0b00000010,
        0b00000001,
        0b01010001,
        0b00001001,
        0b00000110
    }, { // @
        0b00110010,
        0b01001001,
        0b01111001,
        0b01000001,
        0b00111110
    }, { // A
        0b01111110,
        0b00010001,
        0b00010001,
        0b00010001,
        0b01111110
    }, { // B
        0b01111111,
        0b01001001,
        0b01001001,
        0b01001001,
        0b00110110
    }, { // C
        0b00111110,
        0b01000001,
        0b01000001,
        0b01000001,
        0b00100010
    }, { // D
        0b01111111,
        0b01000001,
        0b01000001,
        0b00100010,
        0b00011100
    }, { // E
        0b01111111,
        0b01001001,
        0b01001001,
        0b01001001,
        0b01000001
    }, { // F
        0b01111111,
        0b00001001,
        0b00001001,
        0b00001001,
        0b00000001
    }, { // G
        0b00111110,
        0b01000001,
        0b01001001,
        0b01001001,
        0b00111010
    }, { // H
        0b01111111,
        0b00001000,
        0b00001000,
        0b00001000,
        0b01111111
    }, { // I
        0b00000000,
        0b01000001,
        0b01111111,
        0b01000001,
        0b00000000
    }, { // J
        0b00100000,
        0b01000000,
        0b01000001,
        0b00111111,
        0b00000001
    }, { // K
        0b01111111,
        0b00001000,
        0b00010100,
        0b00100010,
        0b01000001
    }, { // L
        0b01111111,
        0b01000000,
        0b01000000,
        0b01000000,
        0b01000000
    }, { // M
        0b01111111,
        0b00000010,
        0b00001100,
        0b00000010,
        0b01111111
    }, { // N
        0b01111111,
        0b00000100,
        0b00001000,
        0b00010000,
        0b01111111
    }, { // O
        0b00111110,
        0b01000001,
        0b01000001,
        0b01000001,
        0b00111110
    }, { // P
        0b01111111,
        0b00001001,
        0b00001001,
        0b00001001,
        0b00000110
    }, { // Q
        0b00111110,
        0b01000001,
        0b01010001,
        0b00100001,
        0b01011110
    }, { // R
        0b01111111,
        0b00001001,
        0b00011001,
        0b00101001,
        0b01000110
    }, { // S
        0b01000110,
        0b01001001,
        0b01001001,
        0b01001001,
        0b00110001
    }, { // T
        0b00000001,
        0b00000001,
        0b01111111,
        0b00000001,
        0b00000001
    }, { // U
        0b00111111,
        0b01000000,
        0b01000000,
        0b01000000,
        0b00111111
    }, { // V
        0b00011111,
        0b00100000,
        0b01000000,
        0b00100000,
        0b00011111
    }, { // W
        0b00111111,
        0b01000000,
        0b00111000,
        0b01000000,
        0b00111111
    }, { // X
        0b01100011,
        0b00010100,
        0b00001000,
        0b00010100,
        0b01100011
    }, { // Y
        0b00000111,
        0b00001000,
        0b01110000,
        0b00001000,
        0b00000111
    }, { // Z
        0b01100001,
        0b01010001,
        0b01001001,
        0b01000101,
        0b01000011
    }, { // [
        0b00000000,
        0b01111111,
        0b01000001,
        0b01000001,
        0b00000000
    }, { // backslash
        0b00000010,
        0b00000100,
        0b00001000,
        0b00010000,
        0b00100000
    }, { // ]
        0b00000000,
        0b01000001,
        0b01000001,
        0b01111111,
        0b00000000
    }, { // ^
        0b00000100,
        0b00000010,
        0b00000001,
        0b00000010,
        0b00000100
    }, { // _
        0b01000000,
        0b01000000,
        0b01000000,
        0b01000000,
        0b01000000
    }, { // `
        0b00000000,
        0b00000001,
        0b00000010,
        0b00000100,
        0b00000000
    }, { // a
        0b00100000,
        0b01010100,
        0b01010100,
        0b01010100,
        0b01111000
    }, { // b
        0b01111111,
        0b01001000,
        0b01000100,
        0b01000100,
        0b00111000
    }, { // c
        0b00111000,
        0b01000100,
        0b01000100,
        0b01000100,
        0b00100000
    }, { // d
        0b00111000,
        0b01000100,
        0b01000100,
        0b01001000,
        0b01111111
    }, { // e
        0b00111000,
        0b01010100,
        0b01010100,
        0b01010100,
        0b00011000
    }, { // f
        0b00001000,
        0b01111110,
        0b00001001,
        0b00000001,
        0b00000010
    }, { // g
        0b00001100,
        0b01010010,
        0b01010010,
        0b01010010,
        0b00111110
    }, { // h
        0b01111111,
        0b00001000,
        0b00000100,
        0b00000100,
        0b01111000
    }, { // i
        0b00000000,
        0b01000100,
        0b01111101,
        0b01000000,
        0b00000000
    }, { // j
        0b00100000,
        0b01000000,
        0b01000100,
        0b00111101,
        0b00000000
    }, { // k
        0b01111111,
        0b00010000,
        0b00101000,
        0b01000100,
        0b00000000
    }, { // l
        0b00000000,
        0b01000001,
        0b01111111,
        0b01000000,
        0b00000000
    }, { // m
        0b01111100,
        0b00000100,
        0b00011000,
        0b00000100,
        0b01111000
    }, { // n
        0b01111100,
        0b00001000,
        0b00000100,
        0b00000100,
        0b01111000
    }, { // o
        0b00111000,
        0b01000100,
        0b01000100,
        0b01000100,
        0b00111000
    }, { // p
        0b01111100,
        0b00010100,
        0b00010100,
        0b00010100,
        0b00001000
    }, { // q
        0b00001000,
        0b00010100,
        0b00010100,
        0b00011000,
        0b01111100
    }, { // r
        0b01111100,
        0b00001000,
        0b00000100,
        0b00000100,
        0b00001000
    }, { // s
        0b01001000,
        0b01010100,
        0b01010100,
        0b01010100,
        0b00100000
    }, { // t
        0b00000100,
        0b00111111,
        0b01000100,
        0b01000000,
        0b00100000
    }, { // u
        0b00111100,
        0b01000000,
        0b01000000,
        0b00100000,
        0b01111100
    }, { // v
        0b00011100,
        0b00100000,
        0b01000000,
        0b00100000,
        0b00011100
    }, { // w
        0b00111100,
        0b01000000,
        0b00110000,
        0b01000000,
        0b00111100
    }, { // x
        0b01000100,
        0b00101000,
        0b00010000,
        0b00101000,
        0b01000100
    }, { // y
        0b00001100,
        0b01010000,
        0b01010000,
        0b01010000,
        0b00111100
    }, { // z
        0b01000100,
        0b01100100,
        0b01010100,
        0b01001100,
        0b01000100
    }, { // {
        0b00000000,
        0b00001000,
        0b00110110,
        0b01000001,
        0b00000000
    }, { // |
        0b00000000,
        0b00000000,
        0b01111111,
        0b00000000,
        0b00000000
    }, { // }
        0b00000000,
        0b01000001,
        0b00110110,
        0b00001000,
        0b00000000
    }, { // ~
        0b00001000,
        0b00000100,
        0b00001000,
        0b00001000,
        0b00000100
    }
};

const uint8_t specialCharset[][5] PROGMEM = {
    {	 // 196 - Ä
        0b01111000,
        0b00010101,
        0b00010100,
        0b00010101,
        0b01111000
    }, { // 209 - Ñ
        0b01111100,
        0b00001010,
        0b00010001,
        0b00100010,
        0b01111101
    }, { // 214 - Ö
        0b00111000,
        0b01000101,
        0b01000100,
        0b01000101,
        0b00111000
    }, { // 220 - Ü
        0b00111100,
        0b01000001,
        0b01000000,
        0b01000001,
        0b00111100
    }, { // 223 - ß
        0b01111110,
        0b00100101,
        0b00100101,
        0b00100101,
        0b00011010
    }, { // 228 - ä
        0b00100000,
        0b01010101,
        0b01010100,
        0b01010101,
        0b01111000
    }, { // 241 - ñ
        0b01111010,
        0b00010001,
        0b00001001,
        0b00001010,
        0b01110001
    }, { // 246 - ö
        0b00110000,
        0b01001010,
        0b01001000,
        0b01001010,
        0b00110000
    }, { // 252 - ü
        0b00111000,
        0b01000010,
        0b01000000,
        0b00100010,
        0b01111000
    }
};
// clang-format on

static uint8_t specialChar(uint8_t c, uint8_t l) {
  switch (c) {
    case 196:
      return pgm_read_byte(&specialCharset[0][l]);
    case 132:
      return pgm_read_byte(&specialCharset[0][l]);
    case 209:
      return pgm_read_byte(&specialCharset[1][l]);
    case 145:
      return pgm_read_byte(&specialCharset[1][l]);
    case 214:
      return pgm_read_byte(&specialCharset[2][l]);
    case 150:
      return pgm_read_byte(&specialCharset[2][l]);
    case 220:
      return pgm_read_byte(&specialCharset[3][l]);
    case 156:
      return pgm_read_byte(&specialCharset[3][l]);
    case 223:
      return pgm_read_byte(&specialCharset[4][l]);
    case 159:
      return pgm_read_byte(&specialCharset[4][l]);
    case 228:
      return pgm_read_byte(&specialCharset[5][l]);
    case 164:
      return pgm_read_byte(&specialCharset[5][l]);
    case 241:
      return pgm_read_byte(&specialCharset[6][l]);
    case 177:
      return pgm_read_byte(&specialCharset[6][l]);
    case 246:
      return pgm_read_byte(&specialCharset[7][l]);
    case 182:
      return pgm_read_byte(&specialCharset[7][l]);
    case 252:
      return pgm_read_byte(&specialCharset[8][l]);
    case 188:
      return pgm_read_byte(&specialCharset[8][l]);
    default:
      return 0b00000000;
  }
}

void display_write_text(uint16_t x, char t[], uint8_t s, uint8_t c,
                        uint8_t onBright, uint8_t offBright) {
  scrolling = 0;
  x *= 7;
  if (x >= 420) return;
  for (uint16_t i = 0; i < x; i++) vRAM[i] = offBright;
  for (uint8_t i = 0; i < strlen(t); i++) {
    for (uint8_t j = 0; j < 5; j++) {
      if (t[i] & 1 << 7) {
        line = specialChar(t[i], j);
      } else {
        line = pgm_read_byte(&charset[t[i] - 32][j]);
      }

      for (uint8_t k = 0; k < 7; k++) {
        if (line & (1 << k)) {
          vRAM[x] = onBright;
        } else {
          vRAM[x] = offBright;
        }
        x++;
        if (x >= 420) return;
      }
      for (uint16_t l = x + (s - 1) * 7; x < l && x < 420; x++) {
        vRAM[x] = offBright;
      }
    }
    for (uint16_t j = x + c * s * 7; x < j && x < 420; x++) {
      vRAM[x] = offBright;
    }
    if (x >= TEXT_END) return;
  }
  for (; x < TEXT_END; x++) {
    vRAM[x] = offBright;
  }
}

void display_write_scroll(char t[], uint8_t s, uint8_t c, uint8_t onBright,
                          uint8_t offBright) {
  textLength = strlen(t);
  memcpy(text, t, textLength);
  scaler = s;
  charWidth = c + 5;
  dispOnBright = onBright;
  dispOffBright = offBright;
  textWidth = textLength * charWidth;
  pos = 0;
  scaleCounter = 0;
  scrolling = 1;
}

uint8_t display_scrolling() { return scrolling; }

void display_scroll_stop() { scrolling = 0; }

uint8_t display_scroll() {
  if (!scrolling) {
    return 0;
  }

  for (uint8_t i = 0; i < 59; i++)
    memcpy((uint8_t*)vRAM + i * 7, (uint8_t*)vRAM + i * 7 + 7, 7);

  uint8_t curChar = pos % charWidth;
  if (!scaleCounter && pos < textWidth && curChar < 5) {
    if (text[pos / charWidth] & 1 << 7) {
      line = specialChar(text[pos / charWidth], curChar);
    } else {
      line = pgm_read_byte(&charset[text[pos / charWidth] - 32][curChar]);
    }
    for (uint8_t i = 0; i < 7; i++) {
      if (line & (1 << i)) {
        vRAM[413 + i] = dispOnBright;
      } else {
        vRAM[413 + i] = dispOffBright;
      }
    }
  } else {
    for (uint16_t i = 413; i < 420; i++) vRAM[i] = dispOffBright;
  }

  scaleCounter++;
  if (scaleCounter >= scaler) {
    scaleCounter = 0;
    pos++;
    if (pos >= textWidth + (60 / scaler)) {
      scrolling = 0;
      return 0;
    }
  }
  return 1;
}

void display_fade_text(const char* text, uint8_t fade_out) {
  if (text[0])
    snprintf((char*)display_state.sysmsg_buffer, SYSMSG_BUFFER_SIZE, text);
  clear_vram();
  display_state.display_on = 1;
  animations_reset();

  if (fade_out) {
    for (uint8_t i = 64; i > 0; i--) {
      display_write_text(3, (char*)display_state.sysmsg_buffer, 1, 2, i, 0);
      _delay_ms(20);
    }
  } else {
    for (uint8_t i = 0; i <= 64; i++) {
      display_write_text(3, (char*)display_state.sysmsg_buffer, 1, 0, i, 0);
      _delay_ms(20);
    }
  }
}

void clear_vram() {
  clear_vram_text();
  clear_vram_logo();
}

void clear_vram_text() {
  for (uint16_t k = TEXT_START; k < TEXT_END; k++) vRAM[k] = 0;
}

void clear_vram_logo() {
  for (uint16_t k = LOGO_START; k < LOGO_END; k++) vRAM[k] = 0;
}
