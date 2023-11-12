#include "uart.h"

#include <avr/io.h>
#include <avr/pgmspace.h>

uint8_t tx_len = 0;
volatile uint8_t tx_pos = 0;
volatile uint8_t rx_len = 0;
char tx_buf[150];
volatile char rx_buf[100];

volatile uint8_t uart_rx = 0;
volatile uint8_t uart_tx = 0;

void handleUART() {
  if (!uart_rx && (UCSRA & (1 << RXC))) {  // UART Available
    rx_buf[rx_len] = UDR;
    if (rx_buf[rx_len] == '\n') {
      if (rx_buf[rx_len - 1] == '\r') rx_len--;
      rx_buf[rx_len] = 0;
      uart_rx = 1;
    } else {
      rx_len++;
    }
    if (rx_len >= RX_BUFFER_SIZE) uart_rx = 1;
  }

  if (uart_tx && (UCSRA & (1 << UDRE))) {
    UDR = tx_buf[tx_pos];
    tx_pos++;
    if (tx_pos >= tx_len) {
      tx_pos = 0;
      uart_tx = 0;
    }
  }
}

uint8_t uartWriteLn(const char *flashStr) {
  if (!uart_tx) {
    uint8_t len = 0;
    while (len < TX_BUFFER_SIZE - 1 &&
           (tx_buf[len] = pgm_read_byte(flashStr + len)) != '\0')
      len++;
    len++;
    tx_buf[len] = '\r';
    tx_buf[len + 1] = '\n';
    tx_len = len + 2;
    uart_tx = 1;
    return 1;
  }
  return 0;
}
