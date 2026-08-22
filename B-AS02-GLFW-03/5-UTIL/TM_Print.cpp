 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <GL/freeglut.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
 
 //#include "0-AeroSim/flightmodel.h"
 #include "../structures.h"
#include  "../IPC_CONFIG.h"
 


void TM_Print( GD_IN_t* GD_in, Cntrls_t* CTRL )
{
		const char spinner[] = { '\\', '|', '/', '-' };	  
#if 0
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
} GD_t;
#endif

//	char key;


	//		 GD_in->spinner[0] =  GD_in->spinner[0] ?  GD_in->spinner[0] : ' ';
	//		 GD_in->spinner[1] =  GD_in->spinner[1] ?  GD_in->spinner[1] : ' ';
	//		 GD_in->spinner[2] =  GD_in->spinner[2] ?  GD_in->spinner[2] : ' '; 
	//		 GD_in->spinner[3] =  GD_in->spinner[3] ?  GD_in->spinner[3] : ' ';
	//		 if( GD_in->key >= 32 && GD_in->key <= 127 )  key = GD_in->key;
			 CTRL->key =  CTRL->key ?  CTRL->key : ' ';
#if 1
		printf("\033[17A");
		
		printf("\033[2K [%c] Frame Count: %06d	   TIME: %02d:%02d:%02d.%1u  key:%c     ",  
				spinner[ uint( CTRL->frame_cnt/10.0) & 0x03 ],	 CTRL->frame_cnt,  GD_in->hours,  GD_in->min,  GD_in->sec, uint( GD_in->mSec/100),  GD_in->key    );	
				printf("\n");
  		printf( "\t\t-------------------------------------------------------------------------------------------------------------------------\n" );
//			   |           |           |           |           |           |           |           |           |           |           |
 		printf( "\t\t|  Alpha    |   Beta    |   Pitch   |    Yaw    |    Roll   |    Vel    |    Alt    |   Climb   |           |           | \n" );
 		printf( "\t\t|  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |           |           |\n",  
 		    		 GD_in->Alpha ,  GD_in->Beta ,  GD_in->Pitch ,  GD_in->Yaw ,  GD_in->Roll ,  GD_in->AirSpeed ,  GD_in->Alt ,  GD_in->Climb  ); 
 		printf( "\t\t-------------------------------------------------------------------------------------------------------------------------\n" );
		printf( "\t\t|  FrameCnt | Stick Enbl|    Key    |            |           |           |           |          |           |           | \n" );
 		printf( "\t\t|   %5u   |    %5u  |     %c     |            |           |           |           |          |           |           |\n",  
 					 CTRL->frame_cnt ,  GD_in->Stick_Enable_b ,  CTRL->key  ); 
 		printf( "\t\t-------------------------------------------------------------------------------------------------------------------------\n" );
		printf("\033[2K \n");
		printf("\033[2K \n");


#if 0
 	uint32_t frame_cnt; 
 	float pitch_cmd; 
 	float roll_cmd; 
 	float yaw_cmd; 
 	float thrtl_cmd; 
 	float spare2; 
 	float spare3; 
#endif 
  
  		printf( "\t\t--------------------------------------------------------------------------------------------------------------------------\n" );
//                   |           |           |           |           |           |           |           |           |           |           |
 		printf( "\t\t| Frame Cnt | Pitch Cmd | Roll Cmd  |  Yaw Cmd  | Thrtl_Cmd |  Gear_Cmd  |  Elv Trim  | Ail Trim  |  Rud Trim  |         | \n" );
 		printf( "\t\t|  %8u |  %8.2f |  %8.2f |  %8.2f |  %8.2f |      %1d     |  %8.2f |   %8.2f |   %8.2f |         |     \n",  
 				CTRL->frame_cnt ,  CTRL->Elv_Cmd_u ,  CTRL->Ail_Cmd_u ,  CTRL->Rud_Cmd_u ,  CTRL->Thr_Cmd_u   ,CTRL->Gear_Cmd_b,  CTRL->Elv_Trim_Cmd_u ,  CTRL->Ail_Trim_Cmd_u ,  CTRL->Rud_Trim_Cmd_u   ); 
 		printf( "\t\t--------------------------------------------------------------------------------------------------------------------------\n" );
    	printf("\033[2K \n");
		printf("\033[2K \n");
		printf("\033[2K \n");


	//	fflush(stdout);
#endif

}


//========================================================================================================================
// EOF




