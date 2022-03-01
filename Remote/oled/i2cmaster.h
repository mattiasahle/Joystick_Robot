#ifndef _I2CMASTER_H
#define _I2CMASTER_H   1


#include <avr/io.h>

/** defines the data direction (reading from I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_READ    1
/** defines the data direction (writing to I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_WRITE   0

extern void i2c_init(void);
extern void i2c_stop(void);
extern unsigned char i2c_start(unsigned char addr);
extern unsigned char i2c_rep_start(unsigned char addr);
extern uint8_t i2c_start_wait(unsigned char addr);
extern unsigned char i2c_write(unsigned char data);
extern unsigned char i2c_readAck(void);
extern unsigned char i2c_readNak(void);
extern unsigned char i2c_read(unsigned char ack);
#define i2c_read(ack)  (ack) ? i2c_readAck() : i2c_readNak(); 


#endif
