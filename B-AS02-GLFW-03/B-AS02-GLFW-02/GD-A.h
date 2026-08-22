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
typedef struct GD_t
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
 	double Position_X;
 	double Position_Y;
 	float  G;
} GD_t;
#pragma pack(pop)






#endif

//===============================================================================
// EOF
