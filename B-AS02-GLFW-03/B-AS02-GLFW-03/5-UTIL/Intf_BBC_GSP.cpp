

//=================================================================================================================
// Intf_BBC_GSP.cpp
//=================================================================================================================
#include <math.h>
 
#include "../structures.h"
#include "../CONFIG.h"
#include "../7-MATH/Math.h"
 
void ms_to_hmsms(unsigned long total_ms,unsigned int *hours,unsigned int *minutes,unsigned int *seconds,unsigned int *milliseconds );
void latlon_to_km(double dlat, double dlon, double lat, double *dx_km, double *dy_km);



void Intf_BBC_GSP( TM_Stx_t* TM_in->, TM_Ctx_t* TM_out, GD_IN_t* GD_in, Cntrls_t* CTRL )
{

 	static unsigned int h, m, s, ms, frame_cnt;
	float Vel_NS, Vel_EW;
 	static float time_last;
 	static double last_lat, last_lon, d_lat, d_lon, dx_km, dy_km;

//-----------------------------------------------------------------------------------
// TM Input to Graphics Display GD 

		d_lat = last_lat - TM_in->latY_m;
		d_lon = last_lon - TM_in->lonX_m;
				
		latlon_to_km( d_lat, d_lon, TM_in->latY_m, &dx_km, &dy_km );

		 GD_in->Position_X 		+= dy_km;				
		 GD_in->Position_Y 		+= dx_km; 		

		 			Vel_NS		= TM_in->velNorth_mps; 		 
					Vel_EW		= TM_in->velEast_mps;	 	
		 GD_in->AirSpeed 		= sqrt( Vel_NS*Vel_NS + Vel_EW*Vel_EW );

		 GD_in->Roll 			= TM_in->roll_r * RADtoDEG;			 
		 GD_in->Pitch			= TM_in->pitch_r * RADtoDEG;		 
		 GD_in->Yaw 			= TM_in->heading_r * RADtoDEG;		 

		 GD_in->Alt 			= TM_in->hae_m; 					
		 GD_in->Climb			= -TM_in->velDown_mps;		
 	
 		 GD_in->dt				= TM_in->timeTag_ms - time_last;
 			time_last			= TM_in->timeTag_ms;		
 			
 		 GD_in->frame_cnt		= frame_cnt++;	
		
 		ms_to_hmsms( TM_in->timeTag_ms, &h, &m, &s, &ms );
 		 GD_in->hours			= h;
 		 GD_in->min				= m;
 		 GD_in->sec				= s;
 		 GD_in->mSec			= ms;
 			time_last			= TM_in->timeTag_ms;
 	
 		 GD_in->Alpha 			= TM_in->aoa_d;
		 GD_in->Beta			= TM_in->beta_d;
 		
		 GD_in->key				= CTRL->key;
		
		 GD_in->G				= TM_in->accDown_mps2/9.80;	



//-----------------------------------------------------------------------------------
// Controls to  TM Output 

 		TM_out->frame_cnt 		= CTRL->frame_cnt;
		TM_out->Elv_Cmd_u 		= CTRL->Elv_Cmd_u;
		TM_out->Ail_Cmd_u 		= CTRL->Ail_Cmd_u;
		TM_out->Rud_Cmd_u 		= CTRL->Rud_Cmd_u;
		TM_out->Thr_Cmd_u 		= CTRL->Thr_Cmd_u;
		TM_out->Elv_Trim_Cmd_u 	= CTRL->Elv_Trim_Cmd_u;
		TM_out->Ail_Trim_Cmd_u 	= CTRL->Ail_Trim_Cmd_u;
		TM_out->Rud_Trim_Cmd_u 	= CTRL->rud_Trim_Cmd_u;
		TM_out->Flaps_Cmd_i	 	= CTRL->Flaps_Cmd_i;
		TM_out->Spdbrk_Cmd_u	= CTRL->Spdbrk_Cmd_u;
		TM_out->Gear_Cmd_b		= CTRL->Gear_Cmd_b;










#if 0
	uint32_t timeTag_ms;		 	/* '<I' */

	double 	latY_m;			 	/* '<d' */
	double 	lonX_m;			 	/* '<d' */
	float	hae_m;			 		/* '<f' */

	float	velNorth_mps;			/* '<f' */
	float	velEast_mps;		 	/* '<f' */
	float	velDown_mps;		 	/* '<f' */

	float	accForward_mps2;	 	/* '<f' */
	float	accRight_mps2;	 		/* '<f' */
	float	accDown_mps2;			/* '<f' */

	float	roll_r;			 		/* '<f' */
	float	pitch_r;			 	/* '<f' */
	float	heading_r;		 		/* '<f' */

	float	roll_rps;				/* '<f' */
	float	pitch_rps;		 		/* '<f' */
	float	yaw_rps;			 	/* '<f' */

	float	fcsAileron_d;	 		/* '<f' */
	float	fcsElevator_d;	 		/* '<f' */
	float	fcsRudder_d;	 		/* '<f' */
	float	fcsFlaps_d;				/* '<f' */
	float	fcsSpeedbrakes_d; 		/* '<f' */
	float	throttleFbk;		 	/* '<f' */
	float	thrust_N;				/* '<f' */
	float	fuelLevel;		 		/* '<f' */
	float	aoa_d;			 		/* '<f' */
	float	beta_d;					/* '<f' */

	uint8_t gearsDownFbk;			/* '<B' */
	uint8_t wowNose;			 	/* '<B' */
	uint8_t wowLeft;			 	/* '<B' */
	uint8_t wowRight;				/* '<B' */
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

//===================================================================================
/* Convert latitude/longitude differences (in degrees) to kilometres.
 *
 * dlat Difference in latitude (deg)
 * dlon Difference in longitude (deg)
 * lat	Reference latitude	 (deg)
 * dx_km Output: east-west distance (km)
 * dy_km Output: north-south distance (km)
 *
 * Uses the common spherical approximation:
 * 1° latitude ≈ 111.32 km
 * 1° longitude ≈ 111.32 * cos(lat) km
 */
void latlon_to_km(double dlat, double dlon, double lat, double *dx_km, double *dy_km)
{
	const double KM_PER_DEG = 111.32;
	const double DEGtoRAD	= M_PI / 180.0;

	*dy_km = dlat * KM_PER_DEG;
	*dx_km = dlon * KM_PER_DEG * cos(lat * DEG2RAD);
}
//===================================================================================

#if 0
#include <stdio.h>
int main(void)
{
	double dx, dy;

	latlon_to_km( 0.05, 0.05, 34.0, &dx, &dy );
	printf("North-South : %.3f km\n", dy);
	printf("East-West : %.3f km\n", dx);

	return 0;
}
#endif

//===============================================================================
// EOF











