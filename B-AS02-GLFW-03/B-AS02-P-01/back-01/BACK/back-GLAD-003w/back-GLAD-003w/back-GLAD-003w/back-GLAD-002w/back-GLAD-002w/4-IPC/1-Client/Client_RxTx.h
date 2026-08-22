																				
//===============================================================================
// IPC_UDP_C_Rx.h

#ifndef IPC_UDP_C_RX_H
#define IPC_UDP_C_RX_H

void IPC_UDP_C_Rx_init
(
    int *sockfd,
    int port
);

int IPC_UDP_C_Rx
(
    int sockfd,
    char *buffer,
    size_t buffer_size
);

#endif



//===============================================================================
// IPC_UDP_C_Tx.h

#ifndef IPC_UDP_C_TX_H
#define IPC_UDP_C_TX_H

void IPC_UDP_C_Tx_init
(
    int *sockfd,
    struct sockaddr_in *server_addr,
    int port,
    const char *server_ip
);

void IPC_UDP_C_Tx
(
    int sockfd,
    const struct sockaddr_in *server_addr,
    int var1,
    int var2
);

#endif

//===============================================================================
//EOF
