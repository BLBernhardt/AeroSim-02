//
//=============================================================================================
// IPC_UDP_Common_GSP.h
//=============================================================================================

#ifndef IPC_UDP_COMMON_H
#define IPC_UDP_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdint.h>


#define BUFFER_SIZE 1024

// Server ports
#define SERVER_TX_PORT 5001
#define CLIENT_TX_PORT 5002

#define DEFAULT_SERVER_IP "127.0.0.1"
#define DEFAULT_CLIENT_IP "127.0.0.1"

#define MAXETRIES     30
#define RETRY_DELAY_US  200000
#define BUFFER_SIZE        1024

#define MAX_TEXT_LEN 128



//===============================================================================
// Server Tx Struct 
//===============================================================================

#pragma pack(push, 1)
typedef struct Server_Tx_t{
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
} Server_Tx_t;
#pragma pack(pop)


//===============================================================================
// Client Tx Struct 
//===============================================================================

#pragma pack(push, 1) // Ensure no padding for network/message use
typedef struct Client_Tx_t
{
 	uint32_t frame_cnt; 
 	float pitch_cmd; 
 	float roll_cmd; 
 	float yaw_cmd; 
 	float thrtl_cmd; 
 	float spare1; 
 	float spare2; 
 	float spare3; 
} Client_Tx_t;
#pragma pack(pop)


#endif


//===============================================================================
// EOF

