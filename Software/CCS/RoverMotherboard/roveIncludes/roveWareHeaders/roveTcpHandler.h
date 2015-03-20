// roveTcpHandler.h
//
// first created:
//
// 01_22_2015_Owen_Chiaventone
//
// Refactor:
// 02_24_2015_Judah Schad_jrs6w7@mst.edu
//
// Introduce abstraction and message types
// 03_12_2015 Owen Chiaventone

#pragma once

#ifndef ROVETCPHANDLER_H_
#define ROVETCPHANDLER_H_

//includes globally scoped Texas Instruments (TI) header files
#include "../RoverMotherboardMain.h"

//MRDesign Team::roveWare::		roveCom and RoveNet services headers
#include "../mrdtRoveWare.h"

//When data is recieved it goes into the fromBaseStationMailbox as RoveNet recieve struct base_station_msg_struct

//When data is sent it goes into the toBaseStationMailbox mailbox RoveNet send switching on the enum device structs and sizeof()

Void roveTcpHandler(UArg arg0, UArg arg1);


// ------- Network Abstraction Layer -----------//

//If your function needs to access the network, it should
// go through this abstraction layer


struct NetworkConnection
{
	int socketFileDescriptor;
	bool isConnected;
} NetworkConnection;

//These functions are declared static to preserve the scope of the calling
// function. The file descriptor environment is only available from the scope
// of the function that created it, and pointers to it don't seem to work.

//Recieves <bytes> bytes from the specified network connection and places them
// in the specified buffer
// Pre: network connection is valid
// Post: Buffer is filled with bytes from network
// Error: connection.isConnected set to false
//        -1 returned
//
static int roveRecv(struct NetworkConnection* connection, char* buffer, int bytes);

static int roveSend(struct NetworkConnection* connection, char* buffer, int bytes);

static bool roveCloseConnection(struct NetworkConnection*);

//Creates a connection and stores the result in the specified NetworkConnection
//Uses the RED_IP and RED_SOCKET constants
static bool attemptToConnect(struct NetworkConnection* connection);

// -------- Network Message Parsers --------//

//Pre: Next bytes in network queue are a rovecomm message
//     Should only be called if the message type indicates this
//Post:Message placed in fromBaseStation mailbox
static bool parseRoverCommandMessage(struct NetworkConnection* connection);

// -------- Base Station Command Identifiers -------//

//See https://github.com/MST-MRDT/Motherboard/wiki/Base-Station-Communication-Protocol
//for more information.

#define CONSOLE_COMMAND		0x00
#define SYNCHRONIZE_STATUS	0x01
#define COMMAND_METADATA	0x02
#define TELEM_METADATA		0x03
#define ERROR_METADATA		0x04
#define ROVER_COMMAND		0x05
#define ROVER_TELEM			0x06
#define ROVER_ERROR			0x07

#define JSON_START_BYTE '{'

#endif // ROVETCPHANDLER_H_
