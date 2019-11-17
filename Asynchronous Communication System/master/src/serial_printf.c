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

#define	BAUD 57600                   // Desired baud rate
#define BAUDGEN ((16000000/(16*BAUD))-1)  // Calculated divider (may under/overflow for some cases)

void usart_init(void) 
{
  UBRR0 = BAUDGEN;
  UCSR0A = (1 << MPCM0);
  UCSR0B = (1 << TXEN0) | (1 << UCSZ02); // enable  transmit
  UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);// 2 stop bits USBS0, 9 bit mode  UCSZ0
}

void USART_Transmit(unsigned int data)
{
/* Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)))
;
/* Copy 9th bit to TXB8 */
UCSR0B &= ~(1<<TXB80);
if (data & 0x0100)
UCSR0B |= (1<<TXB80);
/* Put data into buffer, sends the data */
UDR0 = data;
}


unsigned int USART_Receive( void )
{
unsigned char status, resh, resl;
/* Wait for data to be received */
while (!(UCSR0A & (1<<RXC0)))
;
/* Get status and 9th bit, then data */
/* from buffer */
status = UCSR0A;
resh = UCSR0B;
resl = UDR0;
/* If error, return -1 */
if (status & ((1<<FE0) | (1<<DOR0) | (1<<UPE0)))
{return -1;}
/* Filter the 9th bit, then return */
resh = (resh >> 1) & 0x01;
return ((resh << 8) | resl);
}

int usart_putchar(char c, FILE *stream) 
{
  while (!( UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = c;
  return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar, NULL, _FDEV_SETUP_WRITE);

void printf_init(void) 
{
  usart_init();
  stdout = &mystdout;
}

void serial_send(uint8_t data)
{
  // Wait for empty transmit buffer
  while (!( UCSR0A & (1<<UDRE0)));
  // Put data into buffer, sends the data */
  UDR0 = data;
}

uint8_t serial_receive(void)
{
  // Wait for data to be received
  while (!(UCSR0A & (1<<RXC0)));
  // Get and return received data from buffer
  return UDR0;
}

uint8_t serial_receive_ready(void)
{
  // check if there is data to be received
  return (UCSR0A & (1<<RXC0));
}