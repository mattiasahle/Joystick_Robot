/*
 * printout.h
 *
 * Created: 2020-10-22 21:26:22
 *  Author: Mattias Ahle
 */


#ifndef PRINTOUT_H_
#define PRINTOUT_H_

void printout_lcd_pos_puts(int x, int y, char *string);
void printout_clear_garbage_left_align(int string_length, char *buffer);



#endif /* PRINTOUT_H_ */