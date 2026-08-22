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
#define DEFAULT_IP "127.0.0.1"

#define MAXETRIES 30
#define RETRY_DELAY_US 200000
#define BUFFER_SIZE 1024

#define MAX_TEXT_LEN 128

//  Server is the simulation process              TM_Stx_t
//  Client is the Display and Controls process    TM_Ctx_t


#pragma pack(push, 1)
typedef struct GD_IN_t
{
 	float 		Alpha;
 	float 		Beta;
 	float 		Pitch;
 	float 		Yaw;
 	float 		Roll;
 	float 		AirSpeed;
 	float 		Alt;
 	float 		Climb;
 	float 		dt;
 	uint32_t 	frame_cnt;
 	int32_t 	hours;
 	int32_t 	min;
 	int32_t 	sec;
 	uint32_t 	mSec;
 	char 		key;
 	double 		Position_X;
 	double 		Position_Y;
 	float  		G_meter;	
 	uint8_t 	Stick_Enable_b; 		// 1 Enable, 	0 Disable		
} GD_IN_t;
#pragma pack(pop)



#pragma pack(push, 1)
typedef struct TM_Stx_t
{
 	float 		Alpha;
 	float 		Beta;
 	float 		Pitch;
 	float 		Yaw;
 	float 		Roll;
 	float 		AirSpeed;
 	float 		Alt;
 	float 		Climb;
 	float 		dt;
 	uint32_t 	frame_cnt;
 	int32_t 	hours;
 	int32_t 	min;
 	int32_t 	sec;
 	uint32_t 	mSec;
 	char 		key;
 	double 		Position_X;
 	double 		Position_Y;
 	float  		G_meter;			
} TM_Stx_t;
#pragma pack(pop)


#pragma pack(push, 1) // Ensure no padding for network/message use
typedef struct TM_Ctx_t
{
 	uint32_t 	frame_cnt; 
 	float 		Elv_Cmd_u; 			// +1.0 to -1.0
 	float 		Ail_Cmd_u; 			// +1.0 to -1.0
 	float 		Rud_Cmd_u; 			// +1.0 to -1.0
 	float 		Thr_cmd_u; 			// +1.0 to    0
 	float 		Elv_Trim_Cmd_u; 	// +1.0 to -1.0
 	float 		Ail_Trim_Cmd_u; 	// +1.0 to -1.0
 	float 		Rud_Trim_Cmd_u; 	// +1.0 to -1.0
 	float 		Flaps_Cmd_i;      	// 1,2,3,4,5  
    float 		Speedbrake_Cmd_u; 	// +1.0 to    0
 	int	  		Gear_Cmd_b;			// 1 Down, 		0 Up
 	uint8_t 	Stick_Enable_b; 		// 1 Enable, 	0 Disable
} TM_Ctx_t;
#pragma pack(pop)



#endif

//===============================================================================
// EOF





#if 0

#pragma pack(push, 1) // Ensure no padding for network/message use
typedef struct TM_Ctx_t
{
 	uint32_t 	frame_cnt; 
 	float 		Elv_Cmd_u; 			// +1.0 to -1.0
 	float 		Ail_Cmd_u; 			// +1.0 to -1.0
 	float 		Rud_Cmd_u; 			// +1.0 to -1.0
 	float 		Thr_cmd_u; 			// +1.0 to    0
 	float 		Elv_Trim_Cmd_u; 	// +1.0 to -1.0
 	float 		Ail_Trim_Cmd_u; 	// +1.0 to -1.0
 	float 		Rud_Trim_Cmd_u; 	// +1.0 to -1.0
 	float 		Flaps_Cmd_i;      	// 1,2,3,4,5  
    float 		Speedbrake_Cmd_u; 	// +1.0 to    0
 	int	  		Gear_Cmd_b;			// 1 Down, 		0 Up
 	uint8_t 	Stick_Enable; 		// 1 Enable, 	0 Disable
} TM_Ctx_t;
#pragma pack(pop)

#endif




























