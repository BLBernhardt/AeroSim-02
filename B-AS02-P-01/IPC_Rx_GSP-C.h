//
//=============================================================================================
// IPC_Rx_GSP-C.h
//=============================================================================================

#ifndef IPC_UDP_COMMON_H
#define IPC_UDP_COMMON_H
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h> 
#include <errno.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <stdint.h>

//===============================================================================
// Python Server Tx Struct
//===============================================================================

#pragma pack(push, 1)
typedef struct GSP_UDP_t{
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
} GSP_UDP_t;
#pragma pack(pop)

#endif


//===============================================================================
// EOF

