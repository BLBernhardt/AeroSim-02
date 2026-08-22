

//=================================================================================================================
// Intf_BBC_GSP.cpp
//=================================================================================================================
#include <math.h>
 
#include "../7-MATH/Math.h" 
#include "../structures.h"
#include "../CONFIG.h"
#include "../GD-A.h"
 
void ms_to_hmsms(unsigned long total_ms,unsigned int *hours,unsigned int *minutes,unsigned int *seconds,unsigned int *milliseconds );
void latlon_to_km(double dlat, double dlon, double lat, double *dx_km, double *dy_km);



void Intf_ASD_GD( GD_t* G_Displ,  Aerodyn_t* AERO_D, Cntrls_t* CTRL  )
{

   	static unsigned int h, m, s, ms, frame_cnt;
    float Vel_NS,  Vel_EW;
 	static float time_last;
 	static double  last_lat,  last_lon, d_lat, d_lon, dx_km, dy_km;

 		G_Displ->Alpha 		=  2.3;
 		G_Displ->Beta 		=  0.5;
 		G_Displ->Pitch 		=  35.0;
 		G_Displ->Yaw 		=  1.0;
 		G_Displ->Roll 		=  2.0;
 		G_Displ->AirSpeed 	=  167.0;
 		G_Displ->Alt 		=  10225;
 		G_Displ->Climb 		=  50.3;
 		G_Displ->dt 			=  0.02;
 //	G_Displ->frame_cnt 	=  123456;
 		G_Displ->hours 		=  12;
 		G_Displ->min 		=  45;
 		G_Displ->sec 		=  23;
 		G_Displ->mSec 		=  056;
 //	G_Displ->Stick_Enable 	=  0; // Consistent 1-byte field
 //		G_Displ->key 		=  'G';
 		G_Displ->spinner[4] 	=  2;


		G_Displ->Pitch   	= AERO_D->Pitch_d;
		G_Displ->Roll     	= -AERO_D->Roll_d;
		G_Displ->Yaw      	= AERO_D->Yaw_d; 	
		G_Displ->AirSpeed 	= AERO_D->AirSpeed_knots;
		G_Displ->Alt      	= AERO_D->Altitude_AGL_feet;
		G_Displ->Alpha    	= AERO_D->Alpha;
		G_Displ->Beta     	= AERO_D->Beta;
		G_Displ->Climb    	= AERO_D->RateOfClimb_ft_sec;
		
		G_Displ->key 			=   CTRL->key;
		
		
//		G_Displ->Position_X 	= AERO_D->Position_inertial_X_ft;
//		G_Displ->Position_Y 	= AERO_D->Position_inertial_Y_ft;
//		G_Displ->G_meter		= AERO_D->G_meter;

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











//===============================================================================
// EOF











