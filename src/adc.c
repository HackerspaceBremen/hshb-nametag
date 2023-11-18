#include "adc.h"

#include <avr/interrupt.h>

#include "calibration_data.h"
#include "charge.h"

volatile struct ADCValues adc_values = {
    .adc_value = 0,
};

void adc_init() {
  // Set ADC MUX to ADC2 (pin PC2)
  ADMUX |= (1 << MUX2) | (1 << MUX0);
  // Enable ADC interrupt and set prescaler to 128 (8 MHz / 128 = 62.5 kHz)
  ADCSRA |= (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void adc_enable() {
  ADMUX |= (1 << REFS0) | (1 << REFS1);  // Select internal 2.56V reference
  ADCSRA |= (1 << ADEN);
}

void adc_disable() {
  ADCSRA &= ~(1 << ADEN);
  // Ensure internal voltage reference is disabled
  ADMUX &= ~(1 << REFS0 | 1 << REFS1);
}

void adc_start_conversion() { ADCSRA |= (1 << ADSC); }

ISR(ADC_vect) {
  adc_values.adc_value = ADC;
  // Voltage Divider Off (active low)
  PORTA |= (1 << 6);
  adc_values.voltage =
      (uint32_t)(adc_values.adc_value + ADC_OFFSET) * ADC_GAIN / 1000;

  if (charge_data.charging) {
    if (adc_values.adc_value < CHARGE_MIN_ADC) {
      // percent = pgm_read_byte(&chargeLUT[0]);
      adc_values.percent = 0;
    } else if (adc_values.adc_value <= CHARGE_MAX_ADC) {
      adc_values.percent =
          pgm_read_byte(&chargeLUT[adc_values.adc_value - CHARGE_MIN_ADC]);
    } else {
      // percent = pgm_read_byte(&chargeLUT[CHARGE_MAX_ADC-DISCHARGE_MIN_ADC]);
      adc_values.percent = 100;
    }
  } else {
    if (adc_values.adc_value < DISCHARGE_MIN_ADC) {
      // percent = pgm_read_byte(&dischargeLUT[0]);
      adc_values.percent = 0;
    } else if (adc_values.adc_value <= DISCHARGE_MAX_ADC) {
      adc_values.percent = pgm_read_byte(
          &dischargeLUT[adc_values.adc_value - DISCHARGE_MIN_ADC]);
    } else {
      // percent =
      // pgm_read_byte(&dischargeLUT[DISCHARGE_MAX_ADC-DISCHARGE_MIN_ADC]);
      adc_values.percent = 100;
    }
  }

  adc_disable();
}
