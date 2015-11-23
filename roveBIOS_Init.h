// Missouri Science and Technology Mars Rover Design Team 2015_2016
//
// roveBios_Init.h
//
// Judah Schad jrs6w7@mst.edu
//
// declare global handles for pre BIOS startup hardware config
//
// mrdt::rovWare

#ifndef ROVEBIOS_INIT_H_
#define ROVEBIOS_INIT_H_

//EK_1294XL dev shield target device
#include "../roveTargetConfigs/Board.h"

//C lib
#include <stdio.h>

//CCS TI config
#include <xdc/runtime/System.h>

//CCS TI operating system
#include <ti/sysbios/BIOS.h>

//TI hardware access routines
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/PWM.h>
//#include <ti/drivers/Watchdog.h>

///////////////BEGIN 2016//////GLOBAL HARDWARE ACCESS HANDLES//////
//DO NOT INIT UART_0 or UART_1-> hardware support conflict: HardwareResourecs/EK_TM4C1294XL.h
UART_Handle uart_2;
UART_Handle uart_3;
UART_Handle uart_4;
UART_Handle uart_5;
UART_Handle uart_6;
UART_Handle uart_7;

//global PWMs
//DO NOT INIT PWM_0 -> ethernet support conflict: HardwareResourecs/EK_TM4C1294XL.h
PWM_Handle pwm_1;
PWM_Handle pwm_2;
PWM_Handle pwm_3;
PWM_Handle pwm_4;
PWM_Handle pwm_5;
PWM_Handle pwm_6;

//TODO: global Watchdog
//Watchdog_Handle watchdog;
//TODO: ADC_0 : S_SQR 1x6
//UInt adc_1;
//UInt adc_2;
//UInt adc_3;
//UInt adc_4;
//UInt adc_5;
//UInt adc_6;
///////////////END 2016//////GLOBAL HARDWARE ACCESS HANDLES//////////



///////////////END 2016//////BEGIN HARDWARE ACCESS CFG FNCTNS////////

PWM_Handle rovePwm_Init(UInt pwm_index, UInt period_in_microseconds);
UART_Handle roveUart_Init(UInt uart_index, UInt baud_rate);
//TODO Tiva HW IO Module ADC Initialization Wrapper
//UInt roveAdc_Init(UInt adc_index, UInt adc_cfg);

///////////////END 2016//////GLOBAL HARDWARE ACCESS CFG FNCTNS//////
#endif//ROVEBIOS_INIT_H_
