
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
//				TM_Stx->Position_X 	= AERO_D->Position_inertial_X_ft;
//				TM_Stx->Position_Y 	= AERO_D->Position_inertial_Y_ft;
//				TM_Stx->G_meter		= AERO_D->G_meter;
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

}

//===============================================================================
// EOF
