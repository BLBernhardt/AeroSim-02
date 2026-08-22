
//==============================================================================================
// Server_TxRx.h
#ifndef SERVER_TXRX_H
#define SERVER_TXRX_H

#include "../IPC_UDP_Common.h"

//===============================================================================
// Server Tx
//===============================================================================

void IPC_UDP_S_Tx_init(int *sockfd, struct sockaddr_in *client_addr, int port, const char *ip);
size_t Prepare_UDP_Msg(const void* src, size_t struct_size, uint8_t* buffer, size_t bufferSize);

int IPC_UDP_S_Tx(int sockfd, const struct sockaddr_in *client_addr, const uint8_t *pkt, size_t pkt_len);

//===============================================================================
// Server Rx
//===============================================================================
void IPC_UDP_S_Rx_init(int *sockfd, struct sockaddr_in *addr, int port, const char *ip);
void Parse_UDP_Msg(const uint8_t* buffer, size_t len, TM_Param_t* dst);
int  IPC_UDP_S_Rx(int sockfd, Client_Tx_t* received);


#endif

//==============================================================================================

