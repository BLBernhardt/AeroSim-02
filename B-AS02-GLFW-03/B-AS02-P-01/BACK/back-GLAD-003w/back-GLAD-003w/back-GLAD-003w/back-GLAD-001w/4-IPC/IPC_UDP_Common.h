//
//=============================================================================================
// IPC_UDP_Common.h
//=============================================================================================

#ifndef IPC_UDP_COMMON_H
#define IPC_UDP_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

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


#pragma pack(push, 1)
typedef struct TM_Param_t
{
    float Alpha;
    float Beta;
    float Pitch;
    float Yaw;
    float Roll;
    float Mach;
    float CL;
    float CL2;
    float dt;
    uint32_t frame_cnt;
    int32_t hours;
    int32_t min;
    int32_t sec;
    uint32_t mSec;
    uint8_t Stick_Enable;     // Consistent 1-byte field
    char key;
    char spinner[4];
} TM_Param_t;
#pragma pack(pop)




//===============================================================================
// Data Structure Definition
//===============================================================================

#pragma pack(push, 1)   // Ensure no padding for network/message use
typedef struct
{
    uint32_t PID;           // Message type identifier
    uint32_t seq_num;          // Sequence number
    uint32_t timestamp;        // Timestamp / tick count
    int32_t  data1;            // Generic integer data 1
    int32_t  data2;            // Generic integer data 2
    float    data3;      // Floating point data 1
    float    data4;      // Floating point data 2
    char     text[MAX_TEXT_LEN]; // Text / status message
} Client_Tx_t;
#pragma pack(pop)



#endif

//===============================================================================
// EOF
