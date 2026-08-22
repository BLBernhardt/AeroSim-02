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
typedef struct GD_IN_t
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
} GD_IN_t;
#pragma pack(pop)


#pragma pack(push, 1) // Ensure no padding for network/message use
typedef struct GD_OUT_t
{
 	uint32_t frame_cnt; 

  	float 	Stick_P;
  	float 	Stick_R;
  	float 	Stick_Y;  
  	float 	Stick_E;
  	
  	float R_cmd;
  	float P_cmd;
  	float Y_cmd;



  	float Thr_Cmd; // throttle 				[  0, +1 ].
  	float Elv_Cmd; // elevator deflection 	[ -1, +1 ].
  	float Ail_Cmd;  // aileron deflection 	[ -1, +1 ].
  	float Rud_Cmd;   // Grudder deflection 	[ -1, +1 ].
 
  	int	Gear_Cmd; 			// Gear Cmd 0:UP  1:DOWN

  	float Elv_Trim; // elevator deflection 	[ -1, +1 ].
  	float Ail_Trim;  // aileron deflection 	[ -1, +1 ].
  	float Rud_Trim;   // Grudder deflection 	[ -1, +1 ].

} GD_OUT_t;
#pragma pack(pop)



#endif

//===============================================================================
// EOF
