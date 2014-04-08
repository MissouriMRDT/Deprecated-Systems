// The rover
//
// Keenan Johnson 2014

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Queue.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <inc/hw_ssi.h>
#include <driverlib/ssi.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include "driverlib/sysctl.h"
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/SPI.h>
#include "driverlib/uart.h"
#include "driverlib/rom.h"

/* Board Header file */
#include "Board.h"

// Stellarino
#include "Stellarino/stellarino_uart.h"

// Driver Files
#include "include/muxes.h"
#include "include/lcd.h"
#include "include/tcp.h"
#include "include/uarts.h"
#include "include/struct_test.h"

//Task Files
#include "tasks/tcp_task.h"
#include "tasks/Mux_test_task.h"
#include "tasks/debug_console_task.h"

Queue_Handle debug_Q;

Int main(Void)
{
	// TI RTOS Inits
    Board_initGeneral();
    Board_initGPIO();
    Board_initUART();


    // Custom Inits
    init_mux_pins();
    //init_spi();

    //Establish TCP connection
    //set_up_tcp();

    // Turn on LED for fun
    GPIO_write(Board_LED0, Board_LED_ON);

    // Create qeueues
    //debug_Q = Queue_create(NULL, NULL);

    // Start BIOS (RTOS)
    BIOS_start();

    //System_printf("%c", input);
	//System_flush()

    return (0);
}
