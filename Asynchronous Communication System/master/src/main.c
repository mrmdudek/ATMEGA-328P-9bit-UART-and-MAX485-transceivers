#include <avr/io.h>
#include <avr/interrupt.h>
#include "serial_printf.h"
#include <inttypes.h>
#include <avr/delay.h>
#include <util/delay.h>

int main(void)
{
usart_init(); // uart init

uint8_t slave_address = 0b00000000; // slave addresses are counted from 0 to 15, initial address is 0

uint8_t slave_data_turn_on = 0b11111111; //if this data turn led on
uint8_t slave_data_turn_off = 0b00000000; //if this data turn led off

uint8_t turn_on_all = 1; // state for turn on/off led (1 - turn on, 0 turn off)

while (1)
{
	
	 if (turn_on_all==1)	// turn on all
	 {
		 for (uint8_t i = 0; i < 255; i++)// iterate all possible slave address
		 {
			 USART_Transmit(((1 << 8) | slave_address)); // transmit 9th bit 1 (address frame), slave address on remaing byte
			 USART_Transmit(((0 << 8) | slave_data_turn_on)); // transmit 9th bit 0 (data frame), slave data on remaing byte
			 slave_address++; // send data to next slave
		 }
		
		slave_address = 0b00000000; // go to next turn from 0
		turn_on_all = 0; // go to turn off led
	 }
	
	_delay_ms(1000);
	
	if (turn_on_all==0)		// turn off all
	 {
		for (uint8_t i = 0; i < 255; i++)// iterate all possible slave address
		 {
			 USART_Transmit(((1 << 8) | slave_address)); // transmit 9th bit 1 (address frame), slave address on remaing byte
			 USART_Transmit(((0 << 8) | slave_data_turn_off)); // transmit 9th bit 0 (data frame), slave data on remaing byte
			 slave_address++; // send data to next slave
		 }
		
		slave_address = 0b00000000;// go to next turn from 0
		turn_on_all = 1;// go to turn on led
	 }
	 _delay_ms(1000);

} // while
} // main