#ifndef STRUCT_H
#define STRUCT_H

//static struct termios old_tio;

typedef unsigned int uint;
typedef bool bit;


struct UDP_t
{
 int Server_Port;
};


typedef struct KB_State_t
{
 //   struct 	termios old_tio;
    char 	last_key;
    int 	init;
} KB_State_t;



typedef struct Cntrls_t
{
  int   terminate;

  int 	Stick_X;
  int 	Stick_Y;
  int 	Stick_Z;  
  
  float x_cmd;
  float y_cmd;
  float z_cmd;
    
  int 	Cursor_has_Stick;
  int 	Button_Down;
  
  int 	MouseStartY;
  int 	MouseStartX;
  
  int 	MouseX;  
  int 	MouseY;
  int   MouseZ;
  
  int 	Stick_Enable;

  int	Pitch_impulse;
  float Pitch_Cmd;
  float angle_alpha;
  int	abort;
  int   key;
  
  float Throttle_Cmd; // throttle 				[  0, +1 ].
  float Elevator_Cmd; // elevator deflection 	[ -1, +1 ].
  float Aileron_Cmd;  // aileron deflection 	[ -1, +1 ].
  float Rudder_Cmd;   // Grudder deflection 	[ -1, +1 ].
} Cntrls_t;

typedef struct  Aerodyn_t            
{
  int   Type;

  float Latitude;   
  float Longitude; 

  float Position_inertial_X_ft;   
  float Position_inertial_Z_ft; 
  float Position_inertial_Y_ft;

  float Offset_X_meters;   
  float Offset_Z_meters; 
  float Offset_Y_meters;

  float Altitude_MSL_feet;
  float Altitude_AGL_feet;
  float Altitude_AGL_feet_r;

  float Altitude_MSL_meters;
  float Altitude_AGL_meters;
  float Altitude_Radar_feet;

  float Pitch_d;
  float Yaw_d;
  float Roll_d;

  float Pitch_d_r;
  float Yaw_d_r;
  float Roll_d_r;

  float Offset_Pitch_d;
  float Offset_Yaw_d;
  float Offset_Roll_d;

  float Heading_d;
  float Heading_d_r;
  float Offset_Heading_d;

  float AirSpeed_meters_ps;
  float AirSpeed_accl_meters_pss;

  float AirSpeed_knots;
  float AirSpeed_knots_r;
  float AirSpeed_knots_max;
  float AirSpeed_knots_vert_climb_max;

  float VertSpeed_feet_ps;
  
  float RateOfClimb_ft_sec;
  float Alpha;
  float Beta;

  unsigned char WOW;
  
} Aerodyn_t;

typedef struct Display_t
{
  int    PFD_Display_Orientation;

  int	 Window1;
  int    Win1_Width;
  int    Win1_Height;
  
  int 	 Window2;  
  int    Win2_Width;
  int    Win2_Height;  
  
  int 	 Window3;  
  int    Win3_Width;
  int    Win3_Height; 

  int    Grid_X;
  int    Grid_Y;

} Display_t;

typedef struct Frame_t
{

   float  Frame_Time;
   float  delta_time;
   float  Frames_per_second; 
   int    Flag;
} Frame_t;

#endif
/*
struct                
{
  float Latitude;   
  float Longitude; 
  float Position_X_meters;   
  float Position_Z_meters; 
  float Altitude_MSL_feet;
  float Altitude_AGL_feet;
  float Altitude_MSL_meters;
  float Altitude_AGL_meters;
  float Altitude_meters;
  float Pitch_d;
  float Yaw_d;
  float Roll_d;
  float Heading_d;
  float AirSpeed_meters_ps;
  float AirSpeed_feet_ps;
  float AirSpeed;
  float Alt_Radar_feet_1;
  float VertSpeed_feet_ps;
  float Flight_Deck_Hieght_feet;
}Aircraft;
*/

#if 0
struct
{
  int Field_Elevation_feet;
}Airport;


struct
{

  double image_13_LL_lat; 
  double image_13_LL_lon;

  double image_18_LL_lat;
  double image_18_LL_lon;

  double image_13_UR_lat;
  double image_13_UR_lon;

  double image_18_UR_lat;
  double image_18_UR_lon;

  double lat_2km_size;
  double lon_2km_size; 

double ref_lat; 
double ref_lon;

double lat_1m_size;
double lon_1m_size;

double Km_per_d_Lat;
double Km_per_d_Lon;

float Offset_NS;		             
float Offset_WE;		             
float Offset_Alt;		            

int   Number_of_Sectors;

  float Field_Elevation;

}Terrain;

struct
{
  int Hud_Enable;
  int PFD_Enable;
  int WOW;
}Discrete;

typedef struct                   
{
int  texture_id;
int  source_sector; 
char source_image[30]; 
int Position_X;
int Position_Z;
float  destination_location_x;
float  destination_location_z;
int  source_subsector_x;       
int  source_subsector_y;
}TERRAIN_CONFIG;


TERRAIN_CONFIG  TerrainInfo[100];
#endif
