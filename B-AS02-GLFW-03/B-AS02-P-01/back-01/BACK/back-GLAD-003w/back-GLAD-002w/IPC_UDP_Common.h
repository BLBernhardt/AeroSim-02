//
//=============================================================================================
// IPC_UDP_Common.h
//=============================================================================================

#ifndef IPC_UDP_COMMON_H
#define IPC_UDP_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUFFER_SIZE 1024

// Server ports
#define SERVER_TX_PORT 5001
#define CLIENT_TX_PORT 5002

#define DEFAULT_SERVER_IP "127.0.0.1"
#define DEFAULT_CLIENT_IP "127.0.0.1"

#define MAXETRIES 30
#define RETRY_DELAY_US 200000
#define BUFFER_SIZE 1024

#define MAX_TEXT_LEN 128


#pragma pack(push, 1)
typedef struct Server_Tx_t
{
 	float Alpha;
 	float Beta;
 	float Pitch;
 	float Yaw;
 	float Roll;
 	float AirSpeed;
 	float Alt;
 	float Climb;
 	float dt;
 	uint32_t frame_cnt;
 	int32_t hours;
 	int32_t min;
 	int32_t sec;
 	uint32_t mSec;
 	uint8_t Stick_Enable; // Consistent 1-byte field
 	char key;
 	char spinner[4];
} Server_Tx_t;
#pragma pack(pop)




//===============================================================================
// Data Structure Definition
//===============================================================================

#pragma pack(push, 1) // Ensure no padding for network/message use
typedef struct Client_Tx_t
{
 	uint32_t frame_cnt; 
 	float pitch_cmd; 
 	float roll_cmd; 
 	float yaw_cmd; 
 	float thrtl_cmd; 
 	float spare1; 
 	float spare2; 
 	float spare3; 
} Client_Tx_t;
#pragma pack(pop)



#endif

//===============================================================================
// EOF
