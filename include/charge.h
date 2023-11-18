#ifndef CHARGE_H_
#define CHARGE_H_

#include <stdint.h>

struct ChargeData {
  uint8_t charging;
  uint8_t charge_on;
  uint8_t low_battery;
  uint32_t last_charge;
};

extern volatile struct ChargeData charge_data;

void charge_display_state();
void battery_check();
void handle_state();
void deep_sleep();

#endif
