//=============================================================================================
// Common_H
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

#define MAX_RETRIES     30
#define RETRY_DELAY_US  200000

#endif
//=====================================================================
// server.h

//========================================================================
// IPC_UDP_S_TX_H

#ifndef IPC_UDP_S_TX_H
#define IPC_UDP_S_TX_H

void IPC_UDP_S_Tx_init
(
    int *sockfd,
    struct sockaddr_in *client_addr,
    int port,
    const char *ip
);

void IPC_UDP_S_Tx
(
    int sockfd,
    const struct sockaddr_in *client_addr,
    int var1,
    int var2
);

#endif




//=======================================================================
// IPC_UDP_S_RX_H


#ifndef IPC_UDP_S_RX_H
#define IPC_UDP_S_RX_H

void IPC_UDP_S_Rx_init
(
    int *sockfd,
    struct sockaddr_in *addr,
    int port
);

int IPC_UDP_S_Rx
(
    int sockfd,
    struct sockaddr_in *client_addr,
    char *buffer,
    size_t buffer_size
);

#endif



#if 0
//====================================================================
// IPC_UDP_S_Tx.h
//====================================================================
#ifndef IPC_UDP_S_TX_H
#define IPC_UDP_S_TX_H

void IPC_UDP_S_Tx_init(int *sockfd);
void IPC_UDP_S_Tx(int sockfd, const struct sockaddr_in *client_addr, int var1, int var2);

#endif


//====================================================================
// IPC_UDP_S_Rx.h
//====================================================================

#ifndef IPC_UDP_S_RX_H
#define IPC_UDP_S_RX_H

void IPC_UDP_S_Rx_init(int *sockfd, struct sockaddr_in *addr, int port);
int IPC_UDP_S_Rx(int sockfd, struct sockaddr_in *client_addr, char *buffer, size_t buffer_size);

#endif
#endif

