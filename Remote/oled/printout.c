/*
 * printout.c
 *
 * Created: 2020-10-22 21:26:08
 *  Author: Mattias Ahle
 */

#include "lcd.h"
#include "i2cmaster.h"
#include "font.h"
#include <string.h>


/******************************************************************************
	Prints a string at a given position on OLED.
******************************************************************************/
void printout_lcd_pos_puts(int x, int y, char *string)
{
	for (uint8_t j = 0; j < strlen(string); j++)
	{
		lcd_gotoxy(x + j, y);			//line x column y
		i2c_start(LCD_I2C_ADR << 1);
		i2c_write(0x40);

		for (uint8_t i = 0; i < 7; i++)
		{
			i2c_write(pgm_read_byte(&(FONT[string[j] - 32][i])));
		}

		i2c_stop();
	}
}



/******************************************************************************
	Replaces garbage chars with ' ', i.e. white space,
	beginning at '\0' and ends with putting '\0' at provided index.
	This means that the index provided is the same as the maximum
	number of chars that gets displayed.

	Example:
	INDEX			   0	1	 2	   3	4	 5
	BEFORE: buffer = {'1', '2', '\0', 'g', 'g', 'g'};
	CALL:	clearBufferGarbage(5, buffer);
	AFTER:	buffer = {'1', '2', ' ', ' ', ' ', '\0'};
******************************************************************************/
void printout_clear_garbage_left_align(int string_length, char *buffer)
{
	int i = 0;

	while (buffer[i])				//find null char
	{
		i++;
	}

	while (i < string_length)		//replace garbage chars with white space
	{
		buffer[i] = ' ';
		i++;
	}

	buffer[string_length] = '\0';	//set last index in buffer to null
}