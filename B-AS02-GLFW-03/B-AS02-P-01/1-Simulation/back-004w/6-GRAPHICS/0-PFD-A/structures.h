


struct _UDP
{
 int Server_Port;
};

//struct
//{
//  int PFD_Display_Orientation;
//}
//Displays;

struct _Cntrls
{
  int Stick_X;
  int Stick_Y;
  int Cursor_has_Stick;
  int Button_Down;
  int MouseX;
  int MouseStartX;
  int MouseY;
  int MouseStartY;
  int Stick_Enable;
  float aileron_cmd_norm;
  float elevator_cmd_norm;
};

struct _Aerodyn            
{
  int   Type;

  float Latitude;   
  float Longitude; 

  float Position_X_meters;   
  float Position_Z_meters; 
  float Position_Y_meters;

  float Offset_X_meters;   
  float Offset_Z_meters; 
  float Offset_Y_meters;

  float Altitude_MSL_feet;
  float Altitude_AGL_feet;
  float Altitude_AGL_feet_r;

  float Altitude_MSL_meters;
  float Altitude_AGL_meters;
  float Altitude_Radar_feet;

  float Pitch_deg;
  float Yaw_deg;
  float Roll_deg;

  float Pitch_deg_r;
  float Yaw_deg_r;
  float Roll_deg_r;

  float Offset_Pitch_deg;
  float Offset_Yaw_deg;
  float Offset_Roll_deg;

  float Heading_deg;
  float Heading_deg_r;
  float Offset_Heading_deg;

  float AirSpeed_meters_ps;
  float AirSpeed_accl_meters_pss;

  float AirSpeed_knots;
  float AirSpeed_knots_r;
  float AirSpeed_knots_max;
  float AirSpeed_knots_vert_climb_max;




  float VertSpeed_feet_ps;
  float Flight_Deck_Hieght_feet;

  unsigned char WOW;

};

struct _Display
{

  int    PFD_Display_Orientation;

  int    Win_Width;
  int    Win_Height;

  int    Grid_X;
  int    Grid_Y;


};



struct _Discrete
{
  int WOW;
};


struct _Frame
{

   float  Frame_Time;
   float  delta_time;
   float  Frames_per_second; 
   int    Flag;
};

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
  float Pitch_deg;
  float Yaw_deg;
  float Roll_deg;
  float Heading_deg;
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

double Km_per_Deg_Lat;
double Km_per_Deg_Lon;

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
