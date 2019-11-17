#include <avr/io.h>
#include <avr/interrupt.h>
#include "serial_printf.h"
#include <inttypes.h>

#define LED 1 // LED connected to PB0

uint8_t slave_address; // slave address, read from DIP buttons

uint8_t slave_data_turn_on = 0b11111111;//if this data turn led on
uint8_t slave_data_turn_off = 0b0000000;//if this data turn led off

void io_init(void)
{
 	DDRB |= 0b1; // led is output

	// DIP Buttons connected to PD 0,1,2,3
	DDRC |= 0b0000; // DIP buttons are inputs, needed for set slave address
	PORTC |= 0b1111; // pull-up DIP buttons
}

uint8_t check_address (void)//check slave address
{
	return (PINC & 0b00001111); // return 0000xxxx, where xxxx is DIP button state
}


ISR(USART_RX_vect)        //interrupt if recive data 
{ 
unsigned char resh, resl;
static uint8_t is_it_my_address = 0;// wait for this address

/* Get status and 9th bit, then data */
/* from buffer */
resh = UCSR0B;
resl = UDR0;

resh = (resh >> 1) & 0x01;

   if (is_it_my_address == 0) // Go there if slave wait for address
   {
	   	if (resh == 1)
		{ 
			if (resl == slave_address) // if 9th bit 0 and address == slave address
			{
				is_it_my_address = 1; // if it is slave address read data (point below)
			}
		}	
   }
   else if (is_it_my_address == 1)	// if it is slave address read data
   {
		if ((resh == 0)) // if 9th bit 0
		{
			if (resl == slave_data_turn_on) // if command == turn on led
			{
				PORTB |= 0b1;	//turn on LED	
			}
			if (resl == slave_data_turn_off)// if command == turn off led
			{
				PORTB &= 0b0;	//turn on LED
			}
			is_it_my_address = 0; // wait for my address
		}	
   }
}

int main(void)
{
io_init(); // Input/Output init
slave_address = check_address(); //check slave address

usart_init(); //init uart
sei(); // set global interrupts

while (1)
{
// there is nothing to do there because everything is in interrupt
} // while

} // main