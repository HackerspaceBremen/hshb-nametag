#ifndef CHARGE_H_
#define CHARGE_H_

#include <avr/io.h>
#include <stdint.h>

// Charging when this bit is not set
#define CHARGING_PIN_HIGH (PIND & (1 << 3))

// STRINGS USED FOR USER COMMUNICATION ON LAUNCH/SHUTDOWN
#define STR_DEVICE_ON " Board %03d"
#define STR_DEVICE_OFF "Goodbye!"

// STRINGS USED FOR USER COMMUNICATION ON BATTERY STATES
#define STR_BATTERY_FULL "Battery Full"
#define STR_BATTERY_CHRG "Charging"
#define STR_BATTERY_PERC "Battery %d%%"
#define STR_BATTERY_LOW "Low Battery"
#define STR_BATTERY_VOLT "%d Volt"

struct ChargeData {
  uint8_t charging : 1;
  uint8_t charge_on : 1;
  uint8_t low_battery : 1;
  uint32_t last_charge;
};

extern volatile struct ChargeData charge_data;

void charge_display_state();
void charge_battery_check();
void charge_handle_state();
void deep_sleep();

#endif
