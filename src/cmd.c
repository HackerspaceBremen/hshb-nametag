#include "cmd.h"

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

#include "animations.h"
#include "display.h"
#include "slots.h"
#include "uart.h"

// CMD HANDLE: FOR INVALID RECEIVED CMD
static void cmd_invalid_command() {
  uart_writeln_flash_str(
      F("Invalid command.\r\nB: Brightness\r\nD: Disable\r\nE: Enable\r\nF: "
        "Format\r\nH: Help\r\nI: Idenfity\r\nL: List\r\nR: Read\r\nW: Write"));
}

// HELPER TO PARSE INTEGER INPUT VALUES FROM CMD MESSAGE STRING
static uint8_t read_number(uint8_t *msg_pos, uint8_t *err,
                           uint8_t last_number) {
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

// HELPER TO EN-/DISABLE A SLOT
static void set_slot_enabled(uint8_t slot_no, uint8_t enabled) {
  struct Slot slot;
  slot.slot_no = slot_no;
  slot_load(&slot);
  slot.enabled = enabled;
  slot_save(&slot);
}

// CMD HANDLE: 'B' SET OVERALL BASE-BRIGHTNESS OF DEVICE (BRIGHT/DARK VALUES)
static void cmd_set_brightness() {
  uint8_t msg_pos = 2;
  uint8_t err = 0;
  uint8_t values[4] = {0, 0, 0, 0};

  if (msg_pos >= rx_len || rx_buf[1] != ' ') err = 1;
  for (uint8_t i = 0; i < 4; i++) {
    values[i] = read_number(&msg_pos, &err, i == 3);
  }

  if (err) {
    // tx_len = snprintf((char *)tx_buf, TX_BUFFER_SIZE, "B %d %d %d %d\r\n",
    // values[0],values[1],values[2],values[3]);

    uart_writeln_flash_str(
        F("B <LogoBright> <LogoDark> <TextBright> <TextDark>"));

  } else {
    display_state.animation_brightness_on = values[0];
    display_state.animation_brightness_off = values[1];
    display_state.text_brightness_on = values[2];
    display_state.text_brightness_off = values[3];
    tx_len = snprintf((char *)tx_buf, TX_BUFFER_SIZE, "B %d %d %d %d\r\n",
                      display_state.animation_brightness_on,
                      display_state.animation_brightness_off,
                      display_state.text_brightness_on,
                      display_state.text_brightness_off);
    uart_enable_tx();
  }
}

// CMD HANDLE: 'W' WRITE
static void cmd_write_slot() {
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
  if (err || msg_pos >= rx_len || saveSlot.animation >= INVALID_ANIMATION) {
    uart_writeln_flash_str(
        F("W <SlotId> <AnimationId> <Seconds/Scrolls> <TextTypeId> "
          "<Offset X/Scroll Speed> <CharScale> <CharSpace> <Text>"));
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

// CMD HANDLE: 'H' SHOW HELP OF AVAILABLE COMMANDS
static void cmd_show_help() {
  uart_writeln_flash_str(F("\r\n\r\n*** HELP ON COMMANDS ***\r\n"));
  _delay_ms(500);  // ATTN: HACK TO WAIT FOR SEND BUFFER TO BE SENT
  uart_writeln_flash_str(F(
      "B <LogoBright> <LogoDark> <TextBright> <TextDark>\r\nSET BRIGHTNESS (0-255) \
      \r\n\r\nD <SlotId>\r\nDISABLE SLOT \
      \r\n\r\nE <SlotId>\r\nENABLE SLOT \
      \r\n\r\nF <SlotId/All>\r\nFORMAT/DELETE SLOT(S)"));
  _delay_ms(500);  // ATTN: HACK TO WAIT FOR SEND BUFFER TO BE SENT
  uart_writeln_flash_str(
      F("\r\nI\r\nIDENTIFY BOARD AND FIRMWARE/BUILD \
    \r\n\r\nH\r\nPRINT THIS HELP \
    \r\n\r\nL\r\nLIST ALL SLOT ANIMATIONS \
    \r\n\r\nR <SlotId>\r\nREAD ALL VALUES FOR A SLOT"));
  _delay_ms(500);  // ATTN: HACK TO WAIT FOR SEND BUFFER TO BE SENT
  uart_writeln_flash_str(
      F("\r\nW <SlotId> <AnimationId> <Seconds/Scrolls> <TextTypeId>\r\n \
  <Offset X/Scroll Speed> <CharScale> <CharSpace> <Text>\r\nWRITE PROVIDED VALUES FOR A SLOT\r\n"));
}

// CMD HANDLE: 'R' READ
static void cmd_read_slot() {
  uint8_t msg_pos = 2;
  uint8_t err = 0;
  struct Slot readSlot;
  readSlot.slot_no = read_number(&msg_pos, &err, 1);

  if (!err) {
    slot_load(&readSlot);
    slot_print(&readSlot);
    uart_enable_tx();
  } else {
    uart_writeln_flash_str(F("R <SlotId>"));
  }
}

// CMD HANDLE: 'D' DISABLE
static void cmd_disable_slot() {
  if (rx_len == 3 && rx_buf[1] == ' ' && rx_buf[2] >= '0' && rx_buf[2] <= '9') {
    uint8_t slot_no = (rx_buf[2] - '0');
    set_slot_enabled(slot_no, 0);
    tx_len = snprintf((char *)tx_buf, TX_BUFFER_SIZE, "Slot %d disabled.\r\n",
                      slot_no);
  } else {
    uart_writeln_flash_str(F("D <SlotId>"));
  }
  uart_enable_tx();
}

// CMD HANDLE: 'E' ENABLE
static void cmd_enable_slot() {
  if (rx_len == 3 && rx_buf[1] == ' ' && rx_buf[2] >= '0' && rx_buf[2] <= '9') {
    uint8_t slot_no = (rx_buf[2] - '0');
    set_slot_enabled(slot_no, 1);
    tx_len = snprintf((char *)tx_buf, TX_BUFFER_SIZE, "Slot %d enabled.\r\n",
                      slot_no);
  } else {
    uart_writeln_flash_str(F("E <SlotId>"));
  }
  uart_enable_tx();
}

// CMD HANDLE: 'F' FORMAT (CLEARS ALL SLOTS)
static void cmd_format_slots() {
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
      uart_writeln_flash_str(F("F <SlotId/All>"));
    }
    uart_enable_tx();
  }
}

// CMD HANDLE: 'L' LIST ALL SLOTS
static void cmd_list_slots() {
  tx_len = 0;
  struct Slot slot;
  for (uint8_t i = 0; i < SLOT_NUM_MAX; i++) {
    slot.slot_no = i;
    slot_load(&slot);
    slot_preview(&slot);
  }
  if (tx_len == 0) {
    uart_writeln_flash_str(F("No Data..."));
  }
  uart_enable_tx();
}

// CMD HANDLE: 'I' IDENTIFY BOARD NUMBER & BUILD VERSION
static void cmd_identify_board() {
  uart_writeln_flash_str(
      F("{ \"board_id\" : \"" STR(BOARD_ID) "\" , \"build\" : \"" STR(
          BUILD_VERSION) "\", \"firmware\" : \"" STR(FIRMWARE_VERSION) "\" "
                                                                       "}"));
}

struct command {
  const char letter;
  uint8_t takes_arguments;
  void (*function)();
};

// DEFINES LIST OF AVAILBLE SERIAL COMMANDS TO BE RECEIVED VIA UART
static struct command AVAILABLE_COMMANDS[] = {
    {.letter = 'B', .takes_arguments = 1, .function = cmd_set_brightness},
    {.letter = 'D', .takes_arguments = 1, .function = cmd_disable_slot},
    {.letter = 'E', .takes_arguments = 1, .function = cmd_enable_slot},
    {.letter = 'F', .takes_arguments = 1, .function = cmd_format_slots},
    {.letter = 'H', .takes_arguments = 0, .function = cmd_show_help},
    {.letter = 'I', .takes_arguments = 0, .function = cmd_identify_board},
    {.letter = 'L', .takes_arguments = 0, .function = cmd_list_slots},
    {.letter = 'R', .takes_arguments = 1, .function = cmd_read_slot},
    {.letter = 'W', .takes_arguments = 1, .function = cmd_write_slot},
    // Experimental commands:

    // End of list
    {.letter = '\0', .takes_arguments = 0, .function = cmd_invalid_command},
};

// CMD HANDLE: ANALYZE INCOMING CMD STRING AND TRIGGER CORRESPONDING FUNCTIONS
void handle_command_input() {
  if (uart_rx) {
    uint8_t cmd = 0;
    for (cmd = 0; AVAILABLE_COMMANDS[cmd].letter != '\0'; cmd++) {
      if (AVAILABLE_COMMANDS[cmd].takes_arguments) {
        if (rx_len == 1) continue;
      } else {
        if (rx_len > 1) continue;
      }
      if (toupper(rx_buf[0]) == AVAILABLE_COMMANDS[cmd].letter) {
        AVAILABLE_COMMANDS[cmd].function();
        break;
      }
    }

    if (AVAILABLE_COMMANDS[cmd].letter == '\0') {
      cmd_invalid_command();
    }

    uart_rx = 0;
    rx_len = 0;
  }
}
