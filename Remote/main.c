/*******************************************************************************

	REDBOT REMOTE CONTROL

	Main execution file for RedBot Remote Control.

	Created: 2020-10-27
	Author: Mattias Ahle, mattias.ahle@gmail.com

	Last update: 2020-11-03
    By: Mattias Ahle

*******************************************************************************/



/*******************************************************************************
	DEFINE
*******************************************************************************/
#define F_CPU 16000000UL



/*******************************************************************************
	INCLUDE
*******************************************************************************/
#include "joystick.h"
#include "output_byte_creator.h"
#include "usart0.h"
#include "oled/lcd.h"
#include "oled/printout.h"
#include "oled/font.h"
#include <util/delay.h>
#include <stdlib.h>



/*******************************************************************************
	GLOBAL VARIABLES
*******************************************************************************/
static char buffer[10];
int8_t received_byte;


/*******************************************************************************
	FUNCTION PROTOTYPES
*******************************************************************************/
void handle_usart_receive(void);
void print_x_on_oled(uint8_t *p_x);
void print_y_on_oled(uint8_t *p_y);
void print_output_byte_on_oled(uint8_t output_byte);
void reset_error_print_on_oled(void);
void print_collision_error_on_oled(void);
void print_distance_error_on_oled(void);
void print_communication_error_on_oled(void);
void print_pic_full_screen(const unsigned char *arr);
void print_pic_half_top_screen(const unsigned char *arr);



/*******************************************************************************
	MAIN FUNCTION
*******************************************************************************/
int main(void)
{
	uint8_t joystick_x_value;
	uint8_t joystick_y_value;
	uint8_t output_byte;

	joystick_init();
	output_byte_creator_init();
	usart_init();
	lcd_init(0xAF);

	_delay_ms(100);

	//Print intro pics
	print_pic_full_screen(&slak_2020);
	_delay_ms(4000);
	lcd_clrscr();

	print_pic_full_screen(&hkr_logo_two);
	_delay_ms(2000);
	lcd_clrscr();

    while (1)
	{
		if (joystick_button_is_pressed())
		{
			reset_error_print_on_oled();
		}

		if (usart_receive())
		{
			handle_usart_receive();
		}

		joystick_x_value = joystick_get_position('X');
		//print_x_on_oled(&joystick_x_value);

		joystick_y_value = joystick_get_position('Y');
		//print_y_on_oled(&joystick_y_value);

		output_byte = output_byte_creator_create(&joystick_x_value, &joystick_y_value, 'L');
		print_output_byte_on_oled(output_byte);
		usart_transmit_character(output_byte);

		output_byte = output_byte_creator_create(&joystick_x_value, &joystick_y_value, 'R');
		print_output_byte_on_oled(output_byte);
		usart_transmit_character(output_byte);
    }
}



/*******************************************************************************
	SUB FUNCTIONS
*******************************************************************************/
void handle_usart_receive(void)
{
	received_byte = UDR0;

	if (received_byte == '0')
	{
		reset_error_print_on_oled();
	}
	else if (received_byte == '1')
	{
		while (1) //wait for button press to confirm collision
		{
			print_pic_full_screen(&collision_one);
			_delay_ms(500);
			print_collision_error_on_oled();
			_delay_ms(500);

			if (joystick_button_is_pressed())
			{
				break;
			}
		}
		usart_transmit_character('1');
		lcd_clrscr();
	}
	else if (received_byte == '2')
	{
		print_distance_error_on_oled();
	}
	else
	{
		print_communication_error_on_oled();
	}
}

void reset_error_print_on_oled(void)
{
	printout_lcd_pos_puts(0, 0, "                    ");
	printout_lcd_pos_puts(0, 1, "                    ");
	printout_lcd_pos_puts(0, 2, "                    ");
	printout_lcd_pos_puts(0, 3, "                    ");
}

void print_collision_error_on_oled(void)
{
	printout_lcd_pos_puts(6, 2, "COLLISION!");
	printout_lcd_pos_puts(4, 3, "Press and hold");
	printout_lcd_pos_puts(3, 4, "joystick button");
	printout_lcd_pos_puts(6, 5, "to confirm.");
}

void print_distance_error_on_oled(void)
{
	print_pic_half_top_screen(&safe_distance);
// 	printout_lcd_pos_puts(0, 0, "OBSTACLE WARNING    ");
// 	printout_lcd_pos_puts(0, 1, "Forward motion      ");
// 	printout_lcd_pos_puts(0, 2, "disabled            ");
// 	printout_lcd_pos_puts(0, 3, "                    ");
}

void print_communication_error_on_oled(void)
{
	printout_lcd_pos_puts(0, 0, "COMMUNICATION ERROR ");
	printout_lcd_pos_puts(0, 1, "                    ");
	printout_lcd_pos_puts(0, 2, "                    ");
	printout_lcd_pos_puts(0, 3, "                    ");
}

void print_x_on_oled(uint8_t *p_x)
{
	printout_lcd_pos_puts(0, 0, "Raw X = ");
	itoa(*p_x, buffer, 10);
	printout_clear_garbage_left_align(3, buffer);
	printout_lcd_pos_puts(8, 0, buffer);
}

void print_y_on_oled(uint8_t *p_y)
{
	printout_lcd_pos_puts(0, 1, "Raw Y = ");
	itoa(*p_y, buffer, 10);
	printout_clear_garbage_left_align(3, buffer);
	printout_lcd_pos_puts(8, 1, buffer);
}

void print_output_byte_on_oled(uint8_t output_byte)
{
	printout_lcd_pos_puts(2, 4, "JOYSTICK OUTPUT");
	printout_lcd_pos_puts(0, 5, "Motor: LEFT | RIGHT");
	printout_lcd_pos_puts(0, 6, "Gear:");
	printout_lcd_pos_puts(12, 6, "|");
	printout_lcd_pos_puts(0, 7, "PWM:");
	printout_lcd_pos_puts(12, 7, "|");

	if (output_byte & (1 << 0))	//if left motor
	{
		if (output_byte & (1 << 1))	//if fwd gear
		{
			printout_lcd_pos_puts(8, 6, "FWD");
		}
		else //else rev gear
		{
			printout_lcd_pos_puts(8, 6, "REV");
		}

		itoa(output_byte & 0b11111100, buffer, 10);
		printout_clear_garbage_left_align(3, buffer);
		printout_lcd_pos_puts(8, 7, buffer);
	}
	else //else right motor
	{
		if (output_byte & (1 << 1))	//if fwd gear
		{
			printout_lcd_pos_puts(14, 6, "FWD");
		}
		else //else rev gear
		{
			printout_lcd_pos_puts(14, 6, "REV");
		}

		itoa(output_byte & 0b11111100, buffer, 10);
		printout_clear_garbage_left_align(3, buffer);
		printout_lcd_pos_puts(14, 7, buffer);
	}
}



void print_pic_full_screen(const unsigned char *arr)
{
	int counter = 0;
	for (uint8_t a = 0; a < 8; a++)
	{
		lcd_gotoxy(0, a);
		i2c_start(LCD_I2C_ADR << 1);
		i2c_write(0x40);

		for (uint8_t b = 0; b < 128; b++)
		{
			i2c_write(pgm_read_byte(&arr[counter]));
			counter++;
		}

		i2c_stop();
	}
}



void print_pic_half_top_screen(const unsigned char *arr)
{
	int counter = 0;
	for (uint8_t a = 0; a < 4; a++)
	{
		lcd_gotoxy(0, a);
		i2c_start(LCD_I2C_ADR << 1);
		i2c_write(0x40);

		for (uint8_t b = 0; b < 128; b++)
		{
			i2c_write(pgm_read_byte(&arr[counter]));
			counter++;
		}

		i2c_stop();
	}
}