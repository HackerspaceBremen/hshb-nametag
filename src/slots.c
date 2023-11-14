#include "slots.h"

#include <avr/eeprom.h>
#include <stdio.h>

#include "uart.h"

uint8_t *get_base_address(uint8_t slot_no) {
  return (uint8_t *)(slot_no * SLOT_EEPROM_SIZE);
}

void load_slot(struct Slot *slot) {
  const uint8_t *base_address = get_base_address(slot->slot_no);
  slot->enabled = eeprom_read_byte(base_address);
  slot->animation = eeprom_read_byte(base_address + 1);
  slot->seconds_scrolls = eeprom_read_byte(base_address + 2);
  slot->text_type = eeprom_read_byte(base_address + 3);
  slot->offset_scroll_speed = eeprom_read_byte(base_address + 4);
  slot->scaler = eeprom_read_byte(base_address + 5);
  slot->char_space = eeprom_read_byte(base_address + 6);
  if (slot->text_type != NO_TEXT) {
    eeprom_read_block(slot->text, base_address + 7, SLOT_TEXT_BUFFER_SIZE);
  } else {
    slot->text[0] = 0;
  }
  slot->text[SLOT_TEXT_BUFFER_SIZE - 1] =
      0;  // Ensure string ends with \0 even with corrupted EEPROM

  slot->slot_loaded_millis = 0;
  slot->scroll_counter = 0;
}

void sanitize_chars(char *chars) {
  // Iterate over text to process special chars
  uint8_t write_ptr = 0, read_ptr = 0;
  while ((read_ptr < SLOT_TEXT_BUFFER_SIZE) &&
         (write_ptr < SLOT_TEXT_BUFFER_SIZE)) {
    if (chars[read_ptr] == '\0') {
      return;
    }

    if (chars[read_ptr] == 195) {
      chars[write_ptr++] = chars[++read_ptr];
      read_ptr++;
    } else {
      chars[write_ptr++] = chars[read_ptr++];
    }
  }
}

void save_slot(struct Slot *slot) {
  uint8_t *base_address = get_base_address(slot->slot_no);
  eeprom_write_byte(base_address, slot->enabled);
  eeprom_write_byte(base_address + 1, slot->animation);
  eeprom_write_byte(base_address + 2, slot->seconds_scrolls);
  eeprom_write_byte(base_address + 3, slot->text_type);
  eeprom_write_byte(base_address + 4, slot->offset_scroll_speed);
  eeprom_write_byte(base_address + 5, slot->scaler);
  eeprom_write_byte(base_address + 6, slot->char_space);

  if (slot->text_type != NO_TEXT) {
    // No need to write text data if it is not shown anyway
    sanitize_chars(slot->text);
    eeprom_write_block(slot->text, base_address + 7, SLOT_TEXT_BUFFER_SIZE);
  }
}

void print_slot(struct Slot *slot) {
  if (!slot->enabled) {
    tx_len = snprintf((char *)tx_buf, TX_BUFFER_SIZE,
                      "Slot %d is not active.\r\n", slot->slot_no);
  } else {
    tx_len = snprintf(
        (char *)tx_buf, TX_BUFFER_SIZE,
        "Slot %d:\r\n%d %d %d %d %d %d %d\r\n%s\r\n", slot->slot_no,
        slot->enabled, slot->animation, slot->seconds_scrolls, slot->text_type,
        slot->offset_scroll_speed, slot->scaler, slot->char_space, slot->text);
  }
}

void preview_slot(struct Slot *slot) {
  if (!slot->enabled) return;
  tx_len += snprintf((char *)tx_buf + tx_len, TX_BUFFER_SIZE - tx_len,
                     "%d: ", slot->slot_no);
  if (slot->text_type != NO_TEXT) {
    for (uint8_t j = 0; j < 8; j++) {
      tx_len++;
      tx_buf[tx_len] = slot->text[j];
      if (tx_buf[tx_len] == 0) {
        tx_buf[tx_len++] = '\r';
        tx_buf[tx_len++] = '\n';
        break;
      } else if (j == 7) {
        if (tx_buf[tx_len] == 195) tx_len--;
        tx_buf[++tx_len] = '\r';
        tx_buf[++tx_len] = '\n';
      }
    }
  } else {
    tx_len +=
        snprintf((char *)tx_buf + tx_len, TX_BUFFER_SIZE - tx_len, "-\r\n");
  }
}
