#ifndef UART_H_
#define UART_H_

#define TX_BUFFER_SIZE 150
char tx_buf[150];
uint8_t tx_len;
volatile uint8_t tx_pos;
#define RX_BUFFER_SIZE 100
volatile char rx_buf[100];
volatile uint8_t rx_len;

volatile uint8_t uart_rx;
volatile uint8_t uart_tx;

void handleUART();

uint8_t uartCheck();

//uint8_t uartRead(char* buf, uint8_t buf_size);
//uint8_t uartWrite(const char *flashStr);
uint8_t uartWriteLn(const char *flashStr);

uint8_t uartBusy();
uint8_t uartAvailable();

#endif // UART_H_