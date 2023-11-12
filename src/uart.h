#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#define TX_BUFFER_SIZE 150
extern char tx_buf[150];
extern uint8_t tx_len;
extern volatile uint8_t tx_pos;
#define RX_BUFFER_SIZE 100
extern volatile char rx_buf[100];
extern volatile uint8_t rx_len;

extern volatile uint8_t uart_rx;
extern volatile uint8_t uart_tx;

void handleUART();

uint8_t uartWriteLn(const char *flashStr);

#endif  // UART_H_
