//	TODO: Port To Fresh Build (using TI example:			This version educational practice not for distro)
//
// roveTcpHandler.c
//
// first created:
//
// 01_22_2015_Owen_Chiaventone
//
// last edited:
//
//02_24_2015_Judah Schad_jrs6w7@mst.edu

//this implements a single function BIOS thread that acts as the RoverMotherboard.cfg roveTcpHandlerTask handle
//
//recieves base station commands and send device telemetry using tcp ip bsd sockets

#include "roveIncludes/roveWareHeaders/roveTcpHandler.h"

//BIOS_start inits this as the roveTcpHandlerTask Thread

//this is a RoverMotherboard.cfg object::		roveTcpHandlerTask		::		priority 1, vital_flag = t, 2048 persistent private stack

Void roveTcpHandler(UArg arg0, UArg arg1){

	//init socket file environment

	fdOpenSession((void*)TaskSelf());

	//init tcp socket handle

    int              serverfd = 0;

    struct 	         sockaddr_in server_addr;

    //init flags for socket status handles

	int			     bytesReceived =0;

	int              connectedFlag = NOT_CONNECTED;

	//a copy is placed in the mailbox when a message is recieved.

	//TI ndk defined timeout

	struct 			 timeval timeout;

    //init RoveNet recieve struct

    base_station_msg_struct fromBaseCmd;


//TODO-> Move to Cmd Cntrl

	extern UART_Handle uart0;
	extern UART_Handle uart1;
	extern UART_Handle uart2;
	extern UART_Handle uart3;
	extern UART_Handle uart4;
	extern UART_Handle uart5;
	extern UART_Handle uart6;
	extern UART_Handle uart7;

	//init and clean RoveCom uart send struct

	struct motor_control_struct motor_control_struct;

	motor_control_struct.value = 0;


	//the task loops for ever

	//sleeps on the Mailbox_post to roveCommandController Task

	//only exits from BIOS_start, on error state

    ms_delay( 1 );

    System_printf("roveTCPHandler 		init! \n");
    System_printf("\n");
    System_printf("\n");
    System_flush();

    ms_delay( 1 );

    while(1){

    	//init socket, if the socket fails any point we break, close(clientfd), and reloop to re-init a fresh clientfd

    	serverfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

       	//flag for bad socket

		if(serverfd == -1){

			System_printf("Failed Socket() create roveClientfd (src = socket()) (%d)\n",fdError() );
			System_flush();

    	}//endif:	(roveServerlocalfd == -1)

		ms_delay( 1 );

		System_printf("TCPHandler:			socket init success! \n");
		System_printf("\n");
		System_printf("\n");
		System_flush();

		ms_delay( 1 );

		//init bsd socket config struct

		memset(&server_addr, 0, sizeof(server_addr) );

		//config the socket

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(TCPPORT);

		inet_pton(AF_INET, RED_IP, &server_addr.sin_addr);

		timeout.tv_sec = 6;
		timeout.tv_usec = 0;

		setsockopt(serverfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout) );
		setsockopt(serverfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout) );

		//connect to Red

		ms_delay( 1 );
	    System_printf("TCPHandler:				 	Trying to connect! \n");
	    System_flush();
	    ms_delay( 10 );

		//connect the socket

		connectedFlag = connect(serverfd, (PSA)&server_addr, sizeof(server_addr) );

		if(connectedFlag == -1){

					System_printf("Error: connect failed.\n");
					System_printf("\n");
					System_printf("\n");
					System_flush();

    	}else{

        		//we are now CONNECTED

        		connectedFlag = CONNECTED;

    			System_printf("Connected to RED! \n");
    			System_flush();

        }//endifelse:	(connect_success < 0)*/

    	//loop to recieve cmds and send telem from and to the base station: if socket breaks, loop breaks and we attempt to reconnect

    	while(connectedFlag == CONNECTED){

    		//clean the structs for Mailbox_post:		.id is enum 	.value is char[MAX_COMMAND_SIZE]

    		fromBaseCmd.id = onenull_device;
    		memset(&fromBaseCmd.value, 0, sizeof(MAX_COMMAND_SIZE) );

    		ms_delay( 1 );
			System_printf("1			Just ONENULLED the RECIEVE ID: %d \n", fromBaseCmd.id);
			System_printf("1			Just NULLED the RECIEVE VALUE: %d \n", fromBaseCmd.value[0]);
			System_flush();
			ms_delay( 1 );

    		//get the tcp packet and store it in the RoveNet fromBaseCmd struct

    		//order is device dependent: Tiva C is little-endian, so this reads indexed to the lsb of fromBaseCmd.id field

    		bytesReceived = recv(serverfd, &(fromBaseCmd.id), 1, 0);

      		ms_delay( 1 );
			System_printf("1			Just got a TCP RECIEVE ID: %d \n", fromBaseCmd.id);
			System_flush();
			ms_delay( 1 );

    		//flag for lost connection when recieving

    		if(bytesReceived == -1){

    			connectedFlag = NOT_CONNECTED;

    			System_printf("Connection lost. (src = recv)\n");
    			System_flush();

    		}else{

    			//we have recieved successfully

						//	the following call copy buffers the packet for roveCmdCntrl Thread, then implicitly task_sleeps roveTcpHandlerTask
						//	finally this call will implicitly awaken the roveCmdCntrlTask Thread to handle the Mailbox.Semaphore
						//	This is a RoverMotherboard.cfg object::		fromBaseStationMailbox		::		 1024, max msg =10

    			//Recieve a variable number of bytes based on the ID sent to us

    			switch(fromBaseCmd.id){

    				case motor_left:

    					bytesReceived = recv(serverfd, &(fromBaseCmd.value), sizeof(struct motor_control_struct), 0);

    					ms_delay( 1 );
						System_printf("1			Just got a TCP REC() motor_left VALUE: %d \n", fromBaseCmd.value[0]);
						System_flush();
						ms_delay( 1 );

						//Mailbox_post(fromBaseStationMailbox, &fromBaseCmd, BIOS_WAIT_FOREVER);

//TODO-> Move this to the roveCmdCntrl

						//*************
						// Drive Left
						//*************

						//roveCom::	enum motor_left::	id = 100

						memset(&motor_control_struct.value, &fromBaseCmd.value, sizeof(struct motor_control_struct) );

						mux_1( 8 );
						mux_2( 7 );
						mux_3( 6 );

						send_struct(uart1, &motor_control_struct, motor_controller);
						send_struct(uart2, &motor_control_struct, motor_controller);
						send_struct(uart3, &motor_control_struct, motor_controller);

						ms_delay( 1 );
						System_printf("3:	Cmd Cntrl Just SENT UART 1, 2, 3! VALUE: %d \n", motor_control_struct.value);
						System_flush();
						ms_delay( 1 );

    				break;

    				case motor_right:

    					bytesReceived = recv(serverfd, &(fromBaseCmd.value), sizeof(struct motor_control_struct), 0);

						ms_delay( 1 );
						System_printf("1			Just got a TCP REC() motor_left VALUE: %d \n", fromBaseCmd.value[0]);
						System_flush();
						ms_delay( 1 );

						//Mailbox_post(fromBaseStationMailbox, &fromBaseCmd, BIOS_WAIT_FOREVER);

						//TODO-> Move this to the roveCmdCntrl

						//*************
						// Drive Right
						//*************

						//roveCom::	enum motor_right::	id = 101

						memset(&motor_control_struct.value, &fromBaseCmd.value, sizeof(struct motor_control_struct) );

						mux_1( 1 );
						mux_2( 2 );
						mux_3( 3 );

						send_struct(uart1, &motor_control_struct, motor_controller);
						send_struct(uart2, &motor_control_struct, motor_controller);
						send_struct(uart3, &motor_control_struct, motor_controller);

						ms_delay( 1 );
						System_printf("3:	Cmd Cntrl Just SENT UART 1, 2, 3! VALUE: %d \n", motor_control_struct.value);
						System_flush();
						ms_delay( 1 );

    				break;

    			}//endswitch:		(fromBaseCmd.id)

    			//flag for lost connection when recieving

				if(bytesReceived == -1){

					connectedFlag = NOT_CONNECTED;

					System_printf("Connection lost. (src = recv)\n");
					System_flush();

    		    }//endif			(bytesReceived == -1)

    		}//endifelse:			(bytesReceived == -1)

			//Mailbox_pend(<&TELEM>);

			//switch(<TELEM.id>){

		    	//the robot arm is the one sending us telemetry

		    	//case test_device:

		     	 	 //bytesSent = send(serverfd,  &TELEM,  sizeof(struct test_device_data_struct), 0);

		    	//break;

	    	//}//endswitch:	(fromBaseCmd.id)

		    //flag for lost connection when sending

    	}//endwhile(connectedFlag == CONNECTED)

    	//If execution reaches this point, then the connection has broken and we will attempt a new socket

    	close(serverfd);

    }//endwhile:	(1)


    //postcondition: execution will not reach this state unless a serious error occurs

    //close the socket file environment

    fdCloseSession((void*)TaskSelf());

    System_printf("Tcp Handler Task Error: Forced Exit\n");
    System_flush();

    //exit Task

    Task_exit();

}//endfnctn:: 	roveTcpHandler() Task Thread
