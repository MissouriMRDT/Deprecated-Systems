// roveHardwareAbstraction
//
// first created:
//
// 01_22_2015_Owen_Chiaventone omc8db
//
// last edited:
//
// 02_24_2015_Judah Schad_jrs6w7@mst.edu

#include "../roveWareHeaders/roveHardwareAbstraction.h"

//TODO Configure Patch Panel Jacks to Physical Devices (In Hardware FIRST)

int getDeviceJack(int device){

	switch(device){
		case 0:
				//Tried to get jack for an null device
				System_printf("getDeviceJack passed null device %d\n", device);
				System_flush();
			return;

		case test_device_id:

			return ONBOARD_ROVECOMM;

		case motor_left_id:

			return ONBOARD_ROVECOMM;

		case motor_right_id:

			return ONBOARD_ROVECOMM;

		case robot_arm_id:

			return ONBOARD_ROVECOMM;

		case drill_id:

			return ONBOARD_ROVECOMM;

		case bms_id:

			return ONBOARD_ROVECOMM;

		case power_board_id:

			return POWER_BOARD;

		default:
				//Tried to get jack for an \ invalid device
				System_printf("getDeviceJack passed invalid device %d\n", device);
				System_flush();
			return;

	}//endswitch (device)

}//endfnctn deviceJack

void pinMode(int pin, int mode){

	//No idea how to start this one

}//endfnctn pinMode

void digitalWrite(int pin, int val){

	//Alarmingly enough, a switch case is really the only way to deal with this
	//Everything is base on a bunch of TI defined constants that can't be indexed into
	//Or iterated through

	if(val == LOW){

		switch(pin){

			case U3_MUX_S0 :
				GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, (0));
				break;
			case U3_MUX_S1 :
				GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_1, (0));
				break;
			case U4_MUX_S0 :
				GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_6, (0));
				break;
			case U4_MUX_S1 :
				GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_7, (0));
				break;
			case U5_MUX_S0 :
				GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, (0));
				break;
			case U5_MUX_S1 :
				GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, (0));
				break;
			case U6_MUX_S0 :
				GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_2, (0));
				break;
			case U6_MUX_S1 :
				GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_3, (0));
				break;
			case U7_MUX_S0 :
				GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, (0));
				break;
			case U7_MUX_S1 :
				GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, (0));
				break;
			default:
						//Tried to write to invalid device
						System_printf("DigitalWrite passed invalid pin %d\n", pin);
						System_flush();
				return;
		}//endswitch

	}else if (val == HIGH){

		switch(pin){

			case U3_MUX_S0 :
				GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_0, (~0));
				break;
			case U3_MUX_S1 :
				GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_1, (~0));
				break;
			case U4_MUX_S0 :
				GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_6, (~0));
				break;
			case U4_MUX_S1 :
				GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_7, (~0));
				break;
			case U5_MUX_S0 :
				GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_0, (~0));
				break;
			case U5_MUX_S1 :
				GPIOPinWrite(GPIO_PORTL_BASE, GPIO_PIN_1, (~0));
				break;
			case U6_MUX_S0 :
				GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_2, (~0));
				break;
			case U6_MUX_S1 :
				GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_3, (~0));
				break;
			case U7_MUX_S0 :
				GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, (~0));
				break;
			case U7_MUX_S1 :
				GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, (~0));
				break;
			default:
						//Tried to write to invalid device
						System_printf("DigitalWrite passed invalid pin %d\n", pin);
						System_flush();
					return;

		}//endswitch

	}//endif

	return;

}//endfnctn digitalWrite

// output 1.5ms for 2 seconds to arm the servos

PWM_Handle rovePWMInit(PWM_Handle pwm_index, uint16_t period_in_microseconds){

	PWM_Handle pwm_handle;

	PWM_Params pwmParams;
    uint16_t   duty = 0;
    uint16_t   dutyInc = 100;

	PWM_Params_init(&pwmParams);

	pwmParams.period = period_in_microseconds;
	pwm_handle = PWM_open(pwm_index, &pwmParams);

	if(pwm_handle == NULL){

		System_abort("Error opening the PWM\n");

	}//endif

	return pwm_handle;

}//endfnctn rovePWMInit

void pwmWrite(PWM_Handle pin, int duty_microseconds){

	// scale to nuetral to 1.5 uSec and resolution to 500 increments

	int speed = ((duty_microseconds/4)+1500);


				if (speed > 1750){

					speed = 1700;

				}//endif

				if (speed < 1250){

					speed = 1250;

				}//endif

	PWM_setDuty(pin, speed);

				//System_printf("speed holds %d \n", ((duty_microseconds/2)+1500));
				//System_flush();

}//endfnctn pwmWrite

int deviceWrite(int rs485jack, char* buffer, int bytes_to_write){

	int bytes_wrote;

	// give us access to the uart handles defined at the global scope in main

	extern UART_Handle uart0;
	extern UART_Handle uart1;
	extern UART_Handle uart2;
	extern UART_Handle uart3;
	extern UART_Handle uart4;
	extern UART_Handle uart5;
	extern UART_Handle uart6;
	extern UART_Handle uart7;

					//System_printf("deviceWrite called\n");
					//System_flush();

	switch(rs485jack){

		// we have to include case 0 to get TI's compiler to build a jump table
		// if we leave this out, mux performance goes from O(1) to O(n) (That's bad)
		case 0:
		break;
		case 1:
			// configure the mux pins, see the mux datasheet for more info
			digitalWrite(U3_MUX_S0, HIGH);
			digitalWrite(U3_MUX_S1, HIGH);
			// write the buffer to the device
			bytes_wrote = UART_write(uart3, buffer, bytes_to_write);
			break;
		case 2:
			digitalWrite(U3_MUX_S0, LOW);
			digitalWrite(U3_MUX_S1, HIGH);
			bytes_wrote = UART_write(uart3, buffer, bytes_to_write);
			break;
		case 3:
			digitalWrite(U3_MUX_S0, HIGH);
			digitalWrite(U3_MUX_S1, LOW);
			bytes_wrote = UART_write(uart3, buffer, bytes_to_write);
			break;
		case 4:
			digitalWrite(U6_MUX_S0, LOW);
			digitalWrite(U6_MUX_S1, HIGH);
			bytes_wrote = UART_write(uart6, buffer, bytes_to_write);
			break;
		case 5:
			digitalWrite(U6_MUX_S0, HIGH);
			digitalWrite(U6_MUX_S1, LOW);
			bytes_wrote = UART_write(uart6, buffer, bytes_to_write);
			break;
		case 6:
			digitalWrite(U7_MUX_S0, HIGH);
			digitalWrite(U7_MUX_S1, HIGH);
			bytes_wrote = UART_write(uart7, buffer, bytes_to_write);
			break;
		case 7:
			digitalWrite(U7_MUX_S0, LOW);
			digitalWrite(U7_MUX_S1, HIGH);
			bytes_wrote = UART_write(uart7, buffer, bytes_to_write);
			break;
		case 8:
			digitalWrite(U7_MUX_S0, HIGH);
			digitalWrite(U7_MUX_S1, LOW);
			bytes_wrote = UART_write(uart7, buffer, bytes_to_write);
			break;
		case 9:
			digitalWrite(U5_MUX_S0, LOW);
			digitalWrite(U5_MUX_S1, LOW);
			bytes_wrote = UART_write(uart5, buffer, bytes_to_write);
			break;
		case 10:
			digitalWrite(U5_MUX_S0, LOW);
			digitalWrite(U5_MUX_S1, HIGH);
			bytes_wrote = UART_write(uart5, buffer, bytes_to_write);
			break;
		case 11:
			digitalWrite(U5_MUX_S0, HIGH);
			digitalWrite(U5_MUX_S1, LOW);
			bytes_wrote = UART_write(uart5, buffer, bytes_to_write);
			break;
		case 12:
			digitalWrite(U5_MUX_S0, LOW);
			digitalWrite(U5_MUX_S1, LOW);
			bytes_wrote = UART_write(uart5, buffer, bytes_to_write);
			break;
		case 13:
			digitalWrite(U5_MUX_S0, HIGH);
			digitalWrite(U5_MUX_S1, HIGH);
			bytes_wrote = UART_write(uart5, buffer, bytes_to_write);
			break;
		case 14:
			digitalWrite(U4_MUX_S0, LOW);
			digitalWrite(U4_MUX_S1, LOW);
			bytes_wrote = UART_write(uart4, buffer, bytes_to_write);
			break;
		case 15:
			digitalWrite(U4_MUX_S0, LOW);
			digitalWrite(U4_MUX_S1, HIGH);
			bytes_wrote = UART_write(uart4, buffer, bytes_to_write);
			break;
		case 16:
			digitalWrite(U4_MUX_S0, HIGH);
			digitalWrite(U4_MUX_S1, HIGH);
			bytes_wrote = UART_write(uart4, buffer, bytes_to_write);
			break;
		case 17:
			digitalWrite(U4_MUX_S0, HIGH);
			digitalWrite(U4_MUX_S1, LOW);
			bytes_wrote = UART_write(uart4, buffer, bytes_to_write);
			break;
		case POWER_BOARD:
			digitalWrite(U6_MUX_S0, HIGH);
			digitalWrite(U6_MUX_S1, HIGH);
			bytes_wrote = UART_write(uart6, buffer, bytes_to_write);
			break;
		case ONBOARD_ROVECOMM:
			bytes_wrote = UART_write(uart2, buffer, bytes_to_write);
			break;
		default:
			//Tried to write to invalid device
			System_printf("DeviceWrite passed invalid device %d\n", rs485jack);
			System_flush();
			return -1;
		//etc.

	}//end switch(rs485jack)

	// make sure the message is fully written before leaving the function

	ms_delay(1);

	return bytes_wrote;

}//endfnctn deviceWrite

int deviceRead(int rs485jack, char* buffer, int bytes_to_read, int timeout){

	int bytes_read;

	// give us access to the uart handles defined at the global scope in main

	extern UART_Handle uart0;
	extern UART_Handle uart1;
	extern UART_Handle uart2;
	extern UART_Handle uart3;
	extern UART_Handle uart4;
	extern UART_Handle uart5;
	extern UART_Handle uart6;
	extern UART_Handle uart7;


	// we have to include case 0 to get TI's compiler to build a jump table
	// if we leave this out, mux performance goes from O(1) to O(n) (That's bad)
	switch(rs485jack){
		case 0:
		case 1:
			//Configure the mux pins
			//See the mux datasheet for more info
			digitalWrite(U3_MUX_S0, HIGH);
			digitalWrite(U3_MUX_S1, HIGH);

			//Write the buffer to the device
			bytes_read = UART_read(uart3, buffer, bytes_to_read);
			break;
		case 2:
			digitalWrite(U3_MUX_S0, LOW);
			digitalWrite(U3_MUX_S1, HIGH);
			bytes_read = UART_read(uart3, buffer, bytes_to_read);
			break;
		case 3:
			digitalWrite(U3_MUX_S0, HIGH);
			digitalWrite(U3_MUX_S1, LOW);
			bytes_read = UART_read(uart3, buffer, bytes_to_read);
			break;
		case 4:
			digitalWrite(U6_MUX_S0, LOW);
			digitalWrite(U6_MUX_S1, HIGH);
			bytes_read = UART_read(uart6, buffer, bytes_to_read);
			break;
		case 5:
			digitalWrite(U6_MUX_S0, HIGH);
			digitalWrite(U6_MUX_S1, LOW);
			bytes_read = UART_read(uart6, buffer, bytes_to_read);
			break;
		case 6:
			digitalWrite(U7_MUX_S0, HIGH);
			digitalWrite(U7_MUX_S1, HIGH);
			bytes_read = UART_read(uart7, buffer, bytes_to_read);
			break;
		case 7:
			digitalWrite(U7_MUX_S0, LOW);
			digitalWrite(U7_MUX_S1, HIGH);
			bytes_read = UART_read(uart7, buffer, bytes_to_read);
			break;
		case 8:
			digitalWrite(U7_MUX_S0, HIGH);
			digitalWrite(U7_MUX_S1, LOW);
			bytes_read = UART_read(uart7, buffer, bytes_to_read);
			break;
		case 9:
			digitalWrite(U5_MUX_S0, LOW);
			digitalWrite(U5_MUX_S1, LOW);
			bytes_read = UART_read(uart5, buffer, bytes_to_read);
			break;
		case 10:
			digitalWrite(U5_MUX_S0, LOW);
			digitalWrite(U5_MUX_S1, HIGH);
			bytes_read = UART_read(uart5, buffer, bytes_to_read);
			break;
		case 11:
			digitalWrite(U5_MUX_S0, HIGH);
			digitalWrite(U5_MUX_S1, LOW);
			bytes_read = UART_read(uart5, buffer, bytes_to_read);
			break;
		case 12:
			digitalWrite(U5_MUX_S0, LOW);
			digitalWrite(U5_MUX_S1, LOW);
			bytes_read = UART_read(uart5, buffer, bytes_to_read);
			break;
		case 13:
			digitalWrite(U5_MUX_S0, HIGH);
			digitalWrite(U5_MUX_S1, HIGH);
			bytes_read = UART_read(uart5, buffer, bytes_to_read);
			break;
		case 14:
			digitalWrite(U4_MUX_S0, LOW);
			digitalWrite(U4_MUX_S1, LOW);
			bytes_read = UART_read(uart4, buffer, bytes_to_read);
			break;
		case 15:
			digitalWrite(U4_MUX_S0, LOW);
			digitalWrite(U4_MUX_S1, HIGH);
			bytes_read = UART_read(uart4, buffer, bytes_to_read);
			break;
		case 16:
			digitalWrite(U4_MUX_S0, HIGH);
			digitalWrite(U4_MUX_S1, HIGH);
			bytes_read = UART_read(uart4, buffer, bytes_to_read);
			break;
		case 17:
			digitalWrite(U4_MUX_S0, HIGH);
			digitalWrite(U4_MUX_S1, LOW);
			bytes_read = UART_read(uart4, buffer, bytes_to_read);
			break;
		case POWER_BOARD:
			digitalWrite(U6_MUX_S0, HIGH);
			digitalWrite(U6_MUX_S1, HIGH);
			bytes_read = UART_read(uart6, buffer, bytes_to_read);
			break;
		case ONBOARD_ROVECOMM:
			bytes_read = UART_read(uart2, buffer, bytes_to_read);
			break;
		default:
			//Tried to write to invalid device
			System_printf("DeviceWrite passed invalid device %d\n", rs485jack);
			System_flush();
			return -1;
		//etc.

	}//endswitch(rs485jack)

	return bytes_read;

}//endfnctn deviceRead

int generateMotorCommand(int speed, char* buffer){

	// variable to do temp work on the count of needed buffer size

	int workerCount = 0;

	// variable to do temp work on the integer for ASCII conversion

	int workerInt = speed;

	// variable to work on the character for ASCII version

	char workerChar;

	while(workerInt != 0){

		// shift right one decimal digit

		workerInt = workerInt/10;

		// count that digit

		workerCount = workerCount + 1;

	}// endwhile(workerInt != 0)

	// reload the speed value

	workerInt = speed;

	// initialize the buffer for G command

	buffer[0] = '!';
	buffer[1] = 'G';
	buffer[2] = ' ';
	buffer[3] = '1';
	buffer[4] = ' ';

	workerCount = workerCount + 5;

	// terminate the string with control character
	// motor controller interperts '_' as the \r

	buffer[workerCount] = '_';

	//just for good measure (systemPrintf debugging, etc)

	buffer[workerCount + 1] = '\0';

	// loop until we run out of digits

	while(workerCount > 5){

		//move one character towards the front of the buffer

		workerCount = workerCount - 1;

		//extracts right digit of integer speed and stores as binary

		workerChar = (workerInt%10);

		//add ASCII offset and place at the back of the buffer

		buffer[workerCount] = workerChar + '0';

		//truncates the last digit of integer speed (shift one digit)

		workerInt = workerInt/10;

	}//endwhile:	(workerCount > 0)

	// the message '!G 1 <ASCIISPEED>_' is placed by side effect in the passed buffer

	// return the size of the message

	return workerCount + 5;

}//end fnctn generateMotorCommand

// TI does these inits in the API implicitly in EK_TM4C1294XL but we may want some added value here:

//void initUarts(){
//}//endfnctn initUarts

//void initMuxGPIO(){
	//Probably just a bunch of pinModes.
//}//endfnctn initMuxGPIO
