// roveStructs.h
//
// first created:
//
// 04_20_2014_Keenan Johnson
//
// last edited:
//
//02_25_2015_Judah Schad_jrs6w7@mst.edu
//03_08_2015 Connor Walsh cwd8d@mst.edu ~ add stubs for RoveComm structs

#pragma once

#ifndef ROVESTRUCTS_H_
#define ROVESTRUCTS_H_

//includes globally scoped Texas Instruments (TI) header files

#include "../RoverMotherboardMain.h"

//size in bytes of longest command that can be recieved from the base station

#define MAX_COMMAND_SIZE 30

#define MAX_TELEM_SIZE 30


typedef enum DeviceID
{

	//TODO reserve zero and open bracket (for JSON)

	onenull_device = 111,
	test_device = 99,
	motor_left = 100,
	motor_right = 101,
	robot_arm = 102

} DeviceID;

typedef struct base_station_msg_struct{

	DeviceID id;
	char value[MAX_COMMAND_SIZE];

} base_station_msg_struct, *base_msg;

struct test_device_data_struct{

	DeviceID  id;
	uint8_t value;

}__attribute__((packed));

//normally the compiler implicitly optimizes memory allocations for member variables by padding to the nearest 32 bits

//attribute__((packed)) explicitly overides this and is necessary because the TI board is 32 bit and the ATMegas are 8 bit

// TODO remove last years structs, they will not be used for new RoveComm

struct motor_control_struct{

	//0 for open loop control, non-zero for closed-loop

	uint8_t value;

}__attribute__((packed));

struct gps_data_struct{

  uint8_t fix;
  uint16_t latitude_whole;
  uint16_t latitude_frac;
  uint16_t longitude_whole;
  uint16_t longitude_frac;
  uint16_t altitude_whole;
  uint16_t altitude_frac;
  uint8_t lat_dir;
  uint8_t lon_dir;
  uint8_t satellites;

}__attribute__((packed));

struct bms_data_struct{

  uint16_t volt0;
  uint16_t temp0;
  uint16_t volt1;
  uint16_t temp1;
  uint16_t volt2;
  uint16_t temp2;
  uint16_t volt3;
  uint16_t temp3;
  uint16_t volt4;
  uint16_t temp4;
  uint16_t volt5;
  uint16_t temp5;
  uint16_t volt6;
  uint16_t temp6;
  uint16_t main_bat_cur;
  uint16_t main_bat_volt;

}__attribute__((packed));

struct arm_control_struct{

	uint8_t reset;
	uint8_t wristUp;
	uint8_t wristDown;
	uint8_t wristClockWise;
	uint8_t wristCounterClockWise;
	uint8_t elbowUp;
	uint8_t elbowDown;
	uint8_t elbowClockWise;
	uint8_t elbowCounterClockWise;
	uint8_t actuatorForward;
	uint8_t actuatorReverse;
	uint8_t baseClockWise;
	uint8_t baseCounterClockWise;

}__attribute__((packed));

struct gripper_control_struct{

	uint8_t grip_cmd;

}__attribute__((packed));

struct drill_Controls{

  //drill ctrl

  uint8_t goalSpeed;
  uint8_t direction;

  //thermo ctrl

  uint8_t heaterPower;
  uint8_t thermoReadings;

  //gas ctrl

  uint8_t sensorPower;

  //readings will only be updated when true

  uint8_t gasReadings;

}__attribute__((packed));

struct drill_Telemetry{

  //gas data

  uint16_t hydrogenReading;
  uint16_t methaneReading;
  uint16_t ammoniaReading;

  //thermo data

  float temp;

  //drill data

  uint8_t actualSpeed;
  uint16_t goalCurrent;
  uint16_t actualCurrent;

}__attribute__((packed));

struct science_payload_control_struct{

  //1 for on, 0 for off

  uint8_t lb395;

  //1 for on, 0 for off

  uint8_t lb440;

  //position. 255 for open, 0 for close

  uint8_t doorserv;

}__attribute__((packed));

struct lighting_board_struct{

  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t pwm1;
  uint8_t pwm2;
  uint8_t pwm3;
  uint8_t d1;
  uint8_t d2;

}__attribute__((packed));

struct camera_control_struct{

  uint8_t pitch;
  uint8_t yaw;
  uint8_t roll;
  uint8_t mode;

}__attribute__((packed));

struct power_board_telem{

  uint16_t ambientTemperature;
  uint16_t busAVoltage;
  uint16_t busBVoltage;
  uint16_t busCVoltage;
  uint16_t inputVoltage;
  uint16_t inputCurrent;

}__attribute__((packed));

// These structs will be used for the New RovComm Protocol
// Each struct will start with an identifier byte that will be used to determine which struct follows

struct mobo_identify_req // Sent from mobo to device to request identify
{
	uint8_t struct_id;

}__attribute__((packed));

struct dev_identify_reply  // Sent from device to mobo to ack request for identification
{
	uint8_t struct_id;
	uint8_t device_id;

}__attribute__((packed));

struct mobo_begin_op_req  // Sent from mobo to tell device to begin operating mode
{
	uint8_t struct_id;

}__attribute__((packed));

struct dev_begin_op_reply // Sent from dev to mobo to acknoledge request and begin operation mode
{
	uint8_t struct_id;
	uint8_t device_id; // this field may not be necessary

}__attribute__((packed));

struct mobo_telem_req // sent from mobo to device to request telemetry data
{
	uint8_t struct_id;

}__attribute__((packed));

struct dev_command_reply // sent from device to mobo to acknoledge command received
{
	uint8_t struct_id;

}__attribute__((packed));

// TODO determine what data is needed, current data is last years implementation
struct dev_robo_arm_command // sent from mobo to roboticArm to control the peripheral
{
	uint8_t struct_id;
	uint8_t reset;
	uint8_t wristUp;
	uint8_t wristDown;
	uint8_t wristClockWise;
	uint8_t wristCounterClockWise;
	uint8_t elbowUp;
	uint8_t elbowDown;
	uint8_t elbowClockWise;
	uint8_t elbowCounterClockWise;
	uint8_t actuatorForward;
	uint8_t actuatorReverse;
	uint8_t baseClockWise;
	uint8_t baseCounterClockWise;

}__attribute__((packed));

struct dev_gripper_command // Sent from mobo to gripper to control the peripheral
{
	uint8_t struct_id;
	uint8_t grip_cmd;

}__attribute__((packed));

struct dev_drill_command // Sent from mobo to drill to control the peripheral
{
	uint8_t struct_id;
	uint8_t grip_cmd;
	//drill ctrl
	uint8_t goalSpeed;
	uint8_t direction;
	//thermo ctrl
	uint8_t heaterPower;
	uint8_t thermoReadings;
	//gas ctrl
	uint8_t sensorPower;
	//readings will only be updated when true
	uint8_t gasReadings;

}__attribute__((packed));

#endif // ROVESTRUCTS_H_
