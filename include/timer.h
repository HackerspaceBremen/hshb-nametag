#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

extern volatile uint32_t global_millis;

void timer_init();

#endif
