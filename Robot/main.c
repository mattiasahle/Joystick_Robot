/******************************************************************************
	REDBOT ROBOT

	Main execution file for RedBot.

	Created: 2020-10-27
	Author: Mattias Ahle, mattias.ahle@gmail.com

	Last update: 2020-11-03
    By: Mattias Ahle
******************************************************************************/



/******************************************************************************
	DEFINE
******************************************************************************/
#define F_CPU 16000000UL
#define GEAR_SEL_BIT 1
#define MOTOR_SEL_BIT 0

//Distance in cm from obstacle when RedBot shall halt forward motion
#define DISTANCE_LIMIT 30

//MPU6050 ZERO (output when RedBot is leveled and still)
#define MPU6050_X_ZERO -350
#define MPU6050_Y_ZERO -35
#define MPU6050_Z_ZERO 1930

//Incremental output from MPU6050 ZERO to signal collision
#define COLLISION_LIMIT 10000



/******************************************************************************
	INCLUDE
******************************************************************************/
#include "motors/motors.h"
#include "usart0.h"
#include "mpu6050/i2cmaster.h"
#include "mpu6050/mpu6050.h"
#include "hc_sr04/hc_sr04.h"
#include "GoT.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>



/******************************************************************************
	FUNCTION PROTOTYPES
******************************************************************************/
void control_motors(uint16_t *p_distance);
void set_gears(uint16_t *distance, uint8_t *p_received_byte);
void set_PWM(uint8_t *p_received_byte);
uint8_t is_left_motor(uint8_t *p_received_byte);
uint8_t is_forward_gear(uint8_t *p_received_byte);

uint8_t is_collision_detected(void);
void handle_collision_detection(void);
void print_rawAccData(void);

void printout_clear_garbage_left_align(int string_length, char *buffer);



/******************************************************************************
	GLOBAL VARIABLES
******************************************************************************/
uint16_t distance;
uint8_t received_byte;
uint8_t collision_confirmed;
int16_t ax, ay, az;
uint8_t distance_warning_sent = 0;

char buffer[50];



/******************************************************************************
	MAIN FUNCTION
******************************************************************************/
int main(void) {
	usart_init();
	motors_init();
	hc_sr04_init();
	mpu6050_init();

    while (1)
	{
		distance = hc_sr04_get_distance();

		if (distance < DISTANCE_LIMIT && distance != 0 && !distance_warning_sent)
		{
			usart_transmit_character('2'); //transmit error code 2: obstacle warning
			distance_warning_sent = 1;
		}
		else if (distance >= DISTANCE_LIMIT && distance != 0 && distance_warning_sent)
		{
			usart_transmit_character('0'); //transmit error code 0: no errors
			distance_warning_sent = 0;
		}
		else
		{
			//Send nothing
		}

		_delay_ms(10);

		control_motors(&distance);

		if (is_collision_detected())
		{
			handle_collision_detection();
		}

	}
}



/******************************************************************************
	SUB FUNCTIONS
******************************************************************************/

/******************************************************************************
	Motor control
******************************************************************************/
void control_motors(uint16_t *p_distance)
{
	if (usart_receive())
	{
		received_byte = UDR0;

		set_gears(p_distance, &received_byte);
		set_PWM(&received_byte);
	}
}

void set_gears(uint16_t *p_distance, uint8_t *p_received_byte)
{
	if (is_forward_gear(p_received_byte))
	{
		if (*p_distance < DISTANCE_LIMIT && *p_distance != 0)
		{
			motors_set_both_neutral();
		}
		else
		{
			motors_set_both_forward();
		}
	}
	else
	{
		motors_set_both_reverse();
	}
}

uint8_t is_forward_gear(uint8_t *p_received_byte)
{
	return *p_received_byte & (1 << GEAR_SEL_BIT);
}



void set_PWM(uint8_t *p_received_byte)
{
	if (is_left_motor(p_received_byte))
	{
		motors_set_left_PWM(*p_received_byte & 0b11111100);
	}
	else
	{
		motors_set_right_PWM(*p_received_byte & 0b11111100);
	}
}

uint8_t is_left_motor(uint8_t *p_received_byte)
{
	return *p_received_byte & (1 << MOTOR_SEL_BIT);
}



/******************************************************************************
	Collision handling
******************************************************************************/
uint8_t is_collision_detected(void)
{
	mpu6050_getRawAccData(&ax, &ay, &az);

	//print_rawAccData();

	return ax > MPU6050_X_ZERO + COLLISION_LIMIT ||
	       ax < MPU6050_X_ZERO - COLLISION_LIMIT ||
	       ay > MPU6050_Y_ZERO + COLLISION_LIMIT ||
		   ay < MPU6050_Y_ZERO - COLLISION_LIMIT ||
	       az > MPU6050_Z_ZERO + COLLISION_LIMIT ||
		   az < MPU6050_Z_ZERO - COLLISION_LIMIT;
}

void handle_collision_detection(void)
{
	motors_stop();
	collision_confirmed = 0;
	usart_transmit_character('1'); //transmit error code 1: collision detected

	while (!collision_confirmed)
	{
		if (usart_receive())
		{
			if (UDR0 == '1')
			{
				collision_confirmed = 1;
			}
		}
	}
}

/* Converts the raw accelerometer data into strings and
   transmits them via USART for debugging and setup purposes.
*/
void print_rawAccData(void)
{
	usart_transmit_character('X');
	sprintf(buffer, "%d", ax);
	usart_transmit_string(buffer);
	usart_transmit_character('\t');

	usart_transmit_character('Y');
	sprintf(buffer, "%d", ay);
	usart_transmit_string(buffer);
	usart_transmit_character('\t');

	usart_transmit_character('Z');
	sprintf(buffer, "%d", az);
	usart_transmit_string(buffer);
	usart_transmit_character('\n');
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