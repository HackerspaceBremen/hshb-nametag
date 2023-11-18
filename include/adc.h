#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

struct ADCValues {
  uint16_t adc_value;
  uint16_t voltage;
  int8_t percent;
};

extern volatile struct ADCValues adc_values;

void adc_init();

void adc_enable();
void adc_disable();

void adc_start_conversion();

#endif
