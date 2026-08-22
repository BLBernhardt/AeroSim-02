

//struct _Cntrls
//{
//    int MouseX;
//    int MouseStartX;
//    int MouseY;
//    int MouseStartY;
//};

struct _Frame
{
   float  delta_time_ms     ;
   float  delta_time_s      ;
   float  Frames_per_second ; 
   float  Last_Frame_Time   ;
   float  Frame_Time        ;
   float  Frame_Rate        ;
   float  time              ;
   float  start_time        ;
   float  flight_dynamics_cnt;
   int    Start_Frame_Flag  ;
   int    frame_flag        ;

};

struct _Control
{
    int Stick_X             ;
    int Stick_Y             ;
    int Cursor_has_Stick    ;
    int MouseX              ;
    int MouseStartX         ;
    int MouseY              ;
    int MouseStartY         ;

    int key_F1_dbnc         ;
    int key_F1              ;

   int key_F2_dbnc          ;
   int key_F2               ;

    int key_F3_dbnc         ;
    int key_F3              ;

    int key_F4_dbnc         ;
    int key_F4              ;

    int key_F5_dbnc         ;
    int key_F5              ;

    int Button_L_Dn         ;
    int Button_C_Dn         ;
    int Button_R_Dn         ;

    int Button_Tggl         ;

};

struct _Display
{
    int   Ortho_Cursor_X    ;
    int   Ortho_Cursor_Y    ;

    int   Ortho_Gridsize_X  ;
    int   Ortho_Gridsize_Y  ;

    int   Frame_Flag        ;
    int   Win_Width         ;
    int   Win_Height        ;

    int   flasher_count     ;
    int   flasher           ;
   
    int   Grid_X            ;
    int   Grid_Y            ;
};

struct _Electrical
{
   int panel_dim            ;
   int lamp_test            ;
   int pwr_on               ;
};

struct _Vehicle
{
   float vcl_act_mass_f     ;
   float vcl_max_mass_f     ;
   float flight_time        ;
   int   disc_wow           ;
   
   float accl_f             ;
   float velc_f             ;   
   
   float mass_max_f         ;
   float mass_min_f         ;
   float mass_f             ; //.act_mass_f

   float Pos_Z_m            ;
   float Pos_X_m            ;
   float Pos_Y_m            ;
   float disc_auto_enbl     ;
   int   alt_i              ;
   int   WOW_Alt            ;
   int   disc_crash         ;
   float time_crash         ;
   int   disc_pwr_on        ;
   int   disc_HL_mode       ;
   int   disc_strobe        ;
};

struct _Engine
{
   float throttle_cmd_f     ;
   float throttle_rsp_f     ;   
   int   disc_ind           ;
   int   disc_on            ;
   float thrust_f           ;
   float thrust_max_f       ;
   float thrust_min_f       ;
   float isp_f              ;
   int   thrust_flame       ;
   float length_flame_ver   ;
   float length_flame_hor   ;
};

#if 0


typedef struct                   
{
  int  texture_id;
  int  source_sector; 
  char source_image[30]; 
  int Position_X;
  int Position_Z;
  int  source_subsector_x;       
  int  source_subsector_y;
  float destination_location_x;
  float destination_location_z;
}TERRAIN_CONFIG;

TERRAIN_CONFIG  TerrainInfo[100];


struct 
{
  int TRN_Aircraft_Position_Correction_Valid;
  int PGCAS_Selected;
  int PGCAS_Warning;
  int PGCAS_Valid;
  int ATAC_Selected;
  int ATAC_Valid;
  int Track_Mode;
  float ATAC_Time_L;
  float ATAC_Time_C;
  float ATAC_Time_R;
}Terprom;

struct
{
  int socket_5023;
  int socket_7778;
  int chan_5023_active;
  int chan_7778_active;
  int chan_7778_enable;
  int chan_5023_enable;
}Comm;

struct AIRC               
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

  float VertSpeed_feet_ps;
  float Flight_Deck_Hieght_feet;

  unsigned char WOW;

};



struct
{
 int IPC_Server_Port;
}Interface;

struct
{
  float Field_Elevation_feet;
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
  int WOW;
}Discrete;
#endif


