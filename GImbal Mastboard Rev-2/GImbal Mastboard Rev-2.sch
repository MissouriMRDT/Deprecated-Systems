EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:MRDT-KiCAD-devices
LIBS:GImbal Mastboard Rev-2-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L TM4C1230H6PM U1
U 1 1 5A75EFDD
P 3250 5350
F 0 "U1" H 2000 7250 50  0000 L CNN
F 1 "TM4C1230H6PM" H 3900 7250 50  0000 L CNN
F 2 "LQFP-64" H 3250 6850 50  0001 C CIN
F 3 "" H 3250 5350 50  0001 C CNN
	1    3250 5350
	1    0    0    -1  
$EndComp
$Comp
L JAG_ARM_Connector-10pin U?
U 1 1 5A7CA4FE
P 9450 4750
F 0 "U?" H 9500 4700 60  0001 C CNN
F 1 "JAG_ARM_Connector-10pin" H 9650 5350 60  0000 C CNN
F 2 "" H 9450 4750 60  0001 C CNN
F 3 "" H 9450 4750 60  0001 C CNN
	1    9450 4750
	1    0    0    -1  
$EndComp
$Comp
L TIVA_Rear_Headers U?
U 1 1 5A7CA59D
P 9450 3550
F 0 "U?" H 9550 3500 60  0001 C CNN
F 1 "TIVA_Rear_Headers" H 9700 5950 60  0000 C CNN
F 2 "" H 9600 7550 60  0001 C CNN
F 3 "" H 9600 7550 60  0001 C CNN
	1    9450 3550
	1    0    0    -1  
$EndComp
$Comp
L OKI U?
U 1 1 5A7CB182
P 2600 2050
F 0 "U?" H 2450 1800 60  0001 C CNN
F 1 "OKI-5V" H 2750 2400 60  0000 C CNN
F 2 "" H 2350 1950 60  0001 C CNN
F 3 "" H 2350 1950 60  0001 C CNN
	1    2600 2050
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5A7CB1A2
P 2000 2100
F 0 "C1" H 2025 2200 50  0000 L CNN
F 1 "10uF" H 2025 2000 50  0000 L CNN
F 2 "" H 2038 1950 50  0001 C CNN
F 3 "" H 2000 2100 50  0001 C CNN
	1    2000 2100
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 5A7CB212
P 2000 1700
F 0 "C2" H 2025 1800 50  0000 L CNN
F 1 "10uF" H 2025 1600 50  0000 L CNN
F 2 "" H 2038 1550 50  0001 C CNN
F 3 "" H 2000 1700 50  0001 C CNN
	1    2000 1700
	1    0    0    -1  
$EndComp
$Comp
L APP_2 C?
U 1 1 5A7CB8EC
P 2600 1300
F 0 "C?" H 2650 1250 60  0000 C CNN
F 1 "APP_2" H 2700 1800 60  0000 C CNN
F 2 "" H 2650 750 60  0001 C CNN
F 3 "" H 2650 750 60  0001 C CNN
	1    2600 1300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5A7CBBE1
P 2450 1300
F 0 "#PWR?" H 2450 1050 50  0001 C CNN
F 1 "GND" H 2450 1150 50  0000 C CNN
F 2 "" H 2450 1300 50  0001 C CNN
F 3 "" H 2450 1300 50  0001 C CNN
	1    2450 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 950  2700 950 
Wire Wire Line
	2700 1200 2250 1200
Wire Wire Line
	2250 1200 2250 1900
Wire Wire Line
	2000 1900 2400 1900
Wire Wire Line
	2450 1200 2450 1300
Connection ~ 2450 1200
Wire Wire Line
	2000 1850 2000 1950
Wire Wire Line
	2000 950  2000 1550
Connection ~ 2000 1900
Connection ~ 2250 1900
Wire Wire Line
	2400 2250 2400 2000
Wire Wire Line
	1700 2250 2400 2250
Wire Wire Line
	2400 1800 2400 1550
Wire Wire Line
	2400 1550 2000 1550
Connection ~ 2000 2250
Text Label 2000 950  0    60   ~ 0
12V
Text Label 2250 1200 0    60   ~ 0
GND
Text GLabel 1700 2250 0    60   Input ~ 0
+5V
$Comp
L Raspberry_Pi_Header U?
U 1 1 5A7CC2AE
P 7300 3200
F 0 "U?" H 7350 3150 60  0001 C CNN
F 1 "Raspberry_Pi_Header" H 7550 5250 60  0000 C CNN
F 2 "" H 7300 4350 60  0001 C CNN
F 3 "" H 7300 4350 60  0001 C CNN
	1    7300 3200
	1    0    0    -1  
$EndComp
Text GLabel 8100 1250 2    59   Input ~ 0
+5V
Text GLabel 8100 1350 2    59   Input ~ 0
+5V
Text GLabel 8100 1450 2    60   Input ~ 0
GND
Text GLabel 7100 1250 0    60   Output ~ 0
+3V3
Text GLabel 7100 1650 0    60   Input ~ 0
GND
Text GLabel 8100 2150 2    60   Input ~ 0
GND
Text GLabel 7100 2450 0    60   Input ~ 0
GND
Text GLabel 7100 3150 0    60   Input ~ 0
GND
Text GLabel 8100 2850 2    60   Input ~ 0
GND
Text GLabel 8100 2650 2    60   Input ~ 0
GND
Text GLabel 9250 1350 0    60   Input ~ 0
+3V3
Text GLabel 10100 1350 2    60   Input ~ 0
+5V
Text GLabel 10100 1450 2    60   Input ~ 0
GND
Text GLabel 10100 2600 2    60   Input ~ 0
GND
Text GLabel 9250 4700 0    60   Input ~ 0
GND
$Comp
L IMU-LSM90S1 U?
U 1 1 5A7CC898
P 6750 5550
F 0 "U?" H 6450 5500 60  0001 C CNN
F 1 "IMU-LSM90S1" H 6750 6300 60  0000 C CNN
F 2 "" H 6450 5550 60  0001 C CNN
F 3 "" H 6450 5550 60  0001 C CNN
	1    6750 5550
	1    0    0    -1  
$EndComp
Text GLabel 4650 4250 2    60   Output ~ 0
SCL_1
Text GLabel 4650 4350 2    60   Output ~ 0
SDA_1
Text GLabel 6200 5100 0    60   Input ~ 0
SDA_1
Text GLabel 6200 5000 0    60   Input ~ 0
SCL_1
Text GLabel 6200 4900 0    60   Input ~ 0
+3V3
Text GLabel 7300 4950 2    60   Input ~ 0
GND
Wire Wire Line
	2850 7350 3450 7350
Connection ~ 3250 7350
Connection ~ 3150 7350
Connection ~ 3050 7350
$Comp
L GND #PWR?
U 1 1 5A7CB122
P 2850 7400
F 0 "#PWR?" H 2850 7150 50  0001 C CNN
F 1 "GND" H 2850 7250 50  0000 C CNN
F 2 "" H 2850 7400 50  0001 C CNN
F 3 "" H 2850 7400 50  0001 C CNN
	1    2850 7400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 7350 2850 7400
Connection ~ 2950 7350
Text GLabel 4650 5450 2    60   Input ~ 0
TCK
Text GLabel 4650 5550 2    60   Input ~ 0
TMS
Text GLabel 4650 5650 2    60   Input ~ 0
TDI
Text GLabel 4650 5750 2    60   Output ~ 0
TDO
Text GLabel 4650 5850 2    60   Input ~ 0
RX_1
Text GLabel 4650 5950 2    60   Output ~ 0
TX_1
Text GLabel 4650 3650 2    60   Output ~ 0
TX_0
Text GLabel 4650 3750 2    60   Input ~ 0
RX_0
Text GLabel 4650 4750 2    60   Output ~ 0
SCL_0
Text GLabel 4650 4850 2    60   Output ~ 0
SDA_0
$Comp
L Molex_SL-4 U?
U 1 1 5A7CB2F2
P 9450 5600
F 0 "U?" H 9500 5550 60  0001 C CNN
F 1 "Molex_SL-4" H 9650 6050 60  0000 C CNN
F 2 "" H 9450 5600 60  0001 C CNN
F 3 "" H 9450 5600 60  0001 C CNN
	1    9450 5600
	1    0    0    -1  
$EndComp
Text GLabel 9250 5250 0    60   Input ~ 0
GND
Text GLabel 9250 5550 0    60   Input ~ 0
+3V3
Text GLabel 9250 5350 0    60   Input ~ 0
SCL_0
Text GLabel 9250 5450 0    60   Input ~ 0
SDA_0
Text GLabel 9250 4300 0    60   Input ~ 0
+3V3
Text GLabel 10100 4300 2    60   Input ~ 0
TMS
Text GLabel 10100 4400 2    60   Input ~ 0
TCK
Text GLabel 10100 4500 2    60   Input ~ 0
TDO
Text GLabel 10100 4600 2    60   Output ~ 0
TDI
$Comp
L +5VD #PWR?
U 1 1 5A7CB639
P 3700 900
F 0 "#PWR?" H 3700 750 50  0001 C CNN
F 1 "+5VD" H 3700 1040 50  0000 C CNN
F 2 "" H 3700 900 50  0001 C CNN
F 3 "" H 3700 900 50  0001 C CNN
	1    3700 900 
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5A7CB655
P 3700 1100
F 0 "R1" V 3780 1100 50  0000 C CNN
F 1 "330" V 3700 1100 50  0000 C CNN
F 2 "" V 3630 1100 50  0001 C CNN
F 3 "" H 3700 1100 50  0001 C CNN
	1    3700 1100
	1    0    0    -1  
$EndComp
$Comp
L LED POWER
U 1 1 5A7CB688
P 3700 1450
F 0 "POWER" H 3700 1550 50  0000 C CNN
F 1 "Green" H 3700 1350 50  0000 C CNN
F 2 "" H 3700 1450 50  0001 C CNN
F 3 "" H 3700 1450 50  0001 C CNN
	1    3700 1450
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR?
U 1 1 5A7CB6C5
P 3700 1650
F 0 "#PWR?" H 3700 1400 50  0001 C CNN
F 1 "GND" H 3700 1500 50  0000 C CNN
F 2 "" H 3700 1650 50  0001 C CNN
F 3 "" H 3700 1650 50  0001 C CNN
	1    3700 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 1650 3700 1600
Wire Wire Line
	3700 1300 3700 1250
Wire Wire Line
	3700 950  3700 900 
Text Notes 10000 5350 3    60   ~ 0
I2C\n
Text Notes 10000 5900 3    60   ~ 0
Serial\n
$Comp
L Molex_SL-3 U?
U 1 1 5A7CBA7D
P 9450 6150
F 0 "U?" H 9500 6100 60  0001 C CNN
F 1 "Molex_SL-3" H 9650 6500 60  0000 C CNN
F 2 "" H 9450 6150 60  0001 C CNN
F 3 "" H 9450 6150 60  0001 C CNN
	1    9450 6150
	1    0    0    -1  
$EndComp
Text GLabel 9250 5900 0    60   Input ~ 0
TX_2
Text GLabel 9250 6000 0    60   Input ~ 0
RX_2
Text GLabel 4650 6950 2    60   Input ~ 0
RX_2
Text GLabel 4650 7050 2    60   Output ~ 0
TX_2
Text GLabel 4650 4550 2    60   Input ~ 0
RX_1
Text GLabel 4650 4650 2    60   Output ~ 0
TX_1
Text GLabel 8100 1550 2    60   Input ~ 0
TX_1
Text GLabel 8100 1650 2    60   Output ~ 0
RX_1
$EndSCHEMATC
