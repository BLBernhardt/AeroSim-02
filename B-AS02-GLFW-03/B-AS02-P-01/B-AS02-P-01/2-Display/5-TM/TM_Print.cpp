 
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
 #include "../IPC_UDP_Common-A.h"
 

void TM_Print( KB_State_t *kb, TM_Param_t* TMtx, Client_Tx_t* TMrx )
{
        const char spinner[] = { '\\', '|', '/', '-' };	  
#if 0
typedef struct TM_Param_t
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
} TM_Param_t;
#endif

			 TMtx->key =  TMtx->key ?  TMtx->key : ' ';
			 TMtx->spinner[0] =  TMtx->spinner[0] ?  TMtx->spinner[0] : ' ';
			 TMtx->spinner[1] =  TMtx->spinner[1] ?  TMtx->spinner[1] : ' ';
			 TMtx->spinner[2] =  TMtx->spinner[2] ?  TMtx->spinner[2] : ' '; 
			 TMtx->spinner[3] =  TMtx->spinner[3] ?  TMtx->spinner[3] : ' ';


#if 1
        printf("\033[17A");
        
        printf("\033[2K [%c] Frame Count: %06d       TIME: %02d:%02d:%02d.%1u   Key:%c \n",  spinner[ uint( TMtx->frame_cnt/10.0) & 0x03 ],	 TMtx->frame_cnt,  TMtx->hours,  TMtx->min,  TMtx->sec, uint( TMtx->mSec/100),  TMtx->key );    
  		printf( "\t\t-------------------------------------------------------------------------------------------------------------------------\n" );
//               |           |           |           |           |           |           |           |           |           |           |
 		printf( "\t\t|  Alpha    |   Beta    |   Pitch   |    Yaw    |    Roll   |    Vel    |    Alt    |   Climb   |           |           | \n" );
 		printf( "\t\t|  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |           |           |\n",  TMtx->Alpha ,  TMtx->Beta ,  TMtx->Pitch ,  TMtx->Yaw ,  TMtx->Roll ,  TMtx->AirSpeed ,  TMtx->Alt ,  TMtx->Climb  ); 
 		printf( "\t\t-------------------------------------------------------------------------------------------------------------------------\n" );
		printf( "\t\t|  FrameCnt | Stick Enbl|    Key    |     G    |  Spin[1]  |  Spin[2]  |  Spin[3]  |           |           |           | \n" );
 		printf( "\t\t|   %5u   |    %5u  |     %c     |   %c   |     %c     |     %c     |     %c     |           |           |           |\n",  TMtx->frame_cnt ,  TMtx->Stick_Enable ,  TMtx->key ,  TMtx->spinner[0] ,  TMtx->spinner[1] , TMtx->spinner[2] ,  TMtx->spinner[3] ); 
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
  
  		printf( "\t\t-------------------------------------------------------------------------------------------------------------------------\n" );
//                   |           |           |           |           |           |           |           |           |           |           |
 		printf( "\t\t| Frame Cnt | Pitch Cmd | Roll Cmd  |  Yaw Cmd  | Thrtl_Cmd |  spare 1  |  spare 2  |  spare 3  |           |           | \n" );
 		printf( "\t\t|  %8u |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |           |           |\n",  TMrx->frame_cnt ,  TMrx->pitch_cmd ,  TMrx->roll_cmd ,  TMrx->yaw_cmd ,  TMrx->thrtl_cmd   ,  TMrx->spare1 ,  TMrx->spare2 ,  TMrx->spare3   ); 
 		printf( "\t\t-------------------------------------------------------------------------------------------------------------------------\n" );
        printf("\033[2K \n");
        printf("\033[2K \n");
        printf("\033[2K \n");


        fflush(stdout);
#endif

}


//========================================================================================================================
// EOF




