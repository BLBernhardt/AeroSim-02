//===============================================================================
// Common_H
//===============================================================================

#ifndef IPC_UDP_COMMON_H
#define IPC_UDP_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUFFER_SIZE 1024

#define SERVER_TX_PORT 5001
#define CLIENT_TX_PORT 5002

#define DEFAULT_SERVER_IP "127.0.0.1"

#define MAX_RETRIES     30
#define RETRY_DELAY_US  200000

#endif


//===============================================================================
//EOF
