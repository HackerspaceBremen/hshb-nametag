#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

extern volatile uint32_t global_millis;
extern volatile uint32_t global_counter;
extern volatile uint8_t scroll_timer_counter;
extern volatile uint8_t adc_timer_counter;

void timer_init();

#endif
