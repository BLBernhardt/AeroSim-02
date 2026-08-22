#ifndef TM_KB
#define TM_KB


typedef unsigned int uint;
typedef bool 		 bit;

#if 0
struct _TM_Param_off
{
	float Alpha;
	float Beta;
	float Mach;
	float CL;
	float CL2;
	float Time;
	uint  frame_cnt;
	bit   Stick_Enable;

};


//=====================================================
// Type definitions (from Telemetry.h)
//=====================================================
typedef unsigned int uint;
typedef bool bit;

struct _TM_Param
{
    float Alpha;
    float Beta;
    float Pitch;
    float Yaw;
    float Roll;
    float Mach;
    float CL;
    float CL2;
    float Time;
    uint 	frame_cnt;
    bit 	Stick_Enable;
    char	key;
}_TM_Param;
#endif
typedef struct KB_State
{
    struct 	termios old_tio;
    char 	last_key;
    int 	init;
} KB_State;

#if 0
// Global telemetry instance
struct _TM_Param TMParam = {
    .Alpha = 2.5f,
    .Beta  = 0.3f,
    .Pitch = 5.0f,
    .Yaw   = 1.2f,
    .Roll  = -0.8f,
    .Mach  = 0.75f,
    .CL    = 0.45f,
    .CL2   = 0.12f,
    .Time  = 0.0f,
    .frame_cnt = 0,
    .Stick_Enable = true
};
#endif
#endif

