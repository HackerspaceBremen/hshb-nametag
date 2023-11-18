#ifndef SLOTS_H_
#define SLOTS_H_

#include <stdint.h>

#define SLOT_TEXT_BUFFER_SIZE 90
#define SLOT_EEPROM_SIZE 100
#define SLOT_MAX 10

enum SLOT_TEXT_TYPE {
  NO_TEXT,
  STATIC,
  SCROLL,
};

struct Slot {
  uint8_t slot_no;

  // TODO: These could be changed to bit fields, but that would require
  // migration of existing EEPROM layouts
  uint8_t enabled;
  uint8_t animation;
  uint8_t seconds_scrolls;
  uint8_t text_type;
  uint8_t offset_scroll_speed;
  uint8_t scaler;
  uint8_t char_space;
  char text[SLOT_TEXT_BUFFER_SIZE];

  // Helper variables not synced to EEPROM
  uint32_t slot_loaded_millis;
  uint8_t scroll_counter, last_animation;
};

extern struct Slot current_slot;

/**
 * Loads a Slot struct from EEPROM
 * @param slot The slot structure to be filled
 */
void slot_load(struct Slot *slot);

/**
 * Saves a Slot struct to EEPROM
 * @param slot The slot structure
 */
void slot_save(struct Slot *slot);

/**
 * Prints slot parameters to UART
 * @param slot The slot structure
 */
void slot_print(struct Slot *slot);

/**
 * Previews slot parameters to UART
 * @param slot The slot structure
 */
void slot_preview(struct Slot *slot);

/**
 * Draws one frame of the current slot, sets advanceSlot when it is time to
 * advance to the next slot
 */
void slot_handle();

/**
 * Advances the current slot by 1
 */
void slot_advance();

#endif
