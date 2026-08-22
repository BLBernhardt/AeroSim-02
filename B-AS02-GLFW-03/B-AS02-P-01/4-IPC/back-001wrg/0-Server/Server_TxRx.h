																			
//===============================================================================
// IPC_UDP_S_TX_H
//===============================================================================

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




//===============================================================================
// IPC_UDP_S_RX_H
//===============================================================================

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

//===============================================================================
//EOF

