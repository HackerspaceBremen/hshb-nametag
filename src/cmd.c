#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "display.h"
#include "slots.h"
#include "uart.h"

#define F(string_literal) (PSTR(string_literal))

uint8_t read_number(uint8_t *msg_pos, uint8_t *err, uint8_t last_number) {
  uint8_t ret = 0;
  if (!isdigit(rx_buf[*msg_pos])) {
    *err = 1;
    return ret;
  }
  while (isdigit(rx_buf[*msg_pos])) {
    ret = ret * 10 + (rx_buf[*msg_pos] - '0');
    (*msg_pos)++;

    if (*msg_pos >= rx_len) {
      if (!last_number) *err = 1;
      return ret;
    }
  }
  (*msg_pos)++;
  return ret;
}

void cmd_write_slot() {
  struct Slot saveSlot;
  saveSlot.enabled = 1;
  uint8_t msg_pos = 2;
  uint8_t err = 0;
  if (msg_pos >= rx_len || rx_buf[1] != ' ') err = 1;
  saveSlot.slot_no = read_number(&msg_pos, &err, 0);
  saveSlot.animation = read_number(&msg_pos, &err, 0);
  saveSlot.seconds_scrolls = read_number(&msg_pos, &err, 0);
  saveSlot.text_type = read_number(&msg_pos, &err, 0);
  saveSlot.offset_scroll_speed = read_number(&msg_pos, &err, 0);
  saveSlot.scaler = read_number(&msg_pos, &err, 0);
  saveSlot.char_space = read_number(&msg_pos, &err, 0);
  if (err || msg_pos >= rx_len) {
    uart_writeln_flash_str(
        F("W <Slot> <Animation> <Seconds/Scrolls> <Text Type> "
          "<Offset/Scroll Speed> <Scaler> <Char Space> <Text>"));
    return;
  }

  for (uint8_t i = msg_pos; i < rx_len && i < SLOT_TEXT_BUFFER_SIZE; i++) {
    saveSlot.text[i - msg_pos] = rx_buf[i];
  }
  saveSlot.text[rx_len - msg_pos] = 0;
  slot_save(&saveSlot);
  slot_print(&saveSlot);
  uart_enable_tx();
}

void cmd_read_slot() {
  uint8_t msg_pos = 2;
  uint8_t err = 0;
  struct Slot readSlot;
  readSlot.slot_no = read_number(&msg_pos, &err, 1);

  if (!err) {
    slot_load(&readSlot);
    slot_print(&readSlot);
    uart_enable_tx();
  } else {
    uart_writeln_flash_str(F("R <Slot>"));
  }
}

void set_slot_enabled(uint8_t slot_no, uint8_t enabled) {
  struct Slot slot;
  slot.slot_no = slot_no;
  slot_load(&slot);
  slot.enabled = enabled;
  slot_save(&slot);
}

void cmd_disable_slot() {
  if (rx_len == 3 && rx_buf[1] == ' ' && rx_buf[2] >= '0' && rx_buf[2] <= '9') {
    uint8_t slot_no = (rx_buf[2] - '0');
    set_slot_enabled(slot_no, 0);
    tx_len = snprintf((char *)tx_buf, TX_BUFFER_SIZE, "Slot %d disabled.\r\n",
                      slot_no);
  } else {
    uart_writeln_flash_str(F("D <Slot>"));
  }
  uart_enable_tx();
}

void cmd_enable_slot() {
  if (rx_len == 3 && rx_buf[1] == ' ' && rx_buf[2] >= '0' && rx_buf[2] <= '9') {
    uint8_t slot_no = (rx_buf[2] - '0');
    set_slot_enabled(slot_no, 1);
    tx_len = snprintf((char *)tx_buf, TX_BUFFER_SIZE, "Slot %d enabled.\r\n",
                      slot_no);
  } else {
    uart_writeln_flash_str(F("E <Slot>"));
  }
  uart_enable_tx();
}

void cmd_format_slots() {
  if (rx_len >= 3 && rx_buf[1] == ' ') {
    if (rx_buf[2] >= '0' && rx_buf[2] <= '9') {
      uint8_t slot = (rx_buf[2] - '0');
      for (int16_t i = slot * SLOT_EEPROM_SIZE;
           i < slot * SLOT_EEPROM_SIZE + SLOT_EEPROM_SIZE; i++) {
        eeprom_write_byte((uint8_t *)(i), (uint8_t)0);
      }
      tx_len =
          snprintf((char *)tx_buf, TX_BUFFER_SIZE, "Slot %d cleared\r\n", slot);
    } else if ((rx_buf[2] == 'A' || rx_buf[2] == 'a') &&
               (rx_buf[3] == 'L' || rx_buf[3] == 'l') &&
               (rx_buf[4] == 'L' || rx_buf[4] == 'l')) {
      for (uint16_t i = 0; i < 1024; i++) {
        eeprom_write_byte((uint8_t *)(i), (uint8_t)0);
      }
      tx_len = snprintf((char *)tx_buf, TX_BUFFER_SIZE, "Memory cleared\r\n");
    } else {
      uart_writeln_flash_str(F("F <Slot/All>"));
    }
    uart_enable_tx();
  }
}

void cmd_list_slots() {
  tx_len = 0;
  struct Slot slot;
  for (uint8_t i = 0; i < SLOT_MAX; i++) {
    slot.slot_no = i;
    slot_load(&slot);
    slot_preview(&slot);
  }
  if (tx_len == 0) {
    uart_writeln_flash_str(F("No Data..."));
  }
  uart_enable_tx();
}

void cmd_set_brightness() {
  uint8_t msg_pos = 2, err = 0;
  uint8_t values[4] = {0, 0, 0, 0};

  for (uint8_t i = 0; i < 4; i++) {
    values[i] = read_number(&msg_pos, &err, i == 3);
  }

  if (err) {
    uart_writeln_flash_str(
        F("B <Logo Bright> <Logo Dark> <Text Bright> <Text Dark>"));
  } else {
    display_state.animation_brightness_on = values[0];
    display_state.animation_brightness_off = values[1];
    display_state.text_brightness_on = values[2];
    display_state.text_brightness_off = values[3];
  }
}

void cmd_invalid_command() {
  uart_writeln_flash_str(
      F("Invalid command.\r\nW: Write\r\nR: Read\r\nE: Enable\r\nD: "
        "Disable\r\nL: List\r\nF: Format\r\nB: Brightness"));
}

void handle_command_input() {
  if (uart_rx) {
    if (rx_buf[0] == 'W' || rx_buf[0] == 'w') {
      cmd_write_slot();
    } else if (rx_buf[0] == 'R' || rx_buf[0] == 'r') {
      cmd_read_slot();
    } else if (rx_buf[0] == 'D' || rx_buf[0] == 'd') {
      cmd_disable_slot();
    } else if (rx_buf[0] == 'E' || rx_buf[0] == 'e') {
      cmd_enable_slot();
    } else if (rx_buf[0] == 'F' || rx_buf[0] == 'f') {
      cmd_format_slots();
    } else if (rx_len == 1 && (rx_buf[0] == 'L' || rx_buf[0] == 'l')) {
      cmd_list_slots();
    } else if (rx_buf[0] == 'B' || rx_buf[0] == 'b') {
      cmd_set_brightness();
    } else {
      cmd_invalid_command();
    }

    uart_rx = 0;
    rx_len = 0;
  }
}
