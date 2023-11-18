#ifndef CHARGE_H_
#define CHARGE_H_

#include <avr/io.h>
#include <stdint.h>

// Charging when this bit is not set
#define CHARGING_PIN_HIGH (PIND & (1 << 3))

struct ChargeData {
  uint8_t charging : 1;
  uint8_t charge_on : 1;
  uint8_t low_battery : 1;
  uint32_t last_charge;
};

extern volatile struct ChargeData charge_data;

void charge_display_state();
void battery_check();
void handle_state();
void deep_sleep();

#endif
