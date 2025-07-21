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

#define F(string_literal) (PSTR(string_literal))

// See https://stackoverflow.com/a/5459929
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define FIRMWARE_VERSION_STR "1.0.0";

// CMD HANDLE: FOR INVALID RECEIVED CMD
static void cmd_invalid_command() {
  uart_writeln_flash_str(
      F("Invalid command.\r\nW: Write\r\nR: Read\r\nE: Enable\r\nD: "
        "Disable\r\nL: List\r\nF: Format\r\nI: Idenfity\r\nB: Brightness"));
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

// CMD HANDLE: 'H' SHOW HELP OF AVAILABLE COMMANDS
static void cmd_show_help() {
    uart_writeln_flash_str(
    F("B <Logo Bright> <Logo Dark> <Text Bright> <Text Dark>\nSET BRIGHTNESS \
      \n\nD <SlotId>\nDISABLE SLOT \
      \n\nE <SlotId>\nENABLE SLOT \
      \n\nF <SlotId/All>\nFORMAT/DELETE SLOT(S)"
    ));
    _delay_ms(500); // ATTN: HACK TO WAIT FOR SEND BUFFER TO BE SENT
    uart_writeln_flash_str(
    F("\nI\nIDENTIFY BOARD AND FIRMWARE/BUILD \
    \n\nH\nTHIS HELP \
    \n\nL\nLIST ALL SLOTS \
    \n\nR <SlotId>\nREAD ALL VALUES FOR A SLOT"
    ));
    _delay_ms(500); // ATTN: HACK TO WAIT FOR SEND BUFFER TO BE SENT
    uart_writeln_flash_str(
      F("\nW <SlotId> <AnimationId> <Seconds/Scrolls> <TextTypeId> \
      <Offset X/Scroll Speed> <CharScale> <CharSpace> <Text>\nWRITE PROVIDED VALUES FOR A SLOT"
    ));
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
    uart_writeln_flash_str(F("R <Slot>"));
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
    uart_writeln_flash_str(F("D <Slot>"));
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
    uart_writeln_flash_str(F("E <Slot>"));
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
      uart_writeln_flash_str(F("F <Slot/All>"));
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

// CMD HANDLE: 'B' SET OVERALL BASE-BRIGHTNESS OF DEVICE (BRIGHT/DARK VALUES)
static void cmd_set_brightness() {
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

// CMD HANDLE: 'I' IDENTIFY BOARD NUMBER & BUILD VERSION
static void cmd_identify_board() {
  if( sizeof( STR(BUILD_VERSION) ) == 1 ) {
  uart_writeln_flash_str(
      F("Board ID: " STR(BOARD_ID) "\nFirmware version: " STR(FIRMWARE_VERSION_STR) )
  );
  }
  else {
    uart_writeln_flash_str(
      F("Board ID: " STR(BOARD_ID) "\nBuild version: " STR(BUILD_VERSION))
    );
  }
}

struct command {
  const char letter;
  uint8_t takes_arguments;
  void (*function)();
};

// DEFINES LIST OF AVAILBLE SERIAL COMMANDS TO BE RECEIVED VIA UART
static struct command AVAILABLE_COMMANDS[] = {
    {.letter = 'W', .takes_arguments = 1, .function = cmd_write_slot},
    {.letter = 'R', .takes_arguments = 1, .function = cmd_read_slot},
    {.letter = 'D', .takes_arguments = 1, .function = cmd_disable_slot},
    {.letter = 'E', .takes_arguments = 1, .function = cmd_enable_slot},
    {.letter = 'F', .takes_arguments = 1, .function = cmd_format_slots},
    {.letter = 'L', .takes_arguments = 0, .function = cmd_list_slots},
    {.letter = 'I', .takes_arguments = 0, .function = cmd_identify_board},
    {.letter = 'H', .takes_arguments = 0, .function = cmd_show_help},

    // Experimental commands:
    {.letter = 'B', .takes_arguments = 1, .function = cmd_set_brightness},

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