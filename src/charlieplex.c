#include "charlieplex.h"

#include <avr/io.h>

extern uint8_t vRAM[522];

void refreshDisplay() {
  DDRA &= 0b11110000;
  DDRB = 0;
  DDRC = 0;
  DDRD &= 0b00001111;

  uint8_t r16_save;
  asm volatile("mov %0, r16" : "=r"(r16_save) :);  // save r16
  register uint8_t tmp asm("r16");

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[0]));
  if (tmp) {
    DDRB = 0b11000000;
    PORTB |= 0b10000000;
    asm volatile(
        "delay_loop0:\n\t"
        "dec r16\n\t"
        "brne delay_loop0\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[1]));
  if (tmp) {
    DDRB = 0b11000000;
    PORTB |= 0b01000000;
    asm volatile(
        "delay_loop1:\n\t"
        "dec r16\n\t"
        "brne delay_loop1\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[2]));
  if (tmp) {
    DDRB = 0b10100000;
    PORTB |= 0b00100000;
    asm volatile(
        "delay_loop2:\n\t"
        "dec r16\n\t"
        "brne delay_loop2\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[3]));
  if (tmp) {
    DDRB = 0b10010000;
    PORTB |= 0b00010000;
    asm volatile(
        "delay_loop3:\n\t"
        "dec r16\n\t"
        "brne delay_loop3\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[4]));
  if (tmp) {
    DDRB = 0b10001000;
    PORTB |= 0b00001000;
    asm volatile(
        "delay_loop4:\n\t"
        "dec r16\n\t"
        "brne delay_loop4\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[5]));
  if (tmp) {
    DDRB = 0b10000100;
    PORTB |= 0b00000100;
    asm volatile(
        "delay_loop5:\n\t"
        "dec r16\n\t"
        "brne delay_loop5\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[6]));
  if (tmp) {
    DDRB = 0b10000010;
    PORTB |= 0b00000010;
    asm volatile(
        "delay_loop6:\n\t"
        "dec r16\n\t"
        "brne delay_loop6\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[7]));
  if (tmp) {
    DDRB = 0b10100000;
    PORTB |= 0b10000000;
    asm volatile(
        "delay_loop7:\n\t"
        "dec r16\n\t"
        "brne delay_loop7\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[8]));
  if (tmp) {
    DDRB = 0b01100000;
    PORTB |= 0b01000000;
    asm volatile(
        "delay_loop8:\n\t"
        "dec r16\n\t"
        "brne delay_loop8\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[9]));
  if (tmp) {
    DDRB = 0b01100000;
    PORTB |= 0b00100000;
    asm volatile(
        "delay_loop9:\n\t"
        "dec r16\n\t"
        "brne delay_loop9\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[10]));
  if (tmp) {
    DDRB = 0b01010000;
    PORTB |= 0b00010000;
    asm volatile(
        "delay_loop10:\n\t"
        "dec r16\n\t"
        "brne delay_loop10\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[11]));
  if (tmp) {
    DDRB = 0b01001000;
    PORTB |= 0b00001000;
    asm volatile(
        "delay_loop11:\n\t"
        "dec r16\n\t"
        "brne delay_loop11\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[12]));
  if (tmp) {
    DDRB = 0b01000100;
    PORTB |= 0b00000100;
    asm volatile(
        "delay_loop12:\n\t"
        "dec r16\n\t"
        "brne delay_loop12\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[13]));
  if (tmp) {
    DDRB = 0b01000010;
    PORTB |= 0b00000010;
    asm volatile(
        "delay_loop13:\n\t"
        "dec r16\n\t"
        "brne delay_loop13\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[14]));
  if (tmp) {
    DDRB = 0b10010000;
    PORTB |= 0b10000000;
    asm volatile(
        "delay_loop14:\n\t"
        "dec r16\n\t"
        "brne delay_loop14\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[15]));
  if (tmp) {
    DDRB = 0b01010000;
    PORTB |= 0b01000000;
    asm volatile(
        "delay_loop15:\n\t"
        "dec r16\n\t"
        "brne delay_loop15\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[16]));
  if (tmp) {
    DDRB = 0b00110000;
    PORTB |= 0b00100000;
    asm volatile(
        "delay_loop16:\n\t"
        "dec r16\n\t"
        "brne delay_loop16\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[17]));
  if (tmp) {
    DDRB = 0b00110000;
    PORTB |= 0b00010000;
    asm volatile(
        "delay_loop17:\n\t"
        "dec r16\n\t"
        "brne delay_loop17\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[18]));
  if (tmp) {
    DDRB = 0b00101000;
    PORTB |= 0b00001000;
    asm volatile(
        "delay_loop18:\n\t"
        "dec r16\n\t"
        "brne delay_loop18\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[19]));
  if (tmp) {
    DDRB = 0b00100100;
    PORTB |= 0b00000100;
    asm volatile(
        "delay_loop19:\n\t"
        "dec r16\n\t"
        "brne delay_loop19\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[20]));
  if (tmp) {
    DDRB = 0b00100010;
    PORTB |= 0b00000010;
    asm volatile(
        "delay_loop20:\n\t"
        "dec r16\n\t"
        "brne delay_loop20\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[21]));
  if (tmp) {
    DDRB = 0b10001000;
    PORTB |= 0b10000000;
    asm volatile(
        "delay_loop21:\n\t"
        "dec r16\n\t"
        "brne delay_loop21\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[22]));
  if (tmp) {
    DDRB = 0b01001000;
    PORTB |= 0b01000000;
    asm volatile(
        "delay_loop22:\n\t"
        "dec r16\n\t"
        "brne delay_loop22\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[23]));
  if (tmp) {
    DDRB = 0b00101000;
    PORTB |= 0b00100000;
    asm volatile(
        "delay_loop23:\n\t"
        "dec r16\n\t"
        "brne delay_loop23\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[24]));
  if (tmp) {
    DDRB = 0b00011000;
    PORTB |= 0b00010000;
    asm volatile(
        "delay_loop24:\n\t"
        "dec r16\n\t"
        "brne delay_loop24\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[25]));
  if (tmp) {
    DDRB = 0b00011000;
    PORTB |= 0b00001000;
    asm volatile(
        "delay_loop25:\n\t"
        "dec r16\n\t"
        "brne delay_loop25\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[26]));
  if (tmp) {
    DDRB = 0b00010100;
    PORTB |= 0b00000100;
    asm volatile(
        "delay_loop26:\n\t"
        "dec r16\n\t"
        "brne delay_loop26\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[27]));
  if (tmp) {
    DDRB = 0b00010010;
    PORTB |= 0b00000010;
    asm volatile(
        "delay_loop27:\n\t"
        "dec r16\n\t"
        "brne delay_loop27\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[28]));
  if (tmp) {
    DDRB = 0b10000100;
    PORTB |= 0b10000000;
    asm volatile(
        "delay_loop28:\n\t"
        "dec r16\n\t"
        "brne delay_loop28\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[29]));
  if (tmp) {
    DDRB = 0b01000100;
    PORTB |= 0b01000000;
    asm volatile(
        "delay_loop29:\n\t"
        "dec r16\n\t"
        "brne delay_loop29\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[30]));
  if (tmp) {
    DDRB = 0b00100100;
    PORTB |= 0b00100000;
    asm volatile(
        "delay_loop30:\n\t"
        "dec r16\n\t"
        "brne delay_loop30\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[31]));
  if (tmp) {
    DDRB = 0b00010100;
    PORTB |= 0b00010000;
    asm volatile(
        "delay_loop31:\n\t"
        "dec r16\n\t"
        "brne delay_loop31\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[32]));
  if (tmp) {
    DDRB = 0b00001100;
    PORTB |= 0b00001000;
    asm volatile(
        "delay_loop32:\n\t"
        "dec r16\n\t"
        "brne delay_loop32\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[33]));
  if (tmp) {
    DDRB = 0b00001100;
    PORTB |= 0b00000100;
    asm volatile(
        "delay_loop33:\n\t"
        "dec r16\n\t"
        "brne delay_loop33\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[34]));
  if (tmp) {
    DDRB = 0b00001010;
    PORTB |= 0b00000010;
    asm volatile(
        "delay_loop34:\n\t"
        "dec r16\n\t"
        "brne delay_loop34\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[35]));
  if (tmp) {
    DDRB = 0b10000010;
    PORTB |= 0b10000000;
    asm volatile(
        "delay_loop35:\n\t"
        "dec r16\n\t"
        "brne delay_loop35\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[36]));
  if (tmp) {
    DDRB = 0b01000010;
    PORTB |= 0b01000000;
    asm volatile(
        "delay_loop36:\n\t"
        "dec r16\n\t"
        "brne delay_loop36\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[37]));
  if (tmp) {
    DDRB = 0b00100010;
    PORTB |= 0b00100000;
    asm volatile(
        "delay_loop37:\n\t"
        "dec r16\n\t"
        "brne delay_loop37\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[38]));
  if (tmp) {
    DDRB = 0b00010010;
    PORTB |= 0b00010000;
    asm volatile(
        "delay_loop38:\n\t"
        "dec r16\n\t"
        "brne delay_loop38\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[39]));
  if (tmp) {
    DDRB = 0b00001010;
    PORTB |= 0b00001000;
    asm volatile(
        "delay_loop39:\n\t"
        "dec r16\n\t"
        "brne delay_loop39\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[40]));
  if (tmp) {
    DDRB = 0b00000110;
    PORTB |= 0b00000100;
    asm volatile(
        "delay_loop40:\n\t"
        "dec r16\n\t"
        "brne delay_loop40\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[41]));
  if (tmp) {
    DDRB = 0b00000110;
    PORTB |= 0b00000010;
    asm volatile(
        "delay_loop41:\n\t"
        "dec r16\n\t"
        "brne delay_loop41\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[42]));
  if (tmp) {
    DDRB = 0b10000001;
    PORTB |= 0b10000000;
    asm volatile(
        "delay_loop42:\n\t"
        "dec r16\n\t"
        "brne delay_loop42\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[43]));
  if (tmp) {
    DDRB = 0b01000001;
    PORTB |= 0b01000000;
    asm volatile(
        "delay_loop43:\n\t"
        "dec r16\n\t"
        "brne delay_loop43\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[44]));
  if (tmp) {
    DDRB = 0b00100001;
    PORTB |= 0b00100000;
    asm volatile(
        "delay_loop44:\n\t"
        "dec r16\n\t"
        "brne delay_loop44\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[45]));
  if (tmp) {
    DDRB = 0b00010001;
    PORTB |= 0b00010000;
    asm volatile(
        "delay_loop45:\n\t"
        "dec r16\n\t"
        "brne delay_loop45\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[46]));
  if (tmp) {
    DDRB = 0b00001001;
    PORTB |= 0b00001000;
    asm volatile(
        "delay_loop46:\n\t"
        "dec r16\n\t"
        "brne delay_loop46\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[47]));
  if (tmp) {
    DDRB = 0b00000101;
    PORTB |= 0b00000100;
    asm volatile(
        "delay_loop47:\n\t"
        "dec r16\n\t"
        "brne delay_loop47\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[48]));
  if (tmp) {
    DDRB = 0b00000011;
    PORTB |= 0b00000010;
    asm volatile(
        "delay_loop48:\n\t"
        "dec r16\n\t"
        "brne delay_loop48\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[49]));
  if (tmp) {
    DDRC = DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop49:\n\t"
        "dec r16\n\t"
        "brne delay_loop49\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[50]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop50:\n\t"
        "dec r16\n\t"
        "brne delay_loop50\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[51]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop51:\n\t"
        "dec r16\n\t"
        "brne delay_loop51\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[52]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop52:\n\t"
        "dec r16\n\t"
        "brne delay_loop52\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[53]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop53:\n\t"
        "dec r16\n\t"
        "brne delay_loop53\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[54]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop54:\n\t"
        "dec r16\n\t"
        "brne delay_loop54\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[55]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop55:\n\t"
        "dec r16\n\t"
        "brne delay_loop55\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[56]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop56:\n\t"
        "dec r16\n\t"
        "brne delay_loop56\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[57]));
  if (tmp) {
    DDRC = DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop57:\n\t"
        "dec r16\n\t"
        "brne delay_loop57\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[58]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop58:\n\t"
        "dec r16\n\t"
        "brne delay_loop58\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[59]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop59:\n\t"
        "dec r16\n\t"
        "brne delay_loop59\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[60]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop60:\n\t"
        "dec r16\n\t"
        "brne delay_loop60\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[61]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop61:\n\t"
        "dec r16\n\t"
        "brne delay_loop61\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[62]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop62:\n\t"
        "dec r16\n\t"
        "brne delay_loop62\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[63]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop63:\n\t"
        "dec r16\n\t"
        "brne delay_loop63\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[64]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop64:\n\t"
        "dec r16\n\t"
        "brne delay_loop64\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[65]));
  if (tmp) {
    DDRC = DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop65:\n\t"
        "dec r16\n\t"
        "brne delay_loop65\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[66]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop66:\n\t"
        "dec r16\n\t"
        "brne delay_loop66\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[67]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop67:\n\t"
        "dec r16\n\t"
        "brne delay_loop67\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[68]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop68:\n\t"
        "dec r16\n\t"
        "brne delay_loop68\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[69]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop69:\n\t"
        "dec r16\n\t"
        "brne delay_loop69\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[70]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop70:\n\t"
        "dec r16\n\t"
        "brne delay_loop70\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[71]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop71:\n\t"
        "dec r16\n\t"
        "brne delay_loop71\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[72]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop72:\n\t"
        "dec r16\n\t"
        "brne delay_loop72\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[73]));
  if (tmp) {
    DDRC = DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop73:\n\t"
        "dec r16\n\t"
        "brne delay_loop73\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[74]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop74:\n\t"
        "dec r16\n\t"
        "brne delay_loop74\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[75]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop75:\n\t"
        "dec r16\n\t"
        "brne delay_loop75\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[76]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop76:\n\t"
        "dec r16\n\t"
        "brne delay_loop76\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[77]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop77:\n\t"
        "dec r16\n\t"
        "brne delay_loop77\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[78]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop78:\n\t"
        "dec r16\n\t"
        "brne delay_loop78\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[79]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop79:\n\t"
        "dec r16\n\t"
        "brne delay_loop79\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[80]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop80:\n\t"
        "dec r16\n\t"
        "brne delay_loop80\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[81]));
  if (tmp) {
    DDRC = DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop81:\n\t"
        "dec r16\n\t"
        "brne delay_loop81\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[82]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop82:\n\t"
        "dec r16\n\t"
        "brne delay_loop82\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[83]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop83:\n\t"
        "dec r16\n\t"
        "brne delay_loop83\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[84]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop84:\n\t"
        "dec r16\n\t"
        "brne delay_loop84\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[85]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop85:\n\t"
        "dec r16\n\t"
        "brne delay_loop85\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[86]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop86:\n\t"
        "dec r16\n\t"
        "brne delay_loop86\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[87]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop87:\n\t"
        "dec r16\n\t"
        "brne delay_loop87\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[88]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop88:\n\t"
        "dec r16\n\t"
        "brne delay_loop88\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[89]));
  if (tmp) {
    DDRC = DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop89:\n\t"
        "dec r16\n\t"
        "brne delay_loop89\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[90]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop90:\n\t"
        "dec r16\n\t"
        "brne delay_loop90\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[91]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop91:\n\t"
        "dec r16\n\t"
        "brne delay_loop91\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[92]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop92:\n\t"
        "dec r16\n\t"
        "brne delay_loop92\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[93]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop93:\n\t"
        "dec r16\n\t"
        "brne delay_loop93\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[94]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop94:\n\t"
        "dec r16\n\t"
        "brne delay_loop94\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[95]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop95:\n\t"
        "dec r16\n\t"
        "brne delay_loop95\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[96]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop96:\n\t"
        "dec r16\n\t"
        "brne delay_loop96\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[97]));
  if (tmp) {
    DDRC = DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop97:\n\t"
        "dec r16\n\t"
        "brne delay_loop97\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[98]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop98:\n\t"
        "dec r16\n\t"
        "brne delay_loop98\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[99]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop99:\n\t"
        "dec r16\n\t"
        "brne delay_loop99\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[100]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop100:\n\t"
        "dec r16\n\t"
        "brne delay_loop100\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[101]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop101:\n\t"
        "dec r16\n\t"
        "brne delay_loop101\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[102]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop102:\n\t"
        "dec r16\n\t"
        "brne delay_loop102\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[103]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop103:\n\t"
        "dec r16\n\t"
        "brne delay_loop103\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[104]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop104:\n\t"
        "dec r16\n\t"
        "brne delay_loop104\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[105]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop105:\n\t"
        "dec r16\n\t"
        "brne delay_loop105\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[106]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop106:\n\t"
        "dec r16\n\t"
        "brne delay_loop106\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[107]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop107:\n\t"
        "dec r16\n\t"
        "brne delay_loop107\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[108]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop108:\n\t"
        "dec r16\n\t"
        "brne delay_loop108\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[109]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop109:\n\t"
        "dec r16\n\t"
        "brne delay_loop109\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[110]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop110:\n\t"
        "dec r16\n\t"
        "brne delay_loop110\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[111]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop111:\n\t"
        "dec r16\n\t"
        "brne delay_loop111\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[112]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop112:\n\t"
        "dec r16\n\t"
        "brne delay_loop112\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[113]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop113:\n\t"
        "dec r16\n\t"
        "brne delay_loop113\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[114]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop114:\n\t"
        "dec r16\n\t"
        "brne delay_loop114\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[115]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop115:\n\t"
        "dec r16\n\t"
        "brne delay_loop115\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[116]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop116:\n\t"
        "dec r16\n\t"
        "brne delay_loop116\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[117]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop117:\n\t"
        "dec r16\n\t"
        "brne delay_loop117\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[118]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop118:\n\t"
        "dec r16\n\t"
        "brne delay_loop118\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[119]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop119:\n\t"
        "dec r16\n\t"
        "brne delay_loop119\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[120]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop120:\n\t"
        "dec r16\n\t"
        "brne delay_loop120\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[121]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop121:\n\t"
        "dec r16\n\t"
        "brne delay_loop121\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[122]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop122:\n\t"
        "dec r16\n\t"
        "brne delay_loop122\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[123]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop123:\n\t"
        "dec r16\n\t"
        "brne delay_loop123\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[124]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop124:\n\t"
        "dec r16\n\t"
        "brne delay_loop124\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[125]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop125:\n\t"
        "dec r16\n\t"
        "brne delay_loop125\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[126]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop126:\n\t"
        "dec r16\n\t"
        "brne delay_loop126\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[127]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop127:\n\t"
        "dec r16\n\t"
        "brne delay_loop127\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[128]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop128:\n\t"
        "dec r16\n\t"
        "brne delay_loop128\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[129]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop129:\n\t"
        "dec r16\n\t"
        "brne delay_loop129\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[130]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop130:\n\t"
        "dec r16\n\t"
        "brne delay_loop130\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[131]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop131:\n\t"
        "dec r16\n\t"
        "brne delay_loop131\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[132]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop132:\n\t"
        "dec r16\n\t"
        "brne delay_loop132\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[133]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop133:\n\t"
        "dec r16\n\t"
        "brne delay_loop133\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[134]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop134:\n\t"
        "dec r16\n\t"
        "brne delay_loop134\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[135]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop135:\n\t"
        "dec r16\n\t"
        "brne delay_loop135\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[136]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop136:\n\t"
        "dec r16\n\t"
        "brne delay_loop136\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[137]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop137:\n\t"
        "dec r16\n\t"
        "brne delay_loop137\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[138]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop138:\n\t"
        "dec r16\n\t"
        "brne delay_loop138\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[139]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop139:\n\t"
        "dec r16\n\t"
        "brne delay_loop139\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[140]));
  if (tmp) {
    DDRB = 0b10000001;
    PORTB |= 0b00000001;
    asm volatile(
        "delay_loop140:\n\t"
        "dec r16\n\t"
        "brne delay_loop140\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[141]));
  if (tmp) {
    DDRB = DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop141:\n\t"
        "dec r16\n\t"
        "brne delay_loop141\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[142]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop142:\n\t"
        "dec r16\n\t"
        "brne delay_loop142\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[143]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop143:\n\t"
        "dec r16\n\t"
        "brne delay_loop143\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[144]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop144:\n\t"
        "dec r16\n\t"
        "brne delay_loop144\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[145]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop145:\n\t"
        "dec r16\n\t"
        "brne delay_loop145\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[146]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop146:\n\t"
        "dec r16\n\t"
        "brne delay_loop146\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[147]));
  if (tmp) {
    DDRB = 0b01000001;
    PORTB |= 0b00000001;
    asm volatile(
        "delay_loop147:\n\t"
        "dec r16\n\t"
        "brne delay_loop147\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[148]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop148:\n\t"
        "dec r16\n\t"
        "brne delay_loop148\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[149]));
  if (tmp) {
    DDRB = DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop149:\n\t"
        "dec r16\n\t"
        "brne delay_loop149\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[150]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop150:\n\t"
        "dec r16\n\t"
        "brne delay_loop150\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[151]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop151:\n\t"
        "dec r16\n\t"
        "brne delay_loop151\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[152]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop152:\n\t"
        "dec r16\n\t"
        "brne delay_loop152\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[153]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop153:\n\t"
        "dec r16\n\t"
        "brne delay_loop153\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[154]));
  if (tmp) {
    DDRB = 0b00100001;
    PORTB |= 0b00000001;
    asm volatile(
        "delay_loop154:\n\t"
        "dec r16\n\t"
        "brne delay_loop154\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[155]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop155:\n\t"
        "dec r16\n\t"
        "brne delay_loop155\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[156]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop156:\n\t"
        "dec r16\n\t"
        "brne delay_loop156\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[157]));
  if (tmp) {
    DDRB = DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop157:\n\t"
        "dec r16\n\t"
        "brne delay_loop157\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[158]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop158:\n\t"
        "dec r16\n\t"
        "brne delay_loop158\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[159]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop159:\n\t"
        "dec r16\n\t"
        "brne delay_loop159\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[160]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop160:\n\t"
        "dec r16\n\t"
        "brne delay_loop160\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[161]));
  if (tmp) {
    DDRB = 0b00010001;
    PORTB |= 0b00000001;
    asm volatile(
        "delay_loop161:\n\t"
        "dec r16\n\t"
        "brne delay_loop161\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[162]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop162:\n\t"
        "dec r16\n\t"
        "brne delay_loop162\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[163]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop163:\n\t"
        "dec r16\n\t"
        "brne delay_loop163\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[164]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop164:\n\t"
        "dec r16\n\t"
        "brne delay_loop164\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[165]));
  if (tmp) {
    DDRB = DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop165:\n\t"
        "dec r16\n\t"
        "brne delay_loop165\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[166]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop166:\n\t"
        "dec r16\n\t"
        "brne delay_loop166\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[167]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop167:\n\t"
        "dec r16\n\t"
        "brne delay_loop167\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[168]));
  if (tmp) {
    DDRB = 0b00001001;
    PORTB |= 0b00000001;
    asm volatile(
        "delay_loop168:\n\t"
        "dec r16\n\t"
        "brne delay_loop168\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[169]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop169:\n\t"
        "dec r16\n\t"
        "brne delay_loop169\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[170]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop170:\n\t"
        "dec r16\n\t"
        "brne delay_loop170\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[171]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop171:\n\t"
        "dec r16\n\t"
        "brne delay_loop171\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[172]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop172:\n\t"
        "dec r16\n\t"
        "brne delay_loop172\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[173]));
  if (tmp) {
    DDRB = DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop173:\n\t"
        "dec r16\n\t"
        "brne delay_loop173\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[174]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop174:\n\t"
        "dec r16\n\t"
        "brne delay_loop174\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[175]));
  if (tmp) {
    DDRB = 0b00000101;
    PORTB |= 0b00000001;
    asm volatile(
        "delay_loop175:\n\t"
        "dec r16\n\t"
        "brne delay_loop175\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[176]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop176:\n\t"
        "dec r16\n\t"
        "brne delay_loop176\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[177]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop177:\n\t"
        "dec r16\n\t"
        "brne delay_loop177\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[178]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop178:\n\t"
        "dec r16\n\t"
        "brne delay_loop178\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[179]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop179:\n\t"
        "dec r16\n\t"
        "brne delay_loop179\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[180]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop180:\n\t"
        "dec r16\n\t"
        "brne delay_loop180\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[181]));
  if (tmp) {
    DDRB = DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop181:\n\t"
        "dec r16\n\t"
        "brne delay_loop181\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[182]));
  if (tmp) {
    DDRB = 0b00000011;
    PORTB |= 0b00000001;
    asm volatile(
        "delay_loop182:\n\t"
        "dec r16\n\t"
        "brne delay_loop182\n\t");
    PORTB = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[183]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop183:\n\t"
        "dec r16\n\t"
        "brne delay_loop183\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[184]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop184:\n\t"
        "dec r16\n\t"
        "brne delay_loop184\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[185]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop185:\n\t"
        "dec r16\n\t"
        "brne delay_loop185\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[186]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop186:\n\t"
        "dec r16\n\t"
        "brne delay_loop186\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[187]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop187:\n\t"
        "dec r16\n\t"
        "brne delay_loop187\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[188]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop188:\n\t"
        "dec r16\n\t"
        "brne delay_loop188\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[189]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop189:\n\t"
        "dec r16\n\t"
        "brne delay_loop189\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[190]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop190:\n\t"
        "dec r16\n\t"
        "brne delay_loop190\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[191]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop191:\n\t"
        "dec r16\n\t"
        "brne delay_loop191\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[192]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop192:\n\t"
        "dec r16\n\t"
        "brne delay_loop192\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[193]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop193:\n\t"
        "dec r16\n\t"
        "brne delay_loop193\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[194]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop194:\n\t"
        "dec r16\n\t"
        "brne delay_loop194\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[195]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop195:\n\t"
        "dec r16\n\t"
        "brne delay_loop195\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[196]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop196:\n\t"
        "dec r16\n\t"
        "brne delay_loop196\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[197]));
  if (tmp) {
    DDRC = 0b11000000;
    PORTC |= 0b10000000;
    asm volatile(
        "delay_loop197:\n\t"
        "dec r16\n\t"
        "brne delay_loop197\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[198]));
  if (tmp) {
    DDRC = 0b11000000;
    PORTC |= 0b01000000;
    asm volatile(
        "delay_loop198:\n\t"
        "dec r16\n\t"
        "brne delay_loop198\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[199]));
  if (tmp) {
    DDRC = 0b10100000;
    PORTC |= 0b00100000;
    asm volatile(
        "delay_loop199:\n\t"
        "dec r16\n\t"
        "brne delay_loop199\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[200]));
  if (tmp) {
    DDRC = 0b10010000;
    PORTC |= 0b00010000;
    asm volatile(
        "delay_loop200:\n\t"
        "dec r16\n\t"
        "brne delay_loop200\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[201]));
  if (tmp) {
    DDRC = 0b10001000;
    PORTC |= 0b00001000;
    asm volatile(
        "delay_loop201:\n\t"
        "dec r16\n\t"
        "brne delay_loop201\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[202]));
  if (tmp) {
    DDRC = 0b10000100;
    PORTC |= 0b00000100;
    asm volatile(
        "delay_loop202:\n\t"
        "dec r16\n\t"
        "brne delay_loop202\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[203]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop203:\n\t"
        "dec r16\n\t"
        "brne delay_loop203\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[204]));
  if (tmp) {
    DDRC = 0b10100000;
    PORTC |= 0b10000000;
    asm volatile(
        "delay_loop204:\n\t"
        "dec r16\n\t"
        "brne delay_loop204\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[205]));
  if (tmp) {
    DDRC = 0b01100000;
    PORTC |= 0b01000000;
    asm volatile(
        "delay_loop205:\n\t"
        "dec r16\n\t"
        "brne delay_loop205\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[206]));
  if (tmp) {
    DDRC = 0b01100000;
    PORTC |= 0b00100000;
    asm volatile(
        "delay_loop206:\n\t"
        "dec r16\n\t"
        "brne delay_loop206\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[207]));
  if (tmp) {
    DDRC = 0b01010000;
    PORTC |= 0b00010000;
    asm volatile(
        "delay_loop207:\n\t"
        "dec r16\n\t"
        "brne delay_loop207\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[208]));
  if (tmp) {
    DDRC = 0b01001000;
    PORTC |= 0b00001000;
    asm volatile(
        "delay_loop208:\n\t"
        "dec r16\n\t"
        "brne delay_loop208\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[209]));
  if (tmp) {
    DDRC = 0b01000100;
    PORTC |= 0b00000100;
    asm volatile(
        "delay_loop209:\n\t"
        "dec r16\n\t"
        "brne delay_loop209\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[210]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop210:\n\t"
        "dec r16\n\t"
        "brne delay_loop210\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[211]));
  if (tmp) {
    DDRC = 0b10010000;
    PORTC |= 0b10000000;
    asm volatile(
        "delay_loop211:\n\t"
        "dec r16\n\t"
        "brne delay_loop211\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[212]));
  if (tmp) {
    DDRC = 0b01010000;
    PORTC |= 0b01000000;
    asm volatile(
        "delay_loop212:\n\t"
        "dec r16\n\t"
        "brne delay_loop212\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[213]));
  if (tmp) {
    DDRC = 0b00110000;
    PORTC |= 0b00100000;
    asm volatile(
        "delay_loop213:\n\t"
        "dec r16\n\t"
        "brne delay_loop213\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[214]));
  if (tmp) {
    DDRC = 0b00110000;
    PORTC |= 0b00010000;
    asm volatile(
        "delay_loop214:\n\t"
        "dec r16\n\t"
        "brne delay_loop214\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[215]));
  if (tmp) {
    DDRC = 0b00101000;
    PORTC |= 0b00001000;
    asm volatile(
        "delay_loop215:\n\t"
        "dec r16\n\t"
        "brne delay_loop215\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[216]));
  if (tmp) {
    DDRC = 0b00100100;
    PORTC |= 0b00000100;
    asm volatile(
        "delay_loop216:\n\t"
        "dec r16\n\t"
        "brne delay_loop216\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[217]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop217:\n\t"
        "dec r16\n\t"
        "brne delay_loop217\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[218]));
  if (tmp) {
    DDRC = 0b10001000;
    PORTC |= 0b10000000;
    asm volatile(
        "delay_loop218:\n\t"
        "dec r16\n\t"
        "brne delay_loop218\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[219]));
  if (tmp) {
    DDRC = 0b01001000;
    PORTC |= 0b01000000;
    asm volatile(
        "delay_loop219:\n\t"
        "dec r16\n\t"
        "brne delay_loop219\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[220]));
  if (tmp) {
    DDRC = 0b00101000;
    PORTC |= 0b00100000;
    asm volatile(
        "delay_loop220:\n\t"
        "dec r16\n\t"
        "brne delay_loop220\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[221]));
  if (tmp) {
    DDRC = 0b00011000;
    PORTC |= 0b00010000;
    asm volatile(
        "delay_loop221:\n\t"
        "dec r16\n\t"
        "brne delay_loop221\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[222]));
  if (tmp) {
    DDRC = 0b00011000;
    PORTC |= 0b00001000;
    asm volatile(
        "delay_loop222:\n\t"
        "dec r16\n\t"
        "brne delay_loop222\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[223]));
  if (tmp) {
    DDRC = 0b00010100;
    PORTC |= 0b00000100;
    asm volatile(
        "delay_loop223:\n\t"
        "dec r16\n\t"
        "brne delay_loop223\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[224]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop224:\n\t"
        "dec r16\n\t"
        "brne delay_loop224\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[225]));
  if (tmp) {
    DDRC = 0b10000100;
    PORTC |= 0b10000000;
    asm volatile(
        "delay_loop225:\n\t"
        "dec r16\n\t"
        "brne delay_loop225\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[226]));
  if (tmp) {
    DDRC = 0b01000100;
    PORTC |= 0b01000000;
    asm volatile(
        "delay_loop226:\n\t"
        "dec r16\n\t"
        "brne delay_loop226\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[227]));
  if (tmp) {
    DDRC = 0b00100100;
    PORTC |= 0b00100000;
    asm volatile(
        "delay_loop227:\n\t"
        "dec r16\n\t"
        "brne delay_loop227\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[228]));
  if (tmp) {
    DDRC = 0b00010100;
    PORTC |= 0b00010000;
    asm volatile(
        "delay_loop228:\n\t"
        "dec r16\n\t"
        "brne delay_loop228\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[229]));
  if (tmp) {
    DDRC = 0b00001100;
    PORTC |= 0b00001000;
    asm volatile(
        "delay_loop229:\n\t"
        "dec r16\n\t"
        "brne delay_loop229\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[230]));
  if (tmp) {
    DDRC = 0b00001100;
    PORTC |= 0b00000100;
    asm volatile(
        "delay_loop230:\n\t"
        "dec r16\n\t"
        "brne delay_loop230\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[231]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop231:\n\t"
        "dec r16\n\t"
        "brne delay_loop231\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[232]));
  if (tmp) {
    DDRC = 0b10000010;
    PORTC |= 0b10000000;
    asm volatile(
        "delay_loop232:\n\t"
        "dec r16\n\t"
        "brne delay_loop232\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[233]));
  if (tmp) {
    DDRC = 0b01000010;
    PORTC |= 0b01000000;
    asm volatile(
        "delay_loop233:\n\t"
        "dec r16\n\t"
        "brne delay_loop233\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[234]));
  if (tmp) {
    DDRC = 0b00100010;
    PORTC |= 0b00100000;
    asm volatile(
        "delay_loop234:\n\t"
        "dec r16\n\t"
        "brne delay_loop234\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[235]));
  if (tmp) {
    DDRC = 0b00010010;
    PORTC |= 0b00010000;
    asm volatile(
        "delay_loop235:\n\t"
        "dec r16\n\t"
        "brne delay_loop235\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[236]));
  if (tmp) {
    DDRC = 0b00001010;
    PORTC |= 0b00001000;
    asm volatile(
        "delay_loop236:\n\t"
        "dec r16\n\t"
        "brne delay_loop236\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[237]));
  if (tmp) {
    DDRC = 0b00000110;
    PORTC |= 0b00000100;
    asm volatile(
        "delay_loop237:\n\t"
        "dec r16\n\t"
        "brne delay_loop237\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[238]));
  if (tmp) {
    DDRC = DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop238:\n\t"
        "dec r16\n\t"
        "brne delay_loop238\n\t");
    PORTB = DDRB = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[239]));
  if (tmp) {
    DDRC = 0b10000001;
    PORTC |= 0b10000000;
    asm volatile(
        "delay_loop239:\n\t"
        "dec r16\n\t"
        "brne delay_loop239\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[240]));
  if (tmp) {
    DDRC = 0b01000001;
    PORTC |= 0b01000000;
    asm volatile(
        "delay_loop240:\n\t"
        "dec r16\n\t"
        "brne delay_loop240\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[241]));
  if (tmp) {
    DDRC = 0b00100001;
    PORTC |= 0b00100000;
    asm volatile(
        "delay_loop241:\n\t"
        "dec r16\n\t"
        "brne delay_loop241\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[242]));
  if (tmp) {
    DDRC = 0b00010001;
    PORTC |= 0b00010000;
    asm volatile(
        "delay_loop242:\n\t"
        "dec r16\n\t"
        "brne delay_loop242\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[243]));
  if (tmp) {
    DDRC = 0b00001001;
    PORTC |= 0b00001000;
    asm volatile(
        "delay_loop243:\n\t"
        "dec r16\n\t"
        "brne delay_loop243\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[244]));
  if (tmp) {
    DDRC = 0b00000101;
    PORTC |= 0b00000100;
    asm volatile(
        "delay_loop244:\n\t"
        "dec r16\n\t"
        "brne delay_loop244\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[245]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop245:\n\t"
        "dec r16\n\t"
        "brne delay_loop245\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[246]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop246:\n\t"
        "dec r16\n\t"
        "brne delay_loop246\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[247]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop247:\n\t"
        "dec r16\n\t"
        "brne delay_loop247\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[248]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop248:\n\t"
        "dec r16\n\t"
        "brne delay_loop248\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[249]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop249:\n\t"
        "dec r16\n\t"
        "brne delay_loop249\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[250]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop250:\n\t"
        "dec r16\n\t"
        "brne delay_loop250\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[251]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop251:\n\t"
        "dec r16\n\t"
        "brne delay_loop251\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[252]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop252:\n\t"
        "dec r16\n\t"
        "brne delay_loop252\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[253]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop253:\n\t"
        "dec r16\n\t"
        "brne delay_loop253\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[254]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop254:\n\t"
        "dec r16\n\t"
        "brne delay_loop254\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[255]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop255:\n\t"
        "dec r16\n\t"
        "brne delay_loop255\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[256]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop256:\n\t"
        "dec r16\n\t"
        "brne delay_loop256\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[257]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop257:\n\t"
        "dec r16\n\t"
        "brne delay_loop257\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[258]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop258:\n\t"
        "dec r16\n\t"
        "brne delay_loop258\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[259]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop259:\n\t"
        "dec r16\n\t"
        "brne delay_loop259\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[260]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop260:\n\t"
        "dec r16\n\t"
        "brne delay_loop260\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[261]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop261:\n\t"
        "dec r16\n\t"
        "brne delay_loop261\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[262]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop262:\n\t"
        "dec r16\n\t"
        "brne delay_loop262\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[263]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop263:\n\t"
        "dec r16\n\t"
        "brne delay_loop263\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[264]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop264:\n\t"
        "dec r16\n\t"
        "brne delay_loop264\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[265]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop265:\n\t"
        "dec r16\n\t"
        "brne delay_loop265\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[266]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop266:\n\t"
        "dec r16\n\t"
        "brne delay_loop266\n\t");
    PORTB = DDRB = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[267]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop267:\n\t"
        "dec r16\n\t"
        "brne delay_loop267\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[268]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop268:\n\t"
        "dec r16\n\t"
        "brne delay_loop268\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[269]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop269:\n\t"
        "dec r16\n\t"
        "brne delay_loop269\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[270]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop270:\n\t"
        "dec r16\n\t"
        "brne delay_loop270\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[271]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop271:\n\t"
        "dec r16\n\t"
        "brne delay_loop271\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[272]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop272:\n\t"
        "dec r16\n\t"
        "brne delay_loop272\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[273]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop273:\n\t"
        "dec r16\n\t"
        "brne delay_loop273\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[274]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop274:\n\t"
        "dec r16\n\t"
        "brne delay_loop274\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[275]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop275:\n\t"
        "dec r16\n\t"
        "brne delay_loop275\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[276]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop276:\n\t"
        "dec r16\n\t"
        "brne delay_loop276\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[277]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop277:\n\t"
        "dec r16\n\t"
        "brne delay_loop277\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[278]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop278:\n\t"
        "dec r16\n\t"
        "brne delay_loop278\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[279]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop279:\n\t"
        "dec r16\n\t"
        "brne delay_loop279\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[280]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop280:\n\t"
        "dec r16\n\t"
        "brne delay_loop280\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[281]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop281:\n\t"
        "dec r16\n\t"
        "brne delay_loop281\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[282]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop282:\n\t"
        "dec r16\n\t"
        "brne delay_loop282\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[283]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop283:\n\t"
        "dec r16\n\t"
        "brne delay_loop283\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[284]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop284:\n\t"
        "dec r16\n\t"
        "brne delay_loop284\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[285]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop285:\n\t"
        "dec r16\n\t"
        "brne delay_loop285\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[286]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop286:\n\t"
        "dec r16\n\t"
        "brne delay_loop286\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[287]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop287:\n\t"
        "dec r16\n\t"
        "brne delay_loop287\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[288]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop288:\n\t"
        "dec r16\n\t"
        "brne delay_loop288\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[289]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop289:\n\t"
        "dec r16\n\t"
        "brne delay_loop289\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[290]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop290:\n\t"
        "dec r16\n\t"
        "brne delay_loop290\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[291]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop291:\n\t"
        "dec r16\n\t"
        "brne delay_loop291\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[292]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop292:\n\t"
        "dec r16\n\t"
        "brne delay_loop292\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[293]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop293:\n\t"
        "dec r16\n\t"
        "brne delay_loop293\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[294]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop294:\n\t"
        "dec r16\n\t"
        "brne delay_loop294\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[295]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop295:\n\t"
        "dec r16\n\t"
        "brne delay_loop295\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[296]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop296:\n\t"
        "dec r16\n\t"
        "brne delay_loop296\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[297]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop297:\n\t"
        "dec r16\n\t"
        "brne delay_loop297\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[298]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop298:\n\t"
        "dec r16\n\t"
        "brne delay_loop298\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[299]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop299:\n\t"
        "dec r16\n\t"
        "brne delay_loop299\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[300]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop300:\n\t"
        "dec r16\n\t"
        "brne delay_loop300\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[301]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop301:\n\t"
        "dec r16\n\t"
        "brne delay_loop301\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[302]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop302:\n\t"
        "dec r16\n\t"
        "brne delay_loop302\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[303]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop303:\n\t"
        "dec r16\n\t"
        "brne delay_loop303\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[304]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop304:\n\t"
        "dec r16\n\t"
        "brne delay_loop304\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[305]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop305:\n\t"
        "dec r16\n\t"
        "brne delay_loop305\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[306]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop306:\n\t"
        "dec r16\n\t"
        "brne delay_loop306\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[307]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop307:\n\t"
        "dec r16\n\t"
        "brne delay_loop307\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[308]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop308:\n\t"
        "dec r16\n\t"
        "brne delay_loop308\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[309]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop309:\n\t"
        "dec r16\n\t"
        "brne delay_loop309\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[310]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop310:\n\t"
        "dec r16\n\t"
        "brne delay_loop310\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[311]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop311:\n\t"
        "dec r16\n\t"
        "brne delay_loop311\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[312]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop312:\n\t"
        "dec r16\n\t"
        "brne delay_loop312\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[313]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop313:\n\t"
        "dec r16\n\t"
        "brne delay_loop313\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[314]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop314:\n\t"
        "dec r16\n\t"
        "brne delay_loop314\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[315]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop315:\n\t"
        "dec r16\n\t"
        "brne delay_loop315\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[316]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop316:\n\t"
        "dec r16\n\t"
        "brne delay_loop316\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[317]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop317:\n\t"
        "dec r16\n\t"
        "brne delay_loop317\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[318]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop318:\n\t"
        "dec r16\n\t"
        "brne delay_loop318\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[319]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop319:\n\t"
        "dec r16\n\t"
        "brne delay_loop319\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[320]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop320:\n\t"
        "dec r16\n\t"
        "brne delay_loop320\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[321]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop321:\n\t"
        "dec r16\n\t"
        "brne delay_loop321\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[322]));
  if (tmp) {
    DDRB = DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop322:\n\t"
        "dec r16\n\t"
        "brne delay_loop322\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[323]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop323:\n\t"
        "dec r16\n\t"
        "brne delay_loop323\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[324]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop324:\n\t"
        "dec r16\n\t"
        "brne delay_loop324\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[325]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop325:\n\t"
        "dec r16\n\t"
        "brne delay_loop325\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[326]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop326:\n\t"
        "dec r16\n\t"
        "brne delay_loop326\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[327]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop327:\n\t"
        "dec r16\n\t"
        "brne delay_loop327\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[328]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop328:\n\t"
        "dec r16\n\t"
        "brne delay_loop328\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[329]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop329:\n\t"
        "dec r16\n\t"
        "brne delay_loop329\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[330]));
  if (tmp) {
    DDRB = DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop330:\n\t"
        "dec r16\n\t"
        "brne delay_loop330\n\t");
    PORTC = DDRC = DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[331]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop331:\n\t"
        "dec r16\n\t"
        "brne delay_loop331\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[332]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop332:\n\t"
        "dec r16\n\t"
        "brne delay_loop332\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[333]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop333:\n\t"
        "dec r16\n\t"
        "brne delay_loop333\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[334]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop334:\n\t"
        "dec r16\n\t"
        "brne delay_loop334\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[335]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop335:\n\t"
        "dec r16\n\t"
        "brne delay_loop335\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[336]));
  if (tmp) {
    DDRC = 0b10000010;
    PORTC |= 0b00000010;
    asm volatile(
        "delay_loop336:\n\t"
        "dec r16\n\t"
        "brne delay_loop336\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[337]));
  if (tmp) {
    DDRC = 0b10000001;
    PORTC |= 0b00000001;
    asm volatile(
        "delay_loop337:\n\t"
        "dec r16\n\t"
        "brne delay_loop337\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[338]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop338:\n\t"
        "dec r16\n\t"
        "brne delay_loop338\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[339]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop339:\n\t"
        "dec r16\n\t"
        "brne delay_loop339\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[340]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop340:\n\t"
        "dec r16\n\t"
        "brne delay_loop340\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[341]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop341:\n\t"
        "dec r16\n\t"
        "brne delay_loop341\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[342]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop342:\n\t"
        "dec r16\n\t"
        "brne delay_loop342\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[343]));
  if (tmp) {
    DDRC = 0b01000010;
    PORTC |= 0b00000010;
    asm volatile(
        "delay_loop343:\n\t"
        "dec r16\n\t"
        "brne delay_loop343\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[344]));
  if (tmp) {
    DDRC = 0b01000001;
    PORTC |= 0b00000001;
    asm volatile(
        "delay_loop344:\n\t"
        "dec r16\n\t"
        "brne delay_loop344\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[345]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop345:\n\t"
        "dec r16\n\t"
        "brne delay_loop345\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[346]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop346:\n\t"
        "dec r16\n\t"
        "brne delay_loop346\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[347]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop347:\n\t"
        "dec r16\n\t"
        "brne delay_loop347\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[348]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop348:\n\t"
        "dec r16\n\t"
        "brne delay_loop348\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[349]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop349:\n\t"
        "dec r16\n\t"
        "brne delay_loop349\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[350]));
  if (tmp) {
    DDRC = 0b00100010;
    PORTC |= 0b00000010;
    asm volatile(
        "delay_loop350:\n\t"
        "dec r16\n\t"
        "brne delay_loop350\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[351]));
  if (tmp) {
    DDRC = 0b00100001;
    PORTC |= 0b00000001;
    asm volatile(
        "delay_loop351:\n\t"
        "dec r16\n\t"
        "brne delay_loop351\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[352]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop352:\n\t"
        "dec r16\n\t"
        "brne delay_loop352\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[353]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop353:\n\t"
        "dec r16\n\t"
        "brne delay_loop353\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[354]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop354:\n\t"
        "dec r16\n\t"
        "brne delay_loop354\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[355]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop355:\n\t"
        "dec r16\n\t"
        "brne delay_loop355\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[356]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop356:\n\t"
        "dec r16\n\t"
        "brne delay_loop356\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[357]));
  if (tmp) {
    DDRC = 0b00010010;
    PORTC |= 0b00000010;
    asm volatile(
        "delay_loop357:\n\t"
        "dec r16\n\t"
        "brne delay_loop357\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[358]));
  if (tmp) {
    DDRC = 0b00010001;
    PORTC |= 0b00000001;
    asm volatile(
        "delay_loop358:\n\t"
        "dec r16\n\t"
        "brne delay_loop358\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[359]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop359:\n\t"
        "dec r16\n\t"
        "brne delay_loop359\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[360]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop360:\n\t"
        "dec r16\n\t"
        "brne delay_loop360\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[361]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop361:\n\t"
        "dec r16\n\t"
        "brne delay_loop361\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[362]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop362:\n\t"
        "dec r16\n\t"
        "brne delay_loop362\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[363]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop363:\n\t"
        "dec r16\n\t"
        "brne delay_loop363\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[364]));
  if (tmp) {
    DDRC = 0b00001010;
    PORTC |= 0b00000010;
    asm volatile(
        "delay_loop364:\n\t"
        "dec r16\n\t"
        "brne delay_loop364\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[365]));
  if (tmp) {
    DDRC = 0b00001001;
    PORTC |= 0b00000001;
    asm volatile(
        "delay_loop365:\n\t"
        "dec r16\n\t"
        "brne delay_loop365\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[366]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop366:\n\t"
        "dec r16\n\t"
        "brne delay_loop366\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[367]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop367:\n\t"
        "dec r16\n\t"
        "brne delay_loop367\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[368]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop368:\n\t"
        "dec r16\n\t"
        "brne delay_loop368\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[369]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop369:\n\t"
        "dec r16\n\t"
        "brne delay_loop369\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[370]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop370:\n\t"
        "dec r16\n\t"
        "brne delay_loop370\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[371]));
  if (tmp) {
    DDRC = 0b00000110;
    PORTC |= 0b00000010;
    asm volatile(
        "delay_loop371:\n\t"
        "dec r16\n\t"
        "brne delay_loop371\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[372]));
  if (tmp) {
    DDRC = 0b00000101;
    PORTC |= 0b00000001;
    asm volatile(
        "delay_loop372:\n\t"
        "dec r16\n\t"
        "brne delay_loop372\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[373]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop373:\n\t"
        "dec r16\n\t"
        "brne delay_loop373\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[374]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop374:\n\t"
        "dec r16\n\t"
        "brne delay_loop374\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[375]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop375:\n\t"
        "dec r16\n\t"
        "brne delay_loop375\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[376]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop376:\n\t"
        "dec r16\n\t"
        "brne delay_loop376\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[377]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop377:\n\t"
        "dec r16\n\t"
        "brne delay_loop377\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[378]));
  if (tmp) {
    DDRC = 0b00000011;
    PORTC |= 0b00000010;
    asm volatile(
        "delay_loop378:\n\t"
        "dec r16\n\t"
        "brne delay_loop378\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[379]));
  if (tmp) {
    DDRC = 0b00000011;
    PORTC |= 0b00000001;
    asm volatile(
        "delay_loop379:\n\t"
        "dec r16\n\t"
        "brne delay_loop379\n\t");
    PORTC = DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[380]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop380:\n\t"
        "dec r16\n\t"
        "brne delay_loop380\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[381]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop381:\n\t"
        "dec r16\n\t"
        "brne delay_loop381\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[382]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop382:\n\t"
        "dec r16\n\t"
        "brne delay_loop382\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[383]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop383:\n\t"
        "dec r16\n\t"
        "brne delay_loop383\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[384]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop384:\n\t"
        "dec r16\n\t"
        "brne delay_loop384\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[385]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop385:\n\t"
        "dec r16\n\t"
        "brne delay_loop385\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[386]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop386:\n\t"
        "dec r16\n\t"
        "brne delay_loop386\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[387]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop387:\n\t"
        "dec r16\n\t"
        "brne delay_loop387\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[388]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop388:\n\t"
        "dec r16\n\t"
        "brne delay_loop388\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[389]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop389:\n\t"
        "dec r16\n\t"
        "brne delay_loop389\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[390]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop390:\n\t"
        "dec r16\n\t"
        "brne delay_loop390\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[391]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop391:\n\t"
        "dec r16\n\t"
        "brne delay_loop391\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[392]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop392:\n\t"
        "dec r16\n\t"
        "brne delay_loop392\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[393]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop393:\n\t"
        "dec r16\n\t"
        "brne delay_loop393\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[394]));
  if (tmp) {
    DDRD |= 0b11000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop394:\n\t"
        "dec r16\n\t"
        "brne delay_loop394\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b00001111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[395]));
  if (tmp) {
    DDRD |= 0b11000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop395:\n\t"
        "dec r16\n\t"
        "brne delay_loop395\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b00001111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[396]));
  if (tmp) {
    DDRD |= 0b10100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop396:\n\t"
        "dec r16\n\t"
        "brne delay_loop396\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b00001111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[397]));
  if (tmp) {
    DDRD |= 0b10010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop397:\n\t"
        "dec r16\n\t"
        "brne delay_loop397\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b00001111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[398]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop398:\n\t"
        "dec r16\n\t"
        "brne delay_loop398\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[399]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop399:\n\t"
        "dec r16\n\t"
        "brne delay_loop399\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[400]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop400:\n\t"
        "dec r16\n\t"
        "brne delay_loop400\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[401]));
  if (tmp) {
    DDRD |= 0b10100000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop401:\n\t"
        "dec r16\n\t"
        "brne delay_loop401\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b00001111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[402]));
  if (tmp) {
    DDRD |= 0b01100000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop402:\n\t"
        "dec r16\n\t"
        "brne delay_loop402\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b00001111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[403]));
  if (tmp) {
    DDRD |= 0b01100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop403:\n\t"
        "dec r16\n\t"
        "brne delay_loop403\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b00001111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[404]));
  if (tmp) {
    DDRD |= 0b01010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop404:\n\t"
        "dec r16\n\t"
        "brne delay_loop404\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b00001111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[405]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop405:\n\t"
        "dec r16\n\t"
        "brne delay_loop405\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[406]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop406:\n\t"
        "dec r16\n\t"
        "brne delay_loop406\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[407]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop407:\n\t"
        "dec r16\n\t"
        "brne delay_loop407\n\t");
    PORTC = DDRC = tmp;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[408]));
  if (tmp) {
    DDRD |= 0b10010000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop408:\n\t"
        "dec r16\n\t"
        "brne delay_loop408\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b00001111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[409]));
  if (tmp) {
    DDRD |= 0b01010000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop409:\n\t"
        "dec r16\n\t"
        "brne delay_loop409\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b00001111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[410]));
  if (tmp) {
    DDRD |= 0b00110000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop410:\n\t"
        "dec r16\n\t"
        "brne delay_loop410\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b00001111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[411]));
  if (tmp) {
    DDRD |= 0b00110000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop411:\n\t"
        "dec r16\n\t"
        "brne delay_loop411\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b00001111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[412]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop412:\n\t"
        "dec r16\n\t"
        "brne delay_loop412\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[413]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop413:\n\t"
        "dec r16\n\t"
        "brne delay_loop413\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[414]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop414:\n\t"
        "dec r16\n\t"
        "brne delay_loop414\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[415]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop415:\n\t"
        "dec r16\n\t"
        "brne delay_loop415\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[416]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop416:\n\t"
        "dec r16\n\t"
        "brne delay_loop416\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[417]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop417:\n\t"
        "dec r16\n\t"
        "brne delay_loop417\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[418]));
  if (tmp) {
    DDRA |= 0b00001000;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop418:\n\t"
        "dec r16\n\t"
        "brne delay_loop418\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRA &= 0b11110111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[419]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRA |= 0b00001000;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop419:\n\t"
        "dec r16\n\t"
        "brne delay_loop419\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110111;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[420]));
  if (tmp) {
    DDRA |= 0b00000110;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop420:\n\t"
        "dec r16\n\t"
        "brne delay_loop420\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11110000;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[421]));
  if (tmp) {
    DDRA |= 0b00001100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop421:\n\t"
        "dec r16\n\t"
        "brne delay_loop421\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11110000;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[422]));
  if (tmp) {
    DDRA |= 0b00000110;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop422:\n\t"
        "dec r16\n\t"
        "brne delay_loop422\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11110000;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[423]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop423:\n\t"
        "dec r16\n\t"
        "brne delay_loop423\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[424]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop424:\n\t"
        "dec r16\n\t"
        "brne delay_loop424\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[425]));
  if (tmp) {
    DDRA |= 0b00001100;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop425:\n\t"
        "dec r16\n\t"
        "brne delay_loop425\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110000;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[426]));
  if (tmp) {
    DDRA |= 0b00001010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop426:\n\t"
        "dec r16\n\t"
        "brne delay_loop426\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11110000;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[427]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop427:\n\t"
        "dec r16\n\t"
        "brne delay_loop427\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[428]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop428:\n\t"
        "dec r16\n\t"
        "brne delay_loop428\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[429]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop429:\n\t"
        "dec r16\n\t"
        "brne delay_loop429\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[430]));
  if (tmp) {
    DDRA |= 0b00000011;
    PORTA |= 0b00000001;
    asm volatile(
        "delay_loop430:\n\t"
        "dec r16\n\t"
        "brne delay_loop430\n\t");
    PORTA &= 0b11111110;
    DDRA &= 0b11110000;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[431]));
  if (tmp) {
    DDRA |= 0b00001010;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop431:\n\t"
        "dec r16\n\t"
        "brne delay_loop431\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110000;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[432]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop432:\n\t"
        "dec r16\n\t"
        "brne delay_loop432\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[433]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop433:\n\t"
        "dec r16\n\t"
        "brne delay_loop433\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[434]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop434:\n\t"
        "dec r16\n\t"
        "brne delay_loop434\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[435]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop435:\n\t"
        "dec r16\n\t"
        "brne delay_loop435\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[436]));
  if (tmp) {
    DDRA |= 0b00000011;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop436:\n\t"
        "dec r16\n\t"
        "brne delay_loop436\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11110000;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[437]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop437:\n\t"
        "dec r16\n\t"
        "brne delay_loop437\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[438]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop438:\n\t"
        "dec r16\n\t"
        "brne delay_loop438\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[439]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop439:\n\t"
        "dec r16\n\t"
        "brne delay_loop439\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[440]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop440:\n\t"
        "dec r16\n\t"
        "brne delay_loop440\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[441]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop441:\n\t"
        "dec r16\n\t"
        "brne delay_loop441\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[442]));
  if (tmp) {
    DDRA |= 0b00000101;
    PORTA |= 0b00000001;
    asm volatile(
        "delay_loop442:\n\t"
        "dec r16\n\t"
        "brne delay_loop442\n\t");
    PORTA &= 0b11111110;
    DDRA &= 0b11110000;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[443]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop443:\n\t"
        "dec r16\n\t"
        "brne delay_loop443\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[444]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop444:\n\t"
        "dec r16\n\t"
        "brne delay_loop444\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[445]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop445:\n\t"
        "dec r16\n\t"
        "brne delay_loop445\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[446]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop446:\n\t"
        "dec r16\n\t"
        "brne delay_loop446\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[447]));
  if (tmp) {
    DDRC |= 0b01000000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop447:\n\t"
        "dec r16\n\t"
        "brne delay_loop447\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[448]));
  if (tmp) {
    DDRC |= 0b10000000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop448:\n\t"
        "dec r16\n\t"
        "brne delay_loop448\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[449]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRC = PORTC = 0b10000000;
    asm volatile(
        "delay_loop449:\n\t"
        "dec r16\n\t"
        "brne delay_loop449\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[450]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop450:\n\t"
        "dec r16\n\t"
        "brne delay_loop450\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[451]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop451:\n\t"
        "dec r16\n\t"
        "brne delay_loop451\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[452]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop452:\n\t"
        "dec r16\n\t"
        "brne delay_loop452\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[453]));
  if (tmp) {
    DDRA |= 0b00000101;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop453:\n\t"
        "dec r16\n\t"
        "brne delay_loop453\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11110000;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[454]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop454:\n\t"
        "dec r16\n\t"
        "brne delay_loop454\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[455]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRC = PORTC = 0b01000000;
    asm volatile(
        "delay_loop455:\n\t"
        "dec r16\n\t"
        "brne delay_loop455\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[456]));
  if (tmp) {
    DDRB |= 0b00000001;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop456:\n\t"
        "dec r16\n\t"
        "brne delay_loop456\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[457]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop457:\n\t"
        "dec r16\n\t"
        "brne delay_loop457\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[458]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop458:\n\t"
        "dec r16\n\t"
        "brne delay_loop458\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[459]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop459:\n\t"
        "dec r16\n\t"
        "brne delay_loop459\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[460]));
  if (tmp) {
    DDRA |= 0b00001001;
    PORTA |= 0b00000001;
    asm volatile(
        "delay_loop460:\n\t"
        "dec r16\n\t"
        "brne delay_loop460\n\t");
    PORTA &= 0b11111110;
    DDRA &= 0b11110000;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[461]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop461:\n\t"
        "dec r16\n\t"
        "brne delay_loop461\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[462]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRB = PORTB = 0b00000001;
    asm volatile(
        "delay_loop462:\n\t"
        "dec r16\n\t"
        "brne delay_loop462\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[463]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop463:\n\t"
        "dec r16\n\t"
        "brne delay_loop463\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[464]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop464:\n\t"
        "dec r16\n\t"
        "brne delay_loop464\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[465]));
  if (tmp) {
    DDRB |= 0b00000010;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop465:\n\t"
        "dec r16\n\t"
        "brne delay_loop465\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[466]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop466:\n\t"
        "dec r16\n\t"
        "brne delay_loop466\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[467]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop467:\n\t"
        "dec r16\n\t"
        "brne delay_loop467\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[468]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop468:\n\t"
        "dec r16\n\t"
        "brne delay_loop468\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[469]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRB = PORTB = 0b00000010;
    asm volatile(
        "delay_loop469:\n\t"
        "dec r16\n\t"
        "brne delay_loop469\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[470]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop470:\n\t"
        "dec r16\n\t"
        "brne delay_loop470\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[471]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop471:\n\t"
        "dec r16\n\t"
        "brne delay_loop471\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[472]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop472:\n\t"
        "dec r16\n\t"
        "brne delay_loop472\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[473]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop473:\n\t"
        "dec r16\n\t"
        "brne delay_loop473\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[474]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop474:\n\t"
        "dec r16\n\t"
        "brne delay_loop474\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[475]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop475:\n\t"
        "dec r16\n\t"
        "brne delay_loop475\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[476]));
  if (tmp) {
    DDRA |= 0b00001001;
    PORTA |= 0b00001000;
    asm volatile(
        "delay_loop476:\n\t"
        "dec r16\n\t"
        "brne delay_loop476\n\t");
    PORTA &= 0b11110111;
    DDRA &= 0b11110000;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[477]));
  if (tmp) {
    DDRD |= 0b00010000;
    DDRA |= 0b00000001;
    PORTA |= 0b00000001;
    asm volatile(
        "delay_loop477:\n\t"
        "dec r16\n\t"
        "brne delay_loop477\n\t");
    PORTA &= 0b11111110;
    DDRA &= 0b11111110;
    DDRD &= 0b11101111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[478]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop478:\n\t"
        "dec r16\n\t"
        "brne delay_loop478\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[479]));
  if (tmp) {
    DDRB |= 0b00000100;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop479:\n\t"
        "dec r16\n\t"
        "brne delay_loop479\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[480]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRB = PORTB = 0b00000100;
    asm volatile(
        "delay_loop480:\n\t"
        "dec r16\n\t"
        "brne delay_loop480\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[481]));
  if (tmp) {
    DDRC |= 0b00000010;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop481:\n\t"
        "dec r16\n\t"
        "brne delay_loop481\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[482]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRC = PORTC = 0b00000010;
    asm volatile(
        "delay_loop482:\n\t"
        "dec r16\n\t"
        "brne delay_loop482\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[483]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop483:\n\t"
        "dec r16\n\t"
        "brne delay_loop483\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[484]));
  if (tmp) {
    DDRB |= 0b00010000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop484:\n\t"
        "dec r16\n\t"
        "brne delay_loop484\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[485]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop485:\n\t"
        "dec r16\n\t"
        "brne delay_loop485\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[486]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop486:\n\t"
        "dec r16\n\t"
        "brne delay_loop486\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[487]));
  if (tmp) {
    DDRB |= 0b00001000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop487:\n\t"
        "dec r16\n\t"
        "brne delay_loop487\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[488]));
  if (tmp) {
    DDRB |= 0b00100000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop488:\n\t"
        "dec r16\n\t"
        "brne delay_loop488\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[489]));
  if (tmp) {
    DDRA |= 0b00000001;
    DDRD |= 0b00010000;
    PORTD |= 0b00010000;
    asm volatile(
        "delay_loop489:\n\t"
        "dec r16\n\t"
        "brne delay_loop489\n\t");
    PORTD &= 0b11101111;
    DDRD &= 0b11101111;
    DDRA &= 0b11111110;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[490]));
  if (tmp) {
    DDRD |= 0b00100000;
    DDRA |= 0b00000001;
    PORTA |= 0b00000001;
    asm volatile(
        "delay_loop490:\n\t"
        "dec r16\n\t"
        "brne delay_loop490\n\t");
    PORTA &= 0b11111110;
    DDRA &= 0b11111110;
    DDRD &= 0b11011111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[491]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop491:\n\t"
        "dec r16\n\t"
        "brne delay_loop491\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[492]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop492:\n\t"
        "dec r16\n\t"
        "brne delay_loop492\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[493]));
  if (tmp) {
    DDRC |= 0b00000100;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop493:\n\t"
        "dec r16\n\t"
        "brne delay_loop493\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[494]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop494:\n\t"
        "dec r16\n\t"
        "brne delay_loop494\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[495]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRB = PORTB = 0b00010000;
    asm volatile(
        "delay_loop495:\n\t"
        "dec r16\n\t"
        "brne delay_loop495\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[496]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop496:\n\t"
        "dec r16\n\t"
        "brne delay_loop496\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[497]));
  if (tmp) {
    DDRC |= 0b00100000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop497:\n\t"
        "dec r16\n\t"
        "brne delay_loop497\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[498]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRB = PORTB = 0b00001000;
    asm volatile(
        "delay_loop498:\n\t"
        "dec r16\n\t"
        "brne delay_loop498\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[499]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRB = PORTB = 0b00100000;
    asm volatile(
        "delay_loop499:\n\t"
        "dec r16\n\t"
        "brne delay_loop499\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[500]));
  if (tmp) {
    DDRA |= 0b00000001;
    DDRD |= 0b00100000;
    PORTD |= 0b00100000;
    asm volatile(
        "delay_loop500:\n\t"
        "dec r16\n\t"
        "brne delay_loop500\n\t");
    PORTD &= 0b11011111;
    DDRD &= 0b11011111;
    DDRA &= 0b11111110;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[501]));
  if (tmp) {
    DDRD |= 0b01000000;
    DDRA |= 0b00000001;
    PORTA |= 0b00000001;
    asm volatile(
        "delay_loop501:\n\t"
        "dec r16\n\t"
        "brne delay_loop501\n\t");
    PORTA &= 0b11111110;
    DDRA &= 0b11111110;
    DDRD &= 0b10111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[502]));
  if (tmp) {
    DDRD |= 0b10000000;
    DDRA |= 0b00000001;
    PORTA |= 0b00000001;
    asm volatile(
        "delay_loop502:\n\t"
        "dec r16\n\t"
        "brne delay_loop502\n\t");
    PORTA &= 0b11111110;
    DDRA &= 0b11111110;
    DDRD &= 0b01111111;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[503]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRC = PORTC = 0b00000100;
    asm volatile(
        "delay_loop503:\n\t"
        "dec r16\n\t"
        "brne delay_loop503\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[504]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop504:\n\t"
        "dec r16\n\t"
        "brne delay_loop504\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[505]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop505:\n\t"
        "dec r16\n\t"
        "brne delay_loop505\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[506]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop506:\n\t"
        "dec r16\n\t"
        "brne delay_loop506\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[507]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRC = PORTC = 0b00100000;
    asm volatile(
        "delay_loop507:\n\t"
        "dec r16\n\t"
        "brne delay_loop507\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[508]));
  if (tmp) {
    DDRB |= 0b01000000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop508:\n\t"
        "dec r16\n\t"
        "brne delay_loop508\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[509]));
  if (tmp) {
    DDRA |= 0b00000001;
    DDRD |= 0b01000000;
    PORTD |= 0b01000000;
    asm volatile(
        "delay_loop509:\n\t"
        "dec r16\n\t"
        "brne delay_loop509\n\t");
    PORTD &= 0b10111111;
    DDRD &= 0b10111111;
    DDRA &= 0b11111110;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[510]));
  if (tmp) {
    DDRA |= 0b00000001;
    DDRD |= 0b10000000;
    PORTD |= 0b10000000;
    asm volatile(
        "delay_loop510:\n\t"
        "dec r16\n\t"
        "brne delay_loop510\n\t");
    PORTD &= 0b01111111;
    DDRD &= 0b01111111;
    DDRA &= 0b11111110;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[511]));
  if (tmp) {
    DDRC |= 0b00001000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop511:\n\t"
        "dec r16\n\t"
        "brne delay_loop511\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[512]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop512:\n\t"
        "dec r16\n\t"
        "brne delay_loop512\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[513]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRB = PORTB = 0b01000000;
    asm volatile(
        "delay_loop513:\n\t"
        "dec r16\n\t"
        "brne delay_loop513\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[514]));
  if (tmp) {
    DDRC |= 0b00000001;
    DDRA |= 0b00000001;
    PORTA |= 0b00000001;
    asm volatile(
        "delay_loop514:\n\t"
        "dec r16\n\t"
        "brne delay_loop514\n\t");
    PORTA &= 0b11111110;
    DDRA &= 0b11111110;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[515]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRC = PORTC = 0b00001000;
    asm volatile(
        "delay_loop515:\n\t"
        "dec r16\n\t"
        "brne delay_loop515\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[516]));
  if (tmp) {
    DDRB |= 0b10000000;
    DDRA |= 0b00000010;
    PORTA |= 0b00000010;
    asm volatile(
        "delay_loop516:\n\t"
        "dec r16\n\t"
        "brne delay_loop516\n\t");
    PORTA &= 0b11111101;
    DDRA &= 0b11111101;
    DDRB = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[517]));
  if (tmp) {
    DDRC |= 0b00010000;
    DDRA |= 0b00000100;
    PORTA |= 0b00000100;
    asm volatile(
        "delay_loop517:\n\t"
        "dec r16\n\t"
        "brne delay_loop517\n\t");
    PORTA &= 0b11111011;
    DDRA &= 0b11111011;
    DDRC = tmp;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[518]));
  if (tmp) {
    DDRA |= 0b00000010;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop518:\n\t"
        "dec r16\n\t"
        "brne delay_loop518\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111101;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[519]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRC = PORTC = 0b00010000;
    asm volatile(
        "delay_loop519:\n\t"
        "dec r16\n\t"
        "brne delay_loop519\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[520]));
  if (tmp) {
    DDRA |= 0b00000100;
    DDRB = PORTB = 0b10000000;
    asm volatile(
        "delay_loop520:\n\t"
        "dec r16\n\t"
        "brne delay_loop520\n\t");
    PORTB = DDRB = tmp;
    DDRA &= 0b11111011;
  }

  asm volatile("lds %0, %1\n\t" : "=r"(tmp) : "i"(&vRAM[521]));
  if (tmp) {
    DDRA |= 0b00000001;
    DDRC = PORTC = 0b00000001;
    asm volatile(
        "delay_loop521:\n\t"
        "dec r16\n\t"
        "brne delay_loop521\n\t");
    PORTC = DDRC = tmp;
    DDRA &= 0b11111110;
  }

  asm volatile("mov r16, %0" : : "r"(r16_save));  // restore r16

  DDRA |= 0b00001111;
  DDRB = 255;
  DDRC = 255;
  DDRD |= 0b11110000;
}
