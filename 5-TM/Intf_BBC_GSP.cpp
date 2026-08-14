
//=================================================================================================================
// Intf_BBC_GSP.cpp
//=================================================================================================================

 
#include "../structures.h"
#include "../CONFIG.h"
 
void ms_to_hmsms(unsigned long total_ms,unsigned int *hours,unsigned int *minutes,unsigned int *seconds,unsigned int *milliseconds );

void Intf_BBC_GSP( TM_Param_t* TM_Disp, GSP_UDP_t*  GSP, Cntrls_t* CTRL  )
{

    unsigned int h, m, s, ms;
    float Vel_NS,  Vel_EW;
#if 0
 		TM_Disp->Alpha 		=  2.3;
 		TM_Disp->Beta 		=  0.5;
 		TM_Disp->Pitch 		=  35.0;
 		TM_Disp->Yaw 		=  1.0;
 		TM_Disp->Roll 		=  2.0;
 		TM_Disp->AirSpeed 	=  167.0;
 		TM_Disp->Alt 		=  10225;
 		TM_Disp->Climb 		=  50.3;
 		TM_Disp->dt 			=  0.02;
 //	TM_Disp->frame_cnt 	=  123456;
 		TM_Disp->hours 		=  12;
 		TM_Disp->min 		=  45;
 		TM_Disp->sec 		=  23;
 		TM_Disp->mSec 		=  056;
 //	TM_Disp->Stick_Enable 	=  0; // Consistent 1-byte field
 //		TM_Disp->key 		=  'G';
 		TM_Disp->spinner[4] 	=  2;

#endif

		TM_Disp->Position_X = GSP->latY_m;				
		TM_Disp->Position_Y = GSP->lonX_m; 				

		 			Vel_NS	= GSP->velNorth_mps;   		 
					Vel_EW	= GSP->velEast_mps;      	
		TM_Disp->AirSpeed 	= sqrt( Vel_NS*Vel_NS + Vel_EW*Vel_EW );

		TM_Disp->Roll 		= GSP->roll_r;              
		TM_Disp->Pitch		= GSP->pitch_r;             
		TM_Disp->Yaw 		= GSP->heading_r;           

		TM_Disp->Alt 		= GSP->hae_m; 					
		TM_Disp->Climb		= -GSP->velDown_mps;		
 	
 		TM_Disp->dt			= 0.02;
 		TM_Disp->frame_cnt	= 12345;	
 			
 		ms_to_hmsms( GSP->timeTag_ms, &h, &m, &s, &ms );
 		TM_Disp->hours		= h;
 		TM_Disp->min		= m;
 		TM_Disp->sec		= s;
 		TM_Disp->mSec		= ms;
 	
 		TM_Disp->Alpha 		= GSP->aoa_deg;
		TM_Disp->Beta		= GSP->beta_deg;
 	
		TM_Disp->key		= CTRL->key;



#if 0
    uint32_t timeTag_ms;          /* '<I' */

    double   latY_m;              /* '<d' */
    double   lonX_m;              /* '<d' */
    float    hae_m;               /* '<f' */

    float    velNorth_mps;        /* '<f' */
    float    velEast_mps;         /* '<f' */
    float    velDown_mps;         /* '<f' */

    float    accForward_mps2;     /* '<f' */
    float    accRight_mps2;       /* '<f' */
    float    accDown_mps2;        /* '<f' */

    float    roll_r;              /* '<f' */
    float    pitch_r;             /* '<f' */
    float    heading_r;           /* '<f' */

    float    roll_rps;            /* '<f' */
    float    pitch_rps;           /* '<f' */
    float    yaw_rps;             /* '<f' */

    float    fcsAileron_deg;      /* '<f' */
    float    fcsElevator_deg;     /* '<f' */
    float    fcsRudder_deg;       /* '<f' */
    float    fcsFlaps_deg;        /* '<f' */
    float    fcsSpeedbrakes_deg;  /* '<f' */
    float    throttleFbk;         /* '<f' */
    float    thrust_N;            /* '<f' */
    float    fuelLevel;           /* '<f' */
    float    aoa_deg;             /* '<f' */
    float    beta_deg;            /* '<f' */

    uint8_t  gearsDownFbk;        /* '<B' */
    uint8_t  wowNose;             /* '<B' */
    uint8_t  wowLeft;             /* '<B' */
    uint8_t  wowRight;            /* '<B' */
#endif


}


void ms_to_hmsms(unsigned long total_ms,unsigned int *hours,unsigned int *minutes,unsigned int *seconds,unsigned int *milliseconds )
{
    *milliseconds = total_ms % 1000;
    total_ms /= 1000;

    *seconds = total_ms % 60;
    total_ms /= 60;

    *minutes = total_ms % 60;
    total_ms /= 60;

    *hours = (unsigned int)total_ms;
}
//===============================================================================
// EOF











