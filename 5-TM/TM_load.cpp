
//=================================================================================================================
// TM_load.cpp
//=================================================================================================================

 
 //#include "0-AeroSim/flightmodel.h"
 #include "../structures.h"
#include "../CONFIG.h"
 

void TM_load( TM_Param_t* TM_Stx, Client_Tx_t* TM_Ctx, Aerodyn_t* AERO_D, Cntrls_t* CTRL  )
{


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






		TM_Stx->Position_X 	= GSP->latY_m;				/* '<d' */
		TM_Stx->Position_Y 	= GSP->lonX_m; 				/* '<d' */
		TM_Stx->Alt 		= GSP->hae_m; 				/* '<f' */

		 					= GSP->velNorth_mps;   		/* '<f' */   
							= GSP->velEast_mps;      	/* '<f' */
							= GSP->velDown_mps;         /* '<f' */

		TM_Stx->Roll 		= GSP->roll_r;              /* '<f' */
		TM_Stx->Pitch		= GSP->pitch_r;             /* '<f' */
		TM_Stx->Yaw 		= GSP->heading_r;           /* '<f' */

		TM_Stx->AirSpeed	= GSP->						/* '<f' */
		TM_Stx->Climb		= GSP->						/* '<f' */
 	
 		TM_Stx->dt			= GSP->
 		
 		TM_Stx->frame_cnt	= GSP->
 		TM_Stx->hours		= GSP->
 		TM_Stx->min			= GSP->
 		TM_Stx->sec			= GSP->
 		TM_Stx->mSec		= GSP->
 	
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
 	uint8_t Stick_Enable; // Consistent 1-byte field
 	char key;
 	char spinner[4];


	//	if( CTRL->IPC_conf == BBC && ( CTRL->Mode == S_SIM || CTRL->Mode == FULL )
	//	{
		TM_Stx->Pitch   	= AERO_D->Pitch_d;
		TM_Stx->Roll     	= -AERO_D->Roll_d;
		TM_Stx->Yaw      	= AERO_D->Yaw_d; 	
		TM_Stx->AirSpeed 	= AERO_D->AirSpeed_knots;
		TM_Stx->Alt      	= AERO_D->Altitude_AGL_feet;
		TM_Stx->Alpha    	= AERO_D->Alpha;
		TM_Stx->Beta     	= AERO_D->Beta;
		TM_Stx->Climb    	= AERO_D->RateOfClimb_ft_sec;
//		TM_Stx->Position_X 	= AERO_D->Position_inertial_X_ft;
//		TM_Stx->Position_Y 	= AERO_D->Position_inertial_Y_ft;
//		TM_Stx->G_meter		= AERO_D->G_meter;
	//	}

 		TM_Ctx->frame_cnt 	=  AERO_D->Frame_cnt; 
 		TM_Ctx->pitch_cmd 	=  CTRL->Elevator_Cmd; 
 		TM_Ctx->roll_cmd 	=  CTRL->Aileron_Cmd; 
 		TM_Ctx->yaw_cmd 	=  CTRL->Rudder_Cmd; 
 		TM_Ctx->thrtl_cmd 	=  CTRL->Throttle_Cmd; 
 		TM_Ctx->spare1 		=  101; 
 		TM_Ctx->spare2 		=  102; 
 		TM_Ctx->spare3 		=  103; 
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

//===============================================================================
// EOF











