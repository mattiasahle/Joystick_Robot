/*******************************************************************************

	KEYSTUDIO JOYSTICK HEADER FILE

	This file contains public function prototypes and macro definitions for
	using the KeyStudio Joystick.

	Created: 2020-10-22
	Author: Mattias Ahle, mattias.ahle@gmail.com

	Last update: 2020-10-28
    By: Mattias Ahle

	Below, the joystick's logical view can be seen from above.
	Observe, this is not representative of the joystick's actual
	output, only how it is logically interpreted.

				  (Y positive)
						Y+
	(X negative)	X- (O) X+	(X positive)
						Y-
				  (Y negative)

	Maximum and minimum values of each axis and each direction are named
	min closest to the origin and max the furthest away from the origin.

								Y_POSITIVE_MAX
									  |
								Y_POSITIVE_MIN

	X_NEGATIVE_MAX - X_NEGATIVE_MIN  (O)  X_POSITIVE_MIN - X_POSITIVE_MAX

								Y_NEGATIVE_MIN
									  |
								Y_NEGATIVE_MAX

*******************************************************************************/


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

/*******************************************************************************
	INCLUDE
*******************************************************************************/
#include <avr/io.h>



/*******************************************************************************
	DEFINE
*******************************************************************************/

/*******************************************************************************
	Connections
*******************************************************************************/
#define JOYSTICK_X PORTC1
#define JOYSTICK_Y PORTC0
#define JOYSTICK_B PORTD2



/*******************************************************************************
	Joystick orientation setup

	Insert joystick output at given position
	If changed, the four functions joystick_is_<axis>_<positive/negative>() in
	joystick.c has to be changed accordingly.
*******************************************************************************/
#define X_VAL_LEFT 255
#define X_VAL_RIGHT 0
#define Y_VAL_DOWN 255
#define Y_VAL_UP 0

/*******************************************************************************
	Joystick output in relaxed position (origin)
*******************************************************************************/
#define X_ZERO 133
#define Y_ZERO 127

/*******************************************************************************
	Zero offset

	Range above and below Y origin which will give no PWM, aka. neutral gear.
*******************************************************************************/
#define ZERO_OFFSET 5



/*******************************************************************************
	PUBLIC FUNCTIONS
*******************************************************************************/
void joystick_init(void);

uint8_t joystick_get_position(char axis);

uint8_t joystick_button_is_pressed(void);
void joystick_handle_button_press(void);

uint8_t joystick_is_x_positive(uint8_t *p_x);
uint8_t joystick_is_x_negative(uint8_t *p_x);
uint8_t joystick_is_y_positive(uint8_t *p_y);
uint8_t joystick_is_y_negative(uint8_t *p_y);

uint8_t joystick_get_x_negative_range(void);
uint8_t joystick_get_x_positive_range(void);
uint8_t joystick_get_y_negative_range(void);
uint8_t joystick_get_y_positive_range(void);

uint8_t joystick_get_x_negative_min(void);
uint8_t joystick_get_x_positive_min(void);
uint8_t joystick_get_y_negative_min(void);
uint8_t joystick_get_y_positive_min(void);



#endif /* JOYSTICK_H_ */