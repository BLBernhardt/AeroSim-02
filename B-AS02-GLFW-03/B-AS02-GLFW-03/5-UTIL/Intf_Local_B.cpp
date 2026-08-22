

//=================================================================================================================
// Intf_BBC_GSP.cpp
//=================================================================================================================
#include <math.h>
 
#include "../7-MATH/Math.h" 
#include "../IPC_CONFIG.h"

 
void ms_to_hmsms(unsigned long total_ms,unsigned int *hours,unsigned int *minutes,unsigned int *seconds,unsigned int *milliseconds );
void latlon_to_km(double dlat, double dlon, double lat, double *dx_km, double *dy_km);



#if 0
typedef struct GD_IN_t
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
 	char 		key;
 	double 		Position_X;
 	double 		Position_Y;
 	float  		G_meter;			
} GD_IN_t;
#endif



void Interface_TM( TM_Stx_t* TM_Stx, TM_Ctx_t* TM_Ctx, GD_IN_t* GD_in, Aero_Cntl_t* AERO_CTRL, Aerodyn_t* AERO_D,  Cntrls_t* CTRL  )
{

//  static unsigned int h, m, s, ms, frame_cnt;
//  float Vel_NS,  Vel_EW;
// 	static float time_last;
// 	static double  last_lat,  last_lon, d_lat, d_lon, dx_km, dy_km;

		AERO_CTRL->Elv_Cmd_u 		= CTRL->Elv_Cmd_u; 		// +1.0 to -1.0
		AERO_CTRL->Ail_Cmd_u 		= CTRL->Ail_Cmd_u; 		// +1.0 to -1.0
		AERO_CTRL->Rud_Cmd_u 		= CTRL->Rud_Cmd_u; 		// +1.0 to -1.0
		AERO_CTRL->Thr_Cmd_u 		= CTRL->Thr_Cmd_u; 		// +1.0 to    0
 
		AERO_CTRL->Elv_Trim_Cmd_u 	= CTRL->Elv_Trim_Cmd_u; // +1.0 to -1.0
		AERO_CTRL->Ail_Trim_Cmd_u 	= CTRL->Ail_Trim_Cmd_u; // +1.0 to -1.0
		AERO_CTRL->Rud_Trim_Cmd_u 	= CTRL->Rud_Trim_Cmd_u; // +1.0 to -1.0
 
 
//		Spdbrk_Cmd_u 	= ; 	// +1.0 to    0
//		Gear_Cmd_b		= ;		// Gear Cmd 0:UP 1:DOWN

#if 1
		GD_in->Pitch   		= AERO_D->Pitch_d;
		GD_in->Roll     	= -AERO_D->Roll_d;
		GD_in->Yaw      	= AERO_D->Yaw_d; 	
		GD_in->AirSpeed 	= AERO_D->AirSpeed_knots;
		GD_in->Alt      	= AERO_D->Altitude_AGL_feet;
		GD_in->Alpha    	= AERO_D->Alpha;
		GD_in->Beta     	= AERO_D->Beta;
		GD_in->Climb    	= AERO_D->RateOfClimb_ft_sec;
		GD_in->key 			= CTRL->key;
#endif
//		GD_in->Position_X 	= AERO_D->Position_inertial_X_ft;
//		GD_in->Position_Y 	= AERO_D->Position_inertial_Y_ft;
//		GD_in->G_meter		= AERO_D->G_meter;

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

/**
 * Convert latitude/longitude differences (in degrees) to kilometres.
 *
 * dlat   Difference in latitude  (deg)
 * dlon   Difference in longitude (deg)
 * lat    Reference latitude      (deg)
 * dx_km  Output: east-west distance   (km)
 * dy_km  Output: north-south distance (km)
 *
 * Uses the common spherical approximation:
 *   1° latitude  ≈ 111.32 km
 *   1° longitude ≈ 111.32 * cos(lat) km
 */
void latlon_to_km(double dlat, double dlon, double lat, double *dx_km, double *dy_km)
{
    const double KM_PER_DEG = 111.32;
    const double DEG2RAD    = M_PI / 180.0;

    *dy_km = dlat * KM_PER_DEG;
    *dx_km = dlon * KM_PER_DEG * cos(lat * DEG2RAD);
}
//=======================================================================================

#if 0
#include <stdio.h>
int main(void)
{
    double dx, dy;

    /* Difference of 0.05° at 34.0° latitude */
    latlon_to_km(0.05, 0.05, 34.0, &dx, &dy);

    printf("North-South : %.3f km\n", dy);
    printf("East-West   : %.3f km\n", dx);

    return 0;
}

#endif


#if 0
 		GD_in->Alpha 		=  2.3;
 		GD_in->Beta 		=  0.5;
 		GD_in->Pitch 		=  35.0;
 		GD_in->Yaw 		=  1.0;
 		GD_in->Roll 		=  2.0;
 		GD_in->AirSpeed 	=  167.0;
 		GD_in->Alt 		=  10225;
 		GD_in->Climb 		=  50.3;
 		GD_in->dt 			=  0.02;
 //	GD_in->frame_cnt 	=  123456;
 		GD_in->hours 		=  12;
 		GD_in->min 		=  45;
 		GD_in->sec 		=  23;
 		GD_in->mSec 		=  056;
 //	GD_in->Stick_Enable 	=  0; // Consistent 1-byte field
 //		GD_in->key 		=  'G';
 		GD_in->spinner[4] 	=  2;

#endif


#if 0
 		TM_Ctx->frame_cnt 	=  AERO_D->Frame_cnt; 
 		TM_Ctx->pitch_cmd 	=  CTRL->Elevator_Cmd; 
 		TM_Ctx->roll_cmd 	=  CTRL->Aileron_Cmd; 
 		TM_Ctx->yaw_cmd 	=  CTRL->Rudder_Cmd; 
 		TM_Ctx->thrtl_cmd 	=  CTRL->Throttle_Cmd; 
 		TM_Ctx->spare1 		=  101; 
 		TM_Ctx->spare2 		=  102; 
 		TM_Ctx->spare3 		=  103; 
#endif





//===============================================================================
// EOF











