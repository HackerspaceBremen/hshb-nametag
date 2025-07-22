#include "timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "adc.h"
#include "charlieplex.h"
#include "display.h"
#include "uart.h"

volatile uint32_t global_millis = 0;
volatile uint32_t global_counter = 0;

volatile uint8_t adc_timer_counter = 0;
volatile uint8_t scroll_timer_counter = 0;

void timer_init() {
  TCCR1B = (1 << WGM12) | (1 << CS11);  // CTC Mode
  TIMSK = (1 << OCIE1A);                // Enable Comp Match Interrupts

  OCR1A = 10000;
}

ISR(TIMER1_COMPA_vect) {
  refreshDisplay();

  if (UCSRA & (1 << RXC)) uart_handle_rx();  // Check UART

  scroll_timer_counter++;

  if (scroll_timer_counter >= display_state.scroll_interval) {
    scroll_timer_counter = 0;
    display_scroll();
  }

  adc_timer_counter++;
  if (adc_timer_counter >= 100) {
    adc_timer_counter = 0;
    // Voltage Divider On (active low)
    PORTA &= ~(1 << 6);
    adc_enable();                              // Enable ADC
    if (UCSRA & (1 << RXC)) uart_handle_rx();  // Check UART
    _delay_us(600);
    adc_start_conversion();
  }

  global_millis += 10;
  global_counter += 1;
}
