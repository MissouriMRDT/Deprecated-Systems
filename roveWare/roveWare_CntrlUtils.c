// Missouri Science and Technology Mars Rover Design Team 2015_2016
//
// roveWare_CntrlUtils.c
//
// jrs6w7@mst.edu
//
// Control Utilities for URC 2016
//
// mrdt::rovWare

#include "roveWare_CntrlUtils.h"

//roveWare 2015 Horizon Robot Arm Dynamixel Ax and Mx series control routines

//currently Nov_2016 Dev

//dynamixel servos have different configurable "modes" of operation depending on series
//check the maximum rpm of relevant model in Joint Mode, if the motor gets set to more than max rpm, it still cannot generate the torque for more than the max rpm


////////////////////////////////////////////////////////////////////TODO

//AX12 specs:

    //AX12 has two modes
        //10bit Wheel -> 0-1023 CCW, 1024 - 2047 CW
        //no amperage control

//MX12 specs

    //MX12 has three modes
        //12bit Wheel -> 0-2047 CCW, 2048 - 4095 CW
        //Goal_punch_reg and acceleration_reg control provides amperage control


//CW/CCW Angle Limit control registers allow the motion to be restrained
//position and speed command range and unit scale vary with operation mode
//The range and the unit of the Angle Limit value clamps the "go to Goal Position" command register written to at address 30 and 31
//CW Angle Limit will clamp the motor Goal Position at a minimum value
//CCW Angle Limit will clamp the motor Goal Position at a maximum value



//dynamixel sigle pin uart serial messaging protocol overhead
int8_t roveDynamixel_SendPacketMSG(uint8_t dynamixel_id, uint8_t* data_buffer, uint16_t data_byte_count){

    //dynamixel check_sum begin to add all the bytes including the id
    uint16_t current_byte_count = 0;
    uint8_t check_sum = dynamixel_id;

    //check_sum with each byte in the data_buffer
    while(current_byte_count < data_byte_count){
        check_sum += data_buffer[current_byte_count];
    }//endwhile

    //last step in the check_sum is to complement the sum, and mask with 255 (max num 8 bits)
    check_sum = (~check_sum) & 0xFF;

    //TODO I don't like this hard coded here like this tho...dont love args
    roveDigital_Write(TRI_STATE_BUFFER, TX_HIGH);

    roveDynamixel_SendByteMSG(dynamixel_id, DYNAMIXEL_PACKET_START_BYTE);
    roveDynamixel_SendByteMSG(dynamixel_id, DYNAMIXEL_PACKET_START_BYTE);
    roveDynamixel_SendByteMSG(dynamixel_id, dynamixel_id);
    roveDynamixel_SendByteMSG(dynamixel_id, data_byte_count+1);

    //send each byte in the data_buffer
    while(current_byte_count < data_byte_count){
        roveDynamixel_SendByteMSG(dynamixel_id, data_buffer[current_byte_count]);
    }//endwhile

    roveDynamixel_SendByteMSG(dynamixel_id, check_sum);

    //wait for uart_write
    roveDelay_MicroSec(DYNAMIXEL_TX_DELAY_MICRO_SEC);
    roveDigital_Write(TRI_STATE_PIN, RX_LOW);
    return (roveDynamixel_CatchErrorMSG());

}//end fnctn roveDynamixel_SendPacketMSG

void roveDynamixel_SendByteMSG(uint8_t dynamixel_id, uint8_t data_byte) {

    uint8_t tiva_pin = roveGetPinNum_ByDeviceId(dynamixel_id);

    //(char*)&  casts low level roveWare stdint _t types to high level char, int, etc ecosystem
    roveUART_Write(tiva_pin, (char*)&data_byte, SINGLE_BYTE);

}//end fnctn roveDynamixel_SendByteMSG

///////////////////////////////////////////////////////Begin Wheel Mode

//wheel mode can be used for wheel-type operation robots since motors of the robot will spin infinitely at angle zero
//Wheel Mode : set to both Angle Limits to zero. The motor spins endlessly
int8_t roveDynamixel_SetWheelModeCFG(uint8_t dynamixel_id) {

    //wheel Mode both are 0
    uint8_t angle_limit_low_byte = 0;
    uint8_t angle_limit_high_byte = 0;
    uint8_t data[]={AX12_IMMEDIATE_CMD, AX12_SET_MODE, angle_limit_low_byte, angle_limit_high_byte};
    return roveDynamixel_SendPacketMSG(dynamixel_id, data, sizeof(data));

}//end fnctn roveDynamixel_WheelModeCFG

//Schema

//range: 0~2047( 0X7FF)
//unit:  0.1%.

//If a value in the range of 0~1023 is used, "Full Stop" by setting to 0 while rotating to CCW direction
//If a value in the range of 1024~2047 is used, "Full Stop" by setting to 1024 while rotating to CW direction
//the 10th bit is the direction bit to control the CW /CCW rotation direction
//technically wheel mode sets control of the motor output, not the 'actual' speed of the wheel

//i.e. 512 means 50% of max motor output

//TODO remove 'spin_wheel_direction' arg1??   ->...arg2 could be negative??
int8_t roveDynamixel_SpinWheelCMD(uint8_t dynamixel_id, uint8_t spin_wheel_direction, uint16_t wheel_speed) {

    if(spin_wheel_direction == CLOCKWISE) {
        wheel_speed += (1 >> 10);
    }//end if

    uint8_t speed_low_byte = wheel_speed;
    uint8_t speed_high_byte = wheel_speed >> 8;
    uint8_t data[]={AX12_IMMEDIATE_CMD, AX12_TARGET_SPEED_REG, speed_low_byte, speed_high_byte};
    return roveDynamixel_SendPacketMSG(dynamixel_id , data,sizeof(data));

}//end fnctn roveDynamixel_RotateWHEELSpeedCMD


///////////////////////////////////////////////////////End Wheel Mode


/////////////////////////////////////////////////////Begin Joint Mode

//joint mode can be used for multi-joint robot since the joint mode can be controlled to go at specific speed to specific angles

//range: 0~1023 (0X3FF)
//unit  0.111rpm

//0, is actually max rpm of the motor is used without controlling the speed
//If it is 1023, it is about 114rpm
//For example, if it is set to 300, it is about 33.3 rpm.

//Joint Mode : set to Angle Limit anything other than zero??
int8_t roveDynamixel_SetJointModeCFG(uint8_t dynamixel_id) {

    //set speed zero? homing function function goal rotation to zero?
    roveDynamixel_SpinWheelCMD(dynamixel_id, COUNTERCLOCKWISE, ZERO_SPEED);

    //SetEndless goal rotation...wheel mode is from TODO 0 to 4096? Multi Turn mode on the MX?
    uint8_t angle_limit_low_byte = 255;
    uint8_t angle_limit_high_byte = 3;
    uint8_t data[]={AX12_IMMEDIATE_CMD, AX12_SET_MODE, angle_limit_low_byte, angle_limit_high_byte};
    return roveDynamixel_SendPacketMSG(dynamixel_id, data, sizeof(data));

}//end fnctn roveDynamixel_WheelModeCFG

            //TODO JOSH REED
            //by only ever writing position WITH speed at the same time...I think this "bug" fixes the savage elect API use case where
            //we keep accidentally ONLY using "SavageElectronics.move()" when in wheel mode...not realizing it has no effect??

//WARNING!! in Wheel Mode, the move() function has no effect!! : joint_position in "wheel mode" is always seen as zero = infinite and ONLY the goal speed matters
//ERROR_STATE: Angle Limit Error Bit (Bit1) staus response set 1 = "Destination Out of Range" and triggers "Alarm LED/Shutdown"

//joint_position (Goal Position): 0 to 1023 (0x3FF) at 0.29 degree
int8_t roveDynamixel_RotateJointCMD(uint8_t dynamixel_id, uint8_t rotate_direction , uint16_t joint_position, uint16_t joint_speed) {
    if(rotate_direction == CLOCKWISE) {
        joint_speed += (1 >> 10);
    }//end if

    //shift split two bytes
    uint8_t position_low_byte = joint_position;
    uint8_t position_high_byte = joint_position >> 8;
    uint8_t speed_low_byte = joint_speed;
    uint8_t speed_high_byte = joint_speed >> 8;
    uint8_t data[]={AX12_IMMEDIATE_CMD, AX12_TARGET_POSITN_REG, position_low_byte, position_high_byte, AX12_TARGET_SPEED_REG, speed_low_byte, speed_high_byte};
    return roveDynamixel_SendPacketMSG(dynamixel_id, data, sizeof(data));

}//end fnctn roveDynamixel_RotateWHEELSpeedCMD


//TODO NEXT : this still stubs out empty success always
int8_t roveDynamixel_CatchErrorMSG() {

    /* Todo: roveAvailable(); UART.h
    int Time_Counter = 0;
    while((availableData() < 5) & (Time_Counter < TIME_OUT)) {
        // Wait for Data
        Time_Counter++;
        delayus(1000);
    }//end while
    while (availableData() > 0) {
        unsigned char Incoming_Byte = readData();
        if ( (Incoming_Byte == 255) & (peekData() == 255) ){
            readData();                                    // Start Bytes
            readData();                                    // Ax-12 ID
            readData();                                    // Length
            unsigned char Error_Byte = readData();                       // Error
            return (Error_Byte);
        }
    }
*/
    // Error Response
    return NO_ERRORS;

}//end fnctn roveDynamixel_CatchErrorMSG

//roveWare 2015 Horizon Drive Motors Robotech slb1300 series
void roveDriveMotor_ByPWM(PWM_Handle motor, int16_t speed) {

    int16_t microseconds;

    //scale down to the final range to be between 1000 and 2000
    microseconds = speed / 2;

    //offset so that 1500 is neutral
    microseconds += 1500;

    //protect the upper bound on motor pulse width
    if (microseconds > 2000) {
        microseconds = 2000;
    }//endif

    //protect the lower bound on motor pulse width
    if (microseconds < 1000) {
        microseconds = 1000;
    }//endif

    rovePWM_Write(motor, microseconds);
    return;

} //endfnct roveDriveMotor_ByPWM



///////////////////////////TODO NEXT

/*
static int receivePacket(unsigned char numBytes)
{
    int result = -1;
    int Time_Counter = 0;
    while((availableData() < numBytes+5) & (Time_Counter < TIME_OUT)){
        Time_Counter++;
        delayus(1000);
    }

    while (availableData() > 0){
        unsigned char Incoming_Byte = readData();
        if ( (Incoming_Byte == 255) & (peekData() == 255) ){
            readData();                            // Start Bytes
            readData();                            // Ax-12 ID
            readData();                            // Length
            if( (unsigned char Error_Byte = readData()) != 0 )   // Error
                return (Error_Byte*(-1));
            if(numBytes==1)
                result = readData();
            else if(numBytes==2)
            {
                char tempByteL=readData();
                char tempByteH=readData();
                result = tempByteH << 8 + tempByteL;
            }
        }
    }
    return result;
}

int reset(unsigned char ID)
{
    char data[]={AX_RESET};
    return sendPacket(ID,data,sizeof(data));
}

int ping(unsigned char ID)
{
    char data[]={AX_PING};
    return sendPacket(ID,data,sizeof(data));
}

int setID(unsigned char ID, unsigned char newID)
{
    char data[]={AX_WRITE_DATA, AX_ID, newID};
    return sendPacket(ID,data,sizeof(data));
}

int setBD(unsigned char ID, long baud)
{
    unsigned char Baud_Rate = (2000000/baud) - 1;
    char data[]={AX_WRITE_DATA, AX_BAUD_RATE, Baud_Rate};
    return sendPacket(ID,data,sizeof(data));
}

void action()
{
    char data[]={AX_ACTION};
    return sendPacket(BROADCAST_ID,data,sizeof(data));
}

int torqueStatus( unsigned char ID, DynamixelStatus Status)
{
    char data[]={AX_WRITE_DATA, AX_TORQUE_ENABLE, (char)Status};
    return sendPacket(ID,data,sizeof(data));
}

int ledStatus(unsigned char ID, DynamixelStatus Status)
{
    char data[]={AX_WRITE_DATA, AX_LED, (char)Status};
    return sendPacket(ID,data,sizeof(data));
}

int readTemperature(unsigned char ID)
{
    char data[]={AX_READ_DATA, AX_PRESENT_TEMPERATURE, AX_BYTE_READ_ONE};
    sendPacket(ID,data,sizeof(data));

    return receivePacket(1);
}

int readPosition(unsigned char ID)
{
    char data[]={AX_READ_DATA, AX_PRESENT_POSITION_L, AX_BYTE_READ_TWO};
    sendPacket(ID,data,sizeof(data));

    return receivePacket(2);
}

int readVoltage(unsigned char ID)
{
    char data[]={AX_READ_DATA, AX_PRESENT_VOLTAGE, AX_BYTE_READ_ONE};
    sendPacket(ID,data,sizeof(data));

    return receivePacket(1);
}

int setTempLimit(unsigned char ID, unsigned char Temperature)
{
    char data[]={AX_WRITE_DATA, AX_LIMIT_TEMPERATURE, Temperature};
    return sendPacket(ID,data,sizeof(data));
}

int setVoltageLimit(unsigned char ID, unsigned char DVoltage, unsigned char UVoltage)
{
    char data[]={AX_WRITE_DATA, AX_DOWN_LIMIT_VOLTAGE, DVoltage, UVoltage};
    return sendPacket(ID,data,sizeof(data));
}

int setAngleLimit(unsigned char ID, int CWLimit, int CCWLimit)
{
    char CW_H,CW_L,CCW_H,CCW_L;
    CW_H = CWLimit >> 8;
    CW_L = CWLimit;
    CCW_H = CCWLimit >> 8;
    CCW_L = CCWLimit;
    //TODO: Not sure if the first byte should be AX_CCW_CW_LENGTH or AX_VL_LENGTH
    char data[]={AX_WRITE_DATA, AX_CW_ANGLE_LIMIT_L, CW_L, CW_H, AX_CCW_ANGLE_LIMIT_L, CCW_L, CCW_H};
    return sendPacket(ID,data,sizeof(data));
}

int setMaxTorque(unsigned char ID, int MaxTorque)
{
    char MaxTorque_H,MaxTorque_L;
    MaxTorque_H = MaxTorque >> 8;
    MaxTorque_L = MaxTorque;
    char data[]={AX_WRITE_DATA, AX_MAX_TORQUE_L, MaxTorque_L, MaxTorque_H};
    return sendPacket(ID,data,sizeof(data));
}

int setSRL(unsigned char ID, DynamixelStatusReturnLevel SRL)
{
    char data[]={AX_WRITE_DATA, AX_RETURN_LEVEL, (char)SRL};
    return sendPacket(ID,data,sizeof(data));
}

int setRDT(unsigned char ID, unsigned char RDT)
{
    char data[]={AX_WRITE_DATA, AX_RETURN_DELAY_TIME, (RDT/2)};
    return sendPacket(ID,data,sizeof(data));
}

int setLEDAlarm(unsigned char ID, unsigned char LEDAlarm)
{
    char data[]={AX_WRITE_DATA, AX_ALARM_LED, LEDAlarm};
    return sendPacket(ID,data,sizeof(data));
}

int setShutdownAlarm(unsigned char ID, unsigned char SALARM)
{
    //TODO: Not sure of packet structure with AX_ALARM_LED and AX_WRITE_DATA
    char data[]={AX_WRITE_DATA, AX_ALARM_SHUTDOWN, SALARM};
    return sendPacket(ID,data,sizeof(data));
}

int setCMargin(unsigned char ID, unsigned char CWCMargin, unsigned char CCWCMargin)
{
    char data[]={AX_WRITE_DATA, AX_CW_COMPLIANCE_MARGIN, CWCMargin, AX_CCW_COMPLIANCE_MARGIN, CCWCMargin};
    return sendPacket(ID,data,sizeof(data));
}

int setCSlope(unsigned char ID, unsigned char CWCSlope, unsigned char CCWCSlope)
{
    char data[]={AX_WRITE_DATA, AX_CW_COMPLIANCE_SLOPE, CWCSlope, CCWCSlope};// AX_CCW_COMPLIANCE_SLOPE
    return sendPacket(ID,data,sizeof(data));
}

int setPunch(unsigned char ID, int Punch)
{
    char Punch_H,Punch_L;
    Punch_H = Punch >> 8;
    Punch_L = Punch;
    char data[]={AX_WRITE_DATA, AX_PUNCH_L, Punch_L, Punch_H};
    return sendPacket(ID,data,sizeof(data));
}

int moving(unsigned char ID)
{
    char data[]={AX_READ_DATA, AX_MOVING, AX_BYTE_READ_ONE};
    sendPacket(ID,data,sizeof(data));

    return receivePacket(1);
}

int lockRegister(unsigned char ID)
{
    char data[]={AX_WRITE_DATA, AX_LOCK, LOCK};
    return sendPacket(ID,data,sizeof(data));
}

int RWStatus(unsigned char ID)
{
    char data[]={AX_READ_DATA, AX_REGISTERED_INSTRUCTION, AX_BYTE_READ_ONE};
    sendPacket(ID,data,sizeof(data));

    return receivePacket(1);
}

int readSpeed(unsigned char ID)
{
    char data[]={AX_READ_DATA, AX_PRESENT_SPEED_L, AX_BYTE_READ_TWO};
    sendPacket(ID,data,sizeof(data));

    return receivePacket(2);
}

int readLoad(unsigned char ID)
{
    char data[]={AX_READ_DATA, AX_PRESENT_LOAD_L, AX_BYTE_READ_TWO};
    sendPacket(ID,data,sizeof(data));

    return receivePacket(2);
}

*/



/*
static int sendPacket(unsigned char ID, char* data, int dataByteCount);
static int receivePacket(unsigned char numBytes);
static int read_error(void);

void beginDirectionPin(long baud, unsigned char directionPin);
void beginBaud(long baud);
void end(void);

int reset(unsigned char ID);
int ping(unsigned char ID);

int setID(unsigned char ID, unsigned char newID);
int setBD(unsigned char ID, long baud);

int move(unsigned char ID, int Position);
int moveSpeed(unsigned char ID, int Position, int Speed);
int setEndless(unsigned char ID,DynamixelStatus Status);
int turn(unsigned char ID, DynamixelDirection Direction, int Speed);
int moveRW(unsigned char ID, int Position);
int moveSpeedRW(unsigned char ID, int Position, int Speed);

void action(void);

int setTempLimit(unsigned char ID, unsigned char Temperature);
int setAngleLimit(unsigned char ID, int CWLimit, int CCWLimit);
int setVoltageLimit(unsigned char ID, unsigned char DVoltage, unsigned char UVoltage);
int setMaxTorque(unsigned char ID, int MaxTorque);
int setSRL(unsigned char ID, DynamixelStatusReturnLevel SRL);
int setRDT(unsigned char ID, unsigned char RDT);
int setLEDAlarm(unsigned char ID, unsigned char LEDAlarm);
int setShutdownAlarm(unsigned char ID, unsigned char SALARM);
int setCMargin(unsigned char ID, unsigned char CWCMargin, unsigned char CCWCMargin);
int setCSlope(unsigned char ID, unsigned char CWCSlope, unsigned char CCWCSlope);
int setPunch(unsigned char ID, int Punch);

int moving(unsigned char ID);
int lockRegister(unsigned char ID);
int RWStatus(unsigned char ID);

int readTemperature(unsigned char ID);
int readVoltage(unsigned char ID);
int readPosition(unsigned char ID);
int readSpeed(unsigned char ID);
int readLoad(unsigned char ID);

int torqueStatus(unsigned char ID, DynamixelStatus Status);
int ledStatus(unsigned char ID, DynamixelStatus Status);

//roveWare 2015 Horizon Robot Arm Shoulder Prismatic Joint TODO series
void rovePolulu_DriveLinAct(int tiva_pin, int16_t speed){

    linear_actuator_struct linear_actuator;

    if( speed > 0 ){

        linear_actuator.command_byte = LIN_ACT_FORWARD;

    }else{

        linear_actuator.command_byte = LIN_ACT_REVERSE;

    }//end if


    //bit of an ugly hack to have called this whole int16_t speed
    linear_actuator.speed = (uint8_t)speed;

    roveUART_Write(tiva_pin, (char*)&linear_actuator, sizeof(linear_actuator) );

    return;

}//endfnctn roveDrivePolulu_LinActCmd*/
