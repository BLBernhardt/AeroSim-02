//=============================================================================
// Client_TxRx.h
#ifndef CLIENT_TXRX_H
#define CLIENT_TXRX_H

#include "../IPC_UDP_Common.h"

//===============================================================================
// Client Tx
//===============================================================================


void IPC_UDP_C_Tx_init( int *sockfd, struct sockaddr_in *addr, int port, const char *ip );
void Pack_UDP_Tx_Msg(const Client_Tx_t* src, uint8_t* buffer, size_t* out_len );
void IPC_UDP_C_Tx(int sockfd, const struct sockaddr_in* dest_addr, const Client_Tx_t* packet );

//===============================================================================
// Client Rx
//===============================================================================


void IPC_UDP_C_Rx_init(int *sockfd, struct sockaddr_in *addr, int port, const char *ip);
 int  IPC_UDP_C_Rx(int sockfd, Server_Tx_t* TMrx);   // 1=got packet, 0=no data, -1=error
 
 
void Convert_UDP_Msg(const uint8_t* buffer, size_t len, Server_Tx_t* dst);

#endif

//=============================================================================


