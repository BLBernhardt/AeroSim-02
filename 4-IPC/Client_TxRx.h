//=============================================================================
// Client_TxRx.h
#ifndef CLIENT_TXRX_H
#define CLIENT_TXRX_H


#include "../IPC_CONFIG.h"
//#include "../TM_G_t-A.h"


//===============================================================================
// Client Tx
//===============================================================================


void IPC_UDP_C_Tx_init( int *sockfd, struct sockaddr_in *addr, int port, const char *ip );
void IPC_UDP_C_Tx(int sockfd, const struct sockaddr_in* dest_addr, const TM_Ctx_t* packet );

//===============================================================================
// Client Rx
//===============================================================================
void IPC_UDP_C_Rx_init(int *sockfd, struct sockaddr_in *addr, int port, const char *ip);
 int  IPC_UDP_C_Rx(int sockfd, TM_Stx_t* TM_in  );   // 1=got packet, 0=no data, -1=error
void Convert_UDP_Msg(const uint8_t* buffer, size_t len, TM_Stx_t* dst);

#endif

//=============================================================================


