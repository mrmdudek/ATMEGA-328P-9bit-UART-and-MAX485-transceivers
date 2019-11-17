/*****************************************************************************
 * serial_printf.c
 *  Redirection of the printf stream to the AVR serial port
 *    1. Initialize usart @ 57600 bps (change in BAUD)
 *    2. Define the low level put_char mechanism
 *    3. Redirect the printf io stream
 *
 *  Created on: 13/09/2016
 *      Author: jpsousa@fe.up.pt (gcc-avr)
 *      paco@f.up.pt (some cosmetic changes)
 *****************************************************************************/

#include <stdio.h>
#include <avr/io.h>

#define	BAUD 57600                    // Desired baud rate
#define BAUDGEN ((F_CPU/(16*BAUD))-1)  // Calculated divider (may under/overflow for some cases)

void usart_init(void) 
{
  UBRR0 = BAUDGEN;
  UCSR0B = (1 << RXEN0) | (1 << RXCIE0) | (1 << UCSZ02); // enable recive , enable recive interrupt, 9 bit mode
  UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);// 2 stop bits USBS0, 9 bit mode 7 << UCSZ00,
}

