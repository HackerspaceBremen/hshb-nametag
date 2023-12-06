#include "uart.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#define BAUD 9600

// Calculate the UBRR value for the desired baud rate
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1)

volatile uint8_t tx_len = 0;
static volatile uint8_t tx_pos = 0;
volatile uint8_t rx_len = 0;
volatile char tx_buf[TX_BUFFER_SIZE];
volatile char rx_buf[RX_BUFFER_SIZE];

volatile uint8_t uart_rx = 0;

void uart_initialize() {
  UCSRB |= 1 << RXCIE;  // Enable RX interrupt

  // Set baud rate
  UBRRH = (unsigned char)(UBRR_VAL >> 8);
  UBRRL = (unsigned char)UBRR_VAL;
  // Set frame format: 8data, 1stop bit
  UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

void uart_enable() { UCSRB |= 1 << RXEN | 1 << TXEN | 1 << RXCIE; }

void uart_disable() { UCSRB &= ~(1 << RXEN | 1 << TXEN); }

void uart_handle_rx() {
  if (!uart_rx) {  // UART still receiving
    rx_buf[rx_len] = UDR;
    if (rx_buf[rx_len] == '\n' || rx_buf[rx_len] == '\r') {
      rx_buf[rx_len] = 0;
      uart_rx = 1;
    } else {
      if (rx_len >= RX_BUFFER_SIZE) {
        uart_rx = 1;
      } else {
        rx_len++;
      }
    }
  } else {
    UDR;  // Read from UDR to clear RXC
  }
}

uint8_t uart_writeln_flash_str(const char *flashStr) {
  // Only callable if UART TX interrupt is currently not enabled (not writing to
  // UART)
  if (!(UCSRB & (1 << UDRIE))) {
    uint8_t len = 0;
    // Reserve 2 to add \r\n
    while (len < TX_BUFFER_SIZE - 2) {
      tx_buf[len] = pgm_read_byte(flashStr + len);
      if (tx_buf[len] == '\0') {
        break;
      }
      len++;
    }
    tx_buf[len] = '\r';
    tx_buf[len + 1] = '\n';
    tx_len = len + 2;
    uart_enable_tx();
    return 1;
  }
  return 0;
}

void uart_enable_tx() {
  UCSRB |= (1 << UDRIE);  // Enable UART TX interrupt
}

static void uart_disable_tx() {
  tx_pos = 0;
  UCSRB &= ~(1 << UDRIE);  // Disable UART TX interrupt
}

ISR(USART_RXC_vect) {
  uart_handle_rx();  // Read UART
}

ISR(USART_UDRE_vect) {
  UDR = tx_buf[tx_pos++];
  if (tx_pos >= tx_len) {
    // Finished sending
    uart_disable_tx();
  }
}
