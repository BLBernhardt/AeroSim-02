
//=================================================================================================================
// TM_load.cpp
//=================================================================================================================

 
 //#include "0-AeroSim/flightmodel.h"
 #include "../structures.h"
#include "../CONFIG.h"
 

void TM_load( TM_Param_t* TM_Stx, Client_Tx_t* TM_Ctx, Aerodyn_t* AERO_D, Cntrls_t* CTRL  )
{

    unsigned int h, m, s, ms;
    float Vel_NS,  Vel_EW, Vel_ac;
#if 0
 		TM_Stx->Alpha 		=  2.3;
 		TM_Stx->Beta 		=  0.5;
 		TM_Stx->Pitch 		=  35.0;
 		TM_Stx->Yaw 		=  1.0;
 		TM_Stx->Roll 		=  2.0;
 		TM_Stx->AirSpeed 	=  167.0;
 		TM_Stx->Alt 		=  10225;
 		TM_Stx->Climb 		=  50.3;
 		TM_Stx->dt 			=  0.02;
 //	TM_Stx->frame_cnt 	=  123456;
 		TM_Stx->hours 		=  12;
 		TM_Stx->min 		=  45;
 		TM_Stx->sec 		=  23;
 		TM_Stx->mSec 		=  056;
 //	TM_Stx->Stick_Enable 	=  0; // Consistent 1-byte field
 //		TM_Stx->key 		=  'G';
 		TM_Stx->spinner[4] 	=  2;

#endif

		TM_Stx->Position_X 	= GSP->latY_m;				
		TM_Stx->Position_Y 	= GSP->lonX_m; 				

		 			Vel_NS	= GSP->velNorth_mps;   		 
					Vel_EW	= GSP->velEast_mps;      	
		TM_Stx->AirSpeed 	= sqrt( Vel_NS*Vel_NS + Vel_EW*Vel_EW );

		TM_Stx->Roll 		= GSP->roll_r;              
		TM_Stx->Pitch		= GSP->pitch_r;             
		TM_Stx->Yaw 		= GSP->heading_r;           

		TM_Stx->Alt 		= GSP->hae_m; 					
		TM_Stx->Climb		= -GSP->velDown_mps;		
 	
 		TM_Stx->dt			= 0.02;
 		TM_Stx->frame_cnt	= 12345;	
 			
 		ms_to_hmsms( GSP->timeTag_ms, &h, &m, &s, &ms );
 		TM_Stx->hours		= h;
 		TM_Stx->min			= m;
 		TM_Stx->sec			= s;
 		TM_Stx->mSec		= ms;
 	
 		TM_Stx->Alpha 		= GSP->aoa_deg;
		TM_Stx->Beta		= GSP->beta_deg;
 	
		TM_Stx->key			= 'M';

#endif

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











