/*
 * roveHardwareAbstraction.c
 *
 *  Created on: Mar 3, 2015
 *      Author: Owen
 */


#include "../roveWareHeaders/roveHardwareAbstraction.h"

void pinMode(int pin, int mode)
{
	//No idea how to start this one
}

void digitalWrite(int pin, int val)
{
	/*
	 * Some big switch case is probably the best way to handle this
	switch pin:
	{
	case U3_MUX_S0:
		//Not the right port, etc
		GPIOPinWrite(GPIO_PORTH_BASE, (GPIO_PIN_0 | GPIO_PIN_1), (0));
		break;
	case U3_MUX_S1:
		break;
	}
	*/

}

int deviceWrite(int rs485jack, char* buffer, int buf_len)
{
	/*
	switch(rs485jack)
	{
	case 1:
		digitalWrite(U1_MUX_S1, LOW)
		digitalWrite(U1_MUX_S0, LOW)
		//Write buffer to UART
		break;
	//etc.
	}
	*/

}

int deviceRead(int rs485jack, char* buffer, int buf_len, int timeout)
{
	/*
	switch(rs485jack)
	{
	case 1:
		digitalWrite(U1_MUX_S1, LOW)
		digitalWrite(U1_MUX_S0, LOW)
		//Read UART into buffer
		break;
	//etc.
	}
	*/
}

void initUarts()
{

}

void initMuxGPIO()
{
	//Probably just a bunch of pinModes.
}
