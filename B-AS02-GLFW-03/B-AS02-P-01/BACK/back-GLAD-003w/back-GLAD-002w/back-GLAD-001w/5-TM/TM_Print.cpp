 
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
 #include "../IPC_UDP_Common.h"
 

void TM_Print( KB_State_t *kb, Server_Tx_t* TM_sTx, Client_Tx_t* TM_cTx )
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

			 TM_sTx->key =  TM_sTx->key ?  TM_sTx->key : ' ';
			 TM_sTx->spinner[0] =  TM_sTx->spinner[0] ?  TM_sTx->spinner[0] : ' ';
			 TM_sTx->spinner[1] =  TM_sTx->spinner[1] ?  TM_sTx->spinner[1] : ' ';
			 TM_sTx->spinner[2] =  TM_sTx->spinner[2] ?  TM_sTx->spinner[2] : ' '; 
			 TM_sTx->spinner[3] =  TM_sTx->spinner[3] ?  TM_sTx->spinner[3] : ' ';


#if 1
        printf("\033[17A");
        
        printf("\033[2K [%c] Frame Count: %06d       TIME: %02d:%02d:%02d.%1u   Key:%c \n",  spinner[ uint( TM_sTx->frame_cnt/10.0) & 0x03 ],	 TM_sTx->frame_cnt,  TM_sTx->hours,  TM_sTx->min,  TM_sTx->sec, uint( TM_sTx->mSec/100),  TM_sTx->key );    
  		printf( "\t\t-------------------------------------------------------------------------------------------------------------------------\n" );
//               |           |           |           |           |           |           |           |           |           |           |
 		printf( "\t\t|  Alpha    |   Beta    |   Pitch   |    Yaw    |    Roll   |    Vel    |    Alt    |   Climb   |           |           | \n" );
 		printf( "\t\t|  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |           |           |\n",  TM_sTx->Alpha ,  TM_sTx->Beta ,  TM_sTx->Pitch ,  TM_sTx->Yaw ,  TM_sTx->Roll ,  TM_sTx->AirSpeed ,  TM_sTx->Alt ,  TM_sTx->Climb  ); 
 		printf( "\t\t-------------------------------------------------------------------------------------------------------------------------\n" );
		printf( "\t\t|  FrameCnt | Stick Enbl|    Key    |  Spin[0]  |  Spin[1]  |  Spin[2]  |  Spin[3]  |           |           |           | \n" );
 		printf( "\t\t|   %5u   |    %5u  |     %c     |     %c     |     %c     |     %c     |     %c     |           |           |           |\n",  TM_sTx->frame_cnt ,  TM_sTx->Stick_Enable ,  TM_sTx->key ,  TM_sTx->spinner[0] ,  TM_sTx->spinner[1] , 
 		                                                                                                                                                                   TM_sTx->spinner[2] ,  TM_sTx->spinner[3] ); 
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
 		printf( "\t\t|  %8u |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |  %8.2f |           |           |\n",  TM_cTx->frame_cnt ,  TM_cTx->pitch_cmd ,  TM_cTx->roll_cmd ,  TM_cTx->yaw_cmd ,  TM_cTx->thrtl_cmd   ,  TM_cTx->spare1 ,  TM_cTx->spare2 ,  TM_cTx->spare3   ); 
 		printf( "\t\t-------------------------------------------------------------------------------------------------------------------------\n" );
        printf("\033[2K \n");
        printf("\033[2K \n");
        printf("\033[2K \n");


        fflush(stdout);
#endif



#if 0
         printf("Rx: dt:%.3f  Frame:%u  Time:%02d:%02d:%02d.%03u   \n",
               		 TM_sTx->dt, 
               		 TM_sTx->frame_cnt,
               		 TM_sTx->hours, 
               		 TM_sTx->min, 
               		 TM_sTx->sec, 
               		 TM_sTx->mSec ); 
        printf("Rx: alpha: %.3f beta %.3f pitch %.3f yaw %.3f roll %.3f mach %.3f CL: %.3f CL2: %.3f  \n",
  				  	 TM_sTx->Alpha,      
    				 TM_sTx->Beta,
    				 TM_sTx->Pitch,   
    				 TM_sTx->Yaw,     
    				 TM_sTx->Roll,   
    				 TM_sTx->Mach, 
    				 TM_sTx->CL,  
    				 TM_sTx->CL2 );        		              
       printf("Rx: frame:%u SE:%u   key:%c   s-1:%c s-2:%c s-3:%c s-4:%c \n ",  
              		 TM_sTx->frame_cnt,
                   	 TM_sTx->Stick_Enable, 
    			     TM_sTx->key,                   	
    		         TM_sTx->spinner[0],
    				 TM_sTx->spinner[1],
    				 TM_sTx->spinner[2],
    				 TM_sTx->spinner[3] ); 
#endif 

}
//========================================================================================================================

 #if 0
void TM_Print_off( TM_Param_t *TM )
{
 
 int i = 0;
 
// 	if( !Cntrls.abort )
// 	{
        // 2. Clear and reprint each line (combined for safety)
        printf("\033[2k  \n");
        printf("\033[2K %15s: %7.2f   %15s: %7.2f  %15s: %7.2f\n",  			"Pitch", 	 TM_cTx->Pitch, 	"Yaw", 		 TM_cTx->Yaw, 	"Roll", 	 TM_cTx->Roll  									);	
        printf("\033[2K %15s: %7.2f   %15s: %7.2f  %15s: %7.2f\n",  			"CL", 		 TM_cTx->CL, 	"CL2", 		 TM_cTx->CL2, 	"test", 	0.0 											);	
        printf("\033[2K %15s: %7.2f   %15s: %7.2f  %15s: %7.2f %15s: %1.1d\n",  "Alpha", 	 TM_cTx->Alpha, 	"Beta", 	 TM_cTx->Beta, 	"test", 	0.0, 	"STICK_ENABLE",  TM_cTx->Stick_Enable 	);	        
        printf("\033[2K %20s: %03d \n",   "Frame Count",  TM_cTx->frame_cnt );   


        printf("\033[5A");

        fflush(stdout);
//     }

}




	{
 		Weight = MASS*G;

 		printf( "\033[7F" );    //  controls cursor on terminal for non scrolling and back up 7 lines.
 		
 		printf( "10]-------------------------------------------------------------------------------------------------------------------------------------\n" );
 		printf( "| Time s | Vinf Ang | Vin | Alt | Elv Ang | Cm | q | Pitch-Pitch | | | \n" );
 		printf( "| %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %9.3f | | |\n", 
 		time, flight_path_angle, Vinf, altitude, delta_e * RADtoDEG , CM_DELTA_E * delta_e * RADtoDEG, q * RADtoDEG, Pitch * RADtoDEG ); 
 		printf( "-------------------------------------------------------------------------------------------------------------------------------------\n" );
		printf( "| Time s | Vu | Vw | Pitch | Vif | Alpha | Weight | Lift | Drag | Thrust | Thrtl_Set | \n" );
 		printf( "| %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %9.2f | %9.2f | %9.2f | %3.2f |\n", time, Vu, Vw, pitch_d, Vinf, Alpha * RADtoDEG, Weight, lift, drag, thrust, Thrtl_Set );
 		printf( "11]-------------------------------------------------------------------------------------------------------------------------------------\n" );

	} 
	
#endif	









