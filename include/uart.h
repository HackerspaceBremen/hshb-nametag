#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#define TX_BUFFER_SIZE 150
#define RX_BUFFER_SIZE 100

extern volatile char tx_buf[TX_BUFFER_SIZE];
extern volatile uint8_t tx_len;
extern volatile char rx_buf[RX_BUFFER_SIZE];
extern volatile uint8_t rx_len;

extern volatile uint8_t uart_rx;

void uart_initialize();
void uart_enable();
void uart_disable();

void uart_handle_rx();
uint8_t uart_writeln_flash_str(const char *flashStr);
void uart_enable_tx();

#endif
