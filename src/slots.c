#include "slots.h"

#include <avr/eeprom.h>
#include <stdio.h>

#include "animations.h"
#include "display.h"
#include "timer.h"
#include "uart.h"

struct Slot current_slot = {
    .slot_no = SLOT_MAX,
    .slot_loaded_millis = 0,
    .scroll_counter = 0,
    .last_animation = INVALID_ANIMATION,
};

uint8_t *get_base_address(uint8_t slot_no) {
  return (uint8_t *)(slot_no * SLOT_EEPROM_SIZE);
}

void slot_load(struct Slot *slot) {
  if (slot->slot_no >= SLOT_MAX) return;
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

void slot_save(struct Slot *slot) {
  if (slot->slot_no >= SLOT_MAX) return;
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

void slot_print(struct Slot *slot) {
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

void slot_preview(struct Slot *slot) {
  if (!slot->enabled) return;
  tx_len += snprintf((char *)tx_buf + tx_len, TX_BUFFER_SIZE - tx_len,
                     "%d: ", slot->slot_no);
  if (slot->text_type != NO_TEXT) {
    for (uint8_t j = 0; j < 8; j++) {
      tx_buf[tx_len] = slot->text[j];
      tx_len++;
      if (tx_buf[tx_len - 1] == 0) {
        tx_buf[tx_len - 1] = '\r';
        tx_buf[tx_len++] = '\n';
        break;
      } else if (j == 7) {
        if (tx_buf[tx_len - 1] == 195) tx_len--;
        tx_buf[tx_len - 1] = '\r';
        tx_buf[tx_len++] = '\n';
      }
    }
  } else {
    tx_len +=
        snprintf((char *)tx_buf + tx_len, TX_BUFFER_SIZE - tx_len, "-\r\n");
  }
}

void slot_advance() {
  display_scroll_stop();
  uint8_t attempts = 0;
  while (1) {
    current_slot.slot_no++;
    if (current_slot.slot_no >= SLOT_MAX) {
      current_slot.slot_no = 0;
    }
    slot_load((struct Slot *)&current_slot);
    if (current_slot.enabled) {
      break;
    }

    attempts++;
    if (attempts >= SLOT_MAX) {
      // No active slots found.
      break;
    }
  }
  if (current_slot.last_animation == INVALID_ANIMATION) {
    // Initialize last_animation to prevent flickering
    current_slot.last_animation = current_slot.animation;
  }

  current_slot.slot_loaded_millis = global_millis;
  if (!current_slot.enabled) {
    // No enabled slot was found
    clear_vram();
    display_write_text(7, "No Data", 1, 2, 5, 0);
  } else {
    if (current_slot.animation != current_slot.last_animation) {
      clear_vram();
      resetAnimations(0);
    }
    current_slot.last_animation = current_slot.animation;
  }
}

void slot_handle() {
  uint8_t advance_slot = 0;
  if (current_slot.enabled) {
    switch (current_slot.animation) {
      case LOGO_ON:
        for (uint16_t k = LOGO_START; k < LOGO_END; k++)
          vRAM[k] = display_state.animation_brightness_on;
        break;
      case MATRIX:
        matrixAnimation();
        break;
      case SWEEP:
        sweepAnimation(32, 1);
        break;
      case WAVE:
        waveAnimation();
        break;
      case LINES:
        linesAnimation(0);
        break;
      case LINES_FILLED:
        linesAnimation(1);
        break;
      case ROTATE:
        rotateAnimation(0);
        break;
      case ROTATE_FILLED:
        rotateAnimation(1);
        break;
      case CIRCLES:
        circlesAnimation();
        break;
      default:  // Includes LOGO_OFF
        for (uint16_t k = LOGO_START; k < LOGO_END; k++)
          vRAM[k] = display_state.animation_brightness_off;
        break;
    }

    switch (current_slot.text_type) {
      case STATIC:
        display_write_text(current_slot.offset_scroll_speed, current_slot.text,
                           current_slot.scaler, current_slot.char_space,
                           display_state.text_brightness_on,
                           display_state.text_brightness_off);
        if (current_slot.seconds_scrolls &&
            (global_millis - current_slot.slot_loaded_millis) / 1000 >=
                current_slot.seconds_scrolls)
          advance_slot = 1;
        break;
      case SCROLL:
        if (!display_scrolling()) {
          current_slot.scroll_counter++;
          if (current_slot.seconds_scrolls &&
              current_slot.scroll_counter > current_slot.seconds_scrolls) {
            display_state.scroll_interval = 0;
            advance_slot = 1;
          } else {
            clear_vram_text();
            if (current_slot.offset_scroll_speed) {
              display_state.scroll_interval = current_slot.offset_scroll_speed;
            } else {
              display_state.scroll_interval = 2;
            }
            display_write_scroll(current_slot.text, current_slot.scaler,
                                 current_slot.char_space,
                                 display_state.text_brightness_on,
                                 display_state.text_brightness_off);
          }
        }
        break;
      default:
        clear_vram_text();
        if (current_slot.seconds_scrolls &&
            (global_millis - current_slot.slot_loaded_millis) / 1000 >=
                current_slot.seconds_scrolls)
          advance_slot = 1;
        break;
    }

  } else {
    if (global_millis - current_slot.slot_loaded_millis > 1000)
      advance_slot = 1;
  }

  if (advance_slot) slot_advance();
}
