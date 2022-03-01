/******************************************************************************

	HC-SR04 ULTRASONIC DISTANCE SENSOR HEADER FILE

	This file contains the interface to interact with and use the HC-SR04
	ultrasonic distance sensor.

	Created: 2020-09-21
	Author: Mattias Ahle, mattias.ahle@gmail.com

	Last update: 2020-10-15
	Author: Mattias Ahle

******************************************************************************/

#ifndef HC_SR04_H_
#define HC_SR04_H_

#include <stdint.h>

void hc_sr04_init(void);
uint16_t hc_sr04_get_distance(void);



#endif /* HC_SR04_H_ */