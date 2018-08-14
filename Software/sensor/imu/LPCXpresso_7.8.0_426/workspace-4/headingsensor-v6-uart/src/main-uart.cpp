/*
 ===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <stdint.h>
#include "LSM9DS1.h"


// Create an instance of the LSM9DS1 library
// Parameters: [accel/gyro I2C address],[mag I2C address]
LSM9DS1 dof(I2C_SLAVE_AG_ADDR, I2C_SLAVE_M_ADDR);
STATIC RINGBUFF_T txring, rxring;

//int main(void) {
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();

	float var;

	/* Variables */
	char vars[5];
	int var2;
	const int SIZE = 40;
	static uint8_t rxbuff[SIZE], txbuff[SIZE];
#if (defined(BOARD_NXP_XPRESSO_11U14) || defined(BOARD_NGX_BLUEBOARD_11U24))
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 18, IOCON_FUNC1 | IOCON_MODE_INACT); /* PIO0_18 used for RXD */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 19, IOCON_FUNC1 | IOCON_MODE_INACT); /* PIO0_19 used for TXD */
#elif (defined(BOARD_NXP_XPRESSO_11C24) || defined(BOARD_MCORE48_1125))
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6,
			(IOCON_FUNC1 | IOCON_MODE_INACT));/* RXD */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7,
			(IOCON_FUNC1 | IOCON_MODE_INACT));/* TXD */
#else
#error "No Pin muxing defined for UART operation"
#endif
	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, 115200);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_USART);/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, 115200);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_USART);
	/* Before using the ring buffers, initialize them using the ring
	 buffer init function */
	RingBuffer_Init(&rxring, rxbuff, 1, SIZE);
	RingBuffer_Init(&txring, txbuff, 1, SIZE);

	// Initialize LSM9DS1 object
	uint16_t status = dof.begin(dof.G_SCALE_245DPS, dof.A_SCALE_2G,
			dof.M_SCALE_4GS, dof.G_ODR_119_BW_14, dof.A_ODR_50, dof.M_ODR_40,
			dof.M_XY_OM_M, dof.M_Z_OM_M);

	// Wait until successful communication
	while (status != WHO_AM_I) {
	};

	// Calibrate sensors
	dof.calibrate();

	while (1) { // do stuff
		dof.updateGAM();
		dof.updatePRY();
		var = dof.getYaw() + 1.57;

		var2 = var * RAD2DEG;

		itoa(var2, vars, 10);
		if(var2<100){
			vars[2] = vars[1];
			vars[1] = vars[0];
			vars[0] = '0';
		}
		if(var2<10){
			vars[2] = vars[0];
			vars[0] = '0';
		}
		vars[3] = '\r';
		vars[4] = '\n';

		Chip_UART_SendRB(LPC_USART, &txring, vars, sizeof(vars) - 1);
	}

//	Chip_I2C_DeInit(I2C0);

	// Force the counter to be placed into memory
	volatile static int i = 0;
	// Enter an infinite loop, just incrementing a counter
	while (1) {
		i++;
	}
	return 0;
}

