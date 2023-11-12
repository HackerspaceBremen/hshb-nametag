#include <avr/io.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "uart.h"

uint8_t tx_len=0;
volatile uint8_t tx_pos=0;
volatile uint8_t rx_len=0;

volatile uint8_t uart_rx=0;
volatile uint8_t uart_tx=0;

void handleUART() {
	if(!uart_rx && (UCSRA & (1 << RXC))) { // UART Available
		rx_buf[rx_len]=UDR;
		if(rx_buf[rx_len]=='\n') {
			if(rx_buf[rx_len-1]=='\r') rx_len--;
			rx_buf[rx_len]=0;
			uart_rx=1;
		} else {
			rx_len++;
		}
		if(rx_len>=RX_BUFFER_SIZE) uart_rx=1;
	}
	
	if(uart_tx && (UCSRA & (1 << UDRE))) {
		UDR=tx_buf[tx_pos];
		tx_pos++;
		if(tx_pos>=tx_len) {
			tx_pos=0;
			uart_tx=0;
		}
	}
}

uint8_t uartBusy() {
	return uart_tx;
}

uint8_t uartAvailable() {
	return uart_rx*rx_len;
}

/*uint8_t uartRead(char* buf, uint8_t buf_size) {
	uint8_t l = rx_len;
	if (rx_len < buf_size) {
		memcpy(buf, rx_buf, rx_len+1);
	} else {
		memcpy(buf, rx_buf, buf_size);
		l=buf_size;
	}
	rx_len=0;
	uart_rx=0;
	return l;
}*/

/*uint8_t uartWrite(const char *flashStr) {
	if(!uart_tx) {
		uint8_t len = 0;
		while(len<130 && (tx_buf[len] = pgm_read_byte(flashStr + len)) != '\0') len++;
		tx_len=len;
		uart_tx=1;
		return 1;
	}
	return 0;
}*/


uint8_t uartWriteLn(const char *flashStr) {
	if(!uart_tx) {
		uint8_t len = 0;
		while(len<TX_BUFFER_SIZE-1 && (tx_buf[len] = pgm_read_byte(flashStr + len)) != '\0') len++;
		len++;
		tx_buf[len]='\r';
		tx_buf[len+1]='\n';
		tx_len=len+2;
		uart_tx=1;
		return 1;
	}
	return 0;
}