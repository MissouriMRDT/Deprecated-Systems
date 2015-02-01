/*
 * roveTcpHandler.h
 *
 *  Created on: Jan 22, 2015
 *      Author: owen
 */

#ifndef ROVETCPHANDLER_H_
#define ROVETCPHANDLER_H_


#include <string.h>

#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drivers/GPIO.h>

/* NDK BSD support */
#include <sys/socket.h>

/* Example/Board Header file */
#include "Board.h"

/* Mailbox Support */
#include <ti/sysbios/knl/Mailbox.h>

/* Json Parsing Utility */
#include "json.h"

/* Source for base_station_command_struct */
#include "structs.h"

#define RED_IP "192.168.1.20"
#define TCPPORT 11000

#define TCPPACKETSIZE 1
#define JSON_BUFFER_SIZE 100
#define NUMTCPWORKERS 3
#define NOT_CONNECTED 0
#define CONNECTED 1

/*
typedef struct MsgObj {
	char message_body[TCPPACKETSIZE];
} MsgObj, *Msg;
*/




//Thread that creates connection and receives data
//When data is recieved it goes into the red_incoming mailbox
//Spawns the roveTcpSender thread when a connection is made
Void roveTcpHandler(UArg arg0, UArg arg1);


//Thread that is spawned when a connection is made
//It pends on mailbox red_outgoing and will send a
//keepalive periodically
Void roveTcpSender(UArg arg0, UArg arg1);

//ConnectToRed attempts to connect to RED
//Pre: File descriptor environment has been initialized in the thread
//Post: A socket with a connection to the base station is allocated in the
//      File descriptor environment. A reference to this is stored in the
//      socket variable.
//      Returns 0 if success, -1 if fail

int attemptToConnect(int *the_socket);

//Parses data in the buffer. Output is placed in output_struct
//Returns
// 1 - Success
int parseJson(base_station_cmd_struct *output_struct, char *JSON_string_buf, int buf_length);

#endif /* ROVETCPHANDLER_H_ */
