//
//=============================================================================================
// TM_G_t-A.h
//=============================================================================================

#ifndef TM_G_H
#define TM_G_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>


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
 	uint8_t 	Stick_Enable; // Consistent 1-byte field
 	char 		key;
 	double 		Position_X;
 	double 		Position_Y;
 	float  		G;
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
 	int	  		Gear_Cmd_b;			// 1 Down, 0 Up
} TM_Ctx_t;
#pragma pack(pop)


#endif

//===============================================================================
// EOF
