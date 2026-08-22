//
//=============================================================================================
// IPC_Rx_GSP-C.h
//=============================================================================================

#ifndef IPC_RX_GPS_H
#define IPC_RX_GPS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include "math.h"

#define BUFFER_SIZE 1024

// Server ports
#define SERVER_TX_PORT 5001
#define CLIENT_TX_PORT 5002

#define DEFAULT_SERVER_IP "127.0.0.1"
#define DEFAULT_CLIENT_IP "127.0.0.1"
#define DEFAULT_IP "127.0.0.1"

#define MAXETRIES 30
#define RETRY_DELAY_US 200000
#define BUFFER_SIZE 1024

#define MAX_TEXT_LEN 128

//  Server is the simulation process              TM_Stx_t
//  Client is the Display and Controls process    TM_Ctx_t


//===============================================================================
// Python Server Tx Struct
//===============================================================================

#pragma pack(push, 1)
typedef struct TM_Stx_t{
    uint32_t timeTag_ms;          

    double   latY_m;              
    double   lonX_m;              
    float    hae_m;               

    float    velNorth_mps;        
    float    velEast_mps;         
    float    velDown_mps;         

    float    accForward_mps2;     
    float    accRight_mps2;       
    float    accDown_mps2;        

    float    roll_r;              
    float    pitch_r;             
    float    heading_r;           

    float    roll_rps;            
    float    pitch_rps;           
    float    yaw_rps;             

    float    fcsAileron_d;      
    float    fcsElevator_d;     
    float    fcsRudder_d;       
    float    fcsFlaps_d;        
    float    fcsSpeedbrakes_d;  
    float    throttleFbk;         
    float    thrust_N;            
    float    fuelLevel;           
    float    aoa_d;             
    float    beta_d;            

    uint8_t  gearsDownFbk;        
    uint8_t  wowNose;             
    uint8_t  wowLeft;             
    uint8_t  wowRight;            
} TM_Stx_t;
#pragma pack(pop)



#pragma pack(push, 1) // Ensure no padding for network/message use
typedef struct TM_Ctx_t
{
 	uint32_t frame_cnt; 
 	float Elv_Cmd_u; 		// +1.0 to -1.0
 	float Ail_Cmd_u; 		// +1.0 to -1.0
 	float Rud_Cmd_u; 		// +1.0 to -1.0
 	float Thr_Cmd_u; 		// +1.0 to    0
 	float Elv_Trim_Cmd_u; 	// +1.0 to -1.0
 	float Ail_Trim_Cmd_u; 	// +1.0 to -1.0
 	float Rud_Trim_Cmd_u; 	// +1.0 to -1.0
 	float Flaps_Cmd_i;      // 1,2,3,4,5  
    float Spdbrk_Cmd_u; // +1.0 to    0
 	int	  Gear_Cmd_b;		// 1 Down, 0 Up
} TM_Ctx_t;
#pragma pack(pop)





#endif


//===============================================================================
// EOF

