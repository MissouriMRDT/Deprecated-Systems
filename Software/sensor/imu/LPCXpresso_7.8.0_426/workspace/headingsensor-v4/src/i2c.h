#ifndef __I2C_H_
#define __I2C_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

#define BUFFER_LENGTH 32
#define I2C_FASTPLUS_BIT     0
#define SPEED_100KHZ         100000
#define SPEED_400KHZ         400000



class Wire {
public:
	static int mode_poll;	// Poll/Interrupt mode flag
	static uint8_t buffer[2][256];

	// Constructor
	Wire();

	// Init Wire object with default parameters
	void init(I2C_ID_T, int);

	// Load data in buffer, inc buffer index/length
	// Params [transfer struct],[slave address],[byte to send]
	size_t write(I2C_XFER_T*, uint8_t, uint8_t, int);

	// Fill Rx buffer with result of requestFrom
	// Params [transfer struct],[slave address],[number of bytes to read]
	int read(I2C_XFER_T*, uint8_t, int);

	// Return i2c status
	I2C_STATUS_T status(I2C_XFER_T*);

	// Set mode to polling/interrupt
	// Params [transfer struct],[polling:1, interrupt:0]
	void setMode(I2C_ID_T, bool);
};

extern Wire i2c;

#endif /* __I2C_H__ */
