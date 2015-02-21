/*
 * roveTcpHandler.c
 *
 *  Created on: Jan 22, 2015
 *      Author: Owen Chiaventone, Judah Schad, Connor Walsh
 *
 *  Deals with maintaining a connection to the base station
 *  more detailed documentation is available on the motherboard wiki
 */

#include "roveTcpHandler.h"

Void roveTcpHandler(UArg arg0, UArg arg1)
{

	System_printf("Function roveTcpHandler() Task Created\n");
	System_flush();

	//init socket file env, json parser, msg structs

	fdOpenSession(TaskSelf());

    int                clientfd = 0;
    struct sockaddr_in localAddr;

	int                connect_success = 0;
    int                connectedFlag = NOT_CONNECTED;

    char incomingBuffer[TCPPACKETSIZE];
    char outgoinggBuffer[TCPPACKETSIZE];

    int                bytesSent = 0;
    int                bytesReceived = 0;

    int    			   KEEPALIVE_SIZE = 9;
    struct timeval     timeout;

    char JsonBuffer[JSON_BUFFER_SIZE];

    int                 JsonBytesRecvd = 0;

   	int 				json_value_string_index = 19;
   	char 				is_end_of_value = 0;
   	int 				value_index = 0;
   	int 				index = 0;

    char Id[5];
    char Value[10];

    int 				cmd_value = 0;
	uint8_t 			value_byte;

    base_station_msg_struct  fromBaseCmd;

    base_station_msg_struct  toBaseTelem;

    System_printf("Environment Instantiated\n");
    System_flush();

    //Task loops for ever

    //It sleeps on full Mailbox_post to roveCommandController Task

    //It awakes on full Mailbox_pend frome roveTelemController Task

    //It only exits on error

    while(1){

    	//init socket

    	System_printf("Attempting to create a Socket()\n");
    	System_flush();

    	clientfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    	//flag bad socket

		if(clientfd == INVALID_SOCKET){

			System_printf("Failed Socket() create (src = socket()) (%d)\n",fdError());
			System_flush();

    	}//endif(clientfd == INVALID_SOCKET)

		//init socket config struct

		memset(&localAddr, 0, sizeof(localAddr) );

		//config socket

		localAddr.sin_family = AF_INET;
		localAddr.sin_port = htons(TCPPORT);

		System_printf("Assigning Target IP Address\n");

		inet_pton(AF_INET, RED_IP, &localAddr.sin_addr);

		timeout.tv_sec = 3600;
		timeout.tv_usec = 0;

		setsockopt(clientfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout) );
		setsockopt(clientfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout) );

		//connect socket

    	System_printf("Attempting to Connect() new connection\n");
    	System_flush();

		connect_success = connect(clientfd, (PSA)&localAddr, sizeof(localAddr) );

		//flag failed connection

    	if(connect_success < 0){

    		connectedFlag = NOT_CONNECTED;

    		System_printf("Error: socket Accept() failed (src = connect())\n");
    		System_flush();

    	}else{

    		connectedFlag = CONNECTED;

    		System_printf("Connected to RED\n");
    		System_flush();

    	}//endif(connect_success < 0)

    	//loop to recieve, break to attempt reconnect

    	while(connectedFlag == CONNECTED){

    		System_printf("Connected. Waiting for data\n");
    	    System_flush();

    		bytesReceived = recv(clientfd, incomingBuffer, TCPPACKETSIZE, 0);

			//flag lost connection

    		if(bytesReceived < 0){

    			connectedFlag = NOT_CONNECTED;

    			System_printf("Connection lost. (src = recv)\n");
    			System_flush();

    		}else{

    			//connected

    			//detect Json by beginning { in buffer

    			if(incomingBuffer[0] == '{'){

    				System_printf("Json Detected\n");
    				System_flush();

    				//Store { and null termination in json buffer

    				JsonBuffer[JsonBytesRecvd] = incomingBuffer[0];
    				JsonBuffer[JsonBytesRecvd+1] = '\0';

    				//break for Json by ending {

    				//break for lost connection again

    				while( (incomingBuffer[0] != '}') && !(bytesReceived < 0) ){

    		    		bytesReceived = recv(clientfd, incomingBuffer, TCPPACKETSIZE, 0);

    		    		//one char incoming, fill Json Buff by each char

    		    		JsonBytesRecvd++;

    		    		JsonBuffer[JsonBytesRecvd] = incomingBuffer[0];
    		    		JsonBuffer[JsonBytesRecvd+1] = '\0';

    				}//endwhile( (incomingBuffer[0] != '}') && !(bytesReceived < 0) )

    				System_printf("Finished Recving Json\n");

    			}//endif(incomingBuffer[0] == '{')

    			//prime Json Parser

    			index = 0;
    			cmd_value = 0;
				is_end_of_value = 0;
				value_index = 0;
				json_value_string_index = 19;

				//peel off ID

    			Id[0] = JsonBuffer[6];
    			Id[1] = JsonBuffer[7];
    			Id[2] = JsonBuffer[8];
    			Id[3] = JsonBuffer[9];
    			Id[4] = '\0';

    			//convert ID

    			cmd_value = atoi(Id);

    			Value[0] = '\0';
    			is_end_of_value = 0;
    			value_index = 0;
    			json_value_string_index = 19;

    			//convert the rest of the values

    			while(is_end_of_value == 0){

    				if(JsonBuffer[json_value_string_index] == '}'){

    					is_end_of_value = 1;

    					Value[value_index] = '\0';

    				}else{

    					Value[value_index] = JsonBuffer[json_value_string_index];

    					json_value_string_index++;

    					value_index++;

    				}//endif(JSON_string_buf[json_value_string_index] == '}')

    			}//endwhile(is_end_of_value == 0)

    			//convert full Value

    			value_byte = atoi(Value);

    			//pass to ID and Value to Command struct

    			fromBaseCmd.id = cmd_value;
    			fromBaseCmd.value = value_byte;

			System_printf("Received data: %c\n", incomingBuffer[0]);
			System_flush();

			//pass struct through to commandThread

			//implicitly sleeps to allow the next Task to awake while Mailbox is full

			Mailbox_post(fromBaseStationMailbox, &fromBaseCmd, BIOS_WAIT_FOREVER);

			//TODO Event handler for Telem Send
/*
			bytesSent = send(clientfd, "keepalive", KEEPALIVE_SIZE, 0);

			if (bytesSent < 0 || bytesSent != KEEPALIVE_SIZE) {

				System_printf("Error: send failed.\n");

				connectedFlag = NOT_CONNECTED;

				//Task_sleep(2000);	-> i++

				bytesSent = send(clientfd, "keepalive", KEEPALIVE_SIZE, 0);

				if (bytesSent < 0 || bytesSent != KEEPALIVE_SIZE) {

					System_printf("Error: send failed.\n");

					connectedFlag = NOT_CONNECTED;

				Mailbox_pend(toBaseStationMailbox, &toBaseTelem, BIOS_WAIT_FOREVER);

				}//endif (bytesSent < 0 || bytesSent != KEEPALIVE_SIZE)

			}//endif (bytesSent < 0 || bytesSent != KEEPALIVE_SIZE)
*/
    	}//endif(bytesReceived < 0)

    } //endwhile(connectedFlag == CONNECTED)

    //If execution reaches this point, then the connection has broken

    close(clientfd);

} //end while(1)


//---- postcondition: Execution will not reach this state unless a serious error occurs

// Close the socket file env

fdCloseSession(TaskSelf());

System_printf("Tcp Handler Task Exit\n");
System_flush();

//exit Task

return;

}//end Function roveTcpHandler() Task
