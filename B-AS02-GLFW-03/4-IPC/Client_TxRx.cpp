

//==============================================================================================================
// Client_TxRx.c
//
// 		TEST:   sudo tshark -i any -f "udp port 5001" -V
//
//



#include <fcntl.h>
#include <errno.h>      // errno, EAGAIN, EWOULDBLOCK

#include "Client_TxRx.h"
#include "../IPC_CONFIG.h"
//#include "../TM_G_t-A.h"


//===============================================================================
// IPC Client Tx Init
//===============================================================================
void IPC_UDP_C_Tx_init(int *sockfd, struct sockaddr_in *addr, int port, const char *ip)
{
    *sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if( *sockfd < 0 ){ perror("Tx socket creation failed"); exit(1); }

    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port   = htons(port);
    addr->sin_addr.s_addr = inet_addr(ip);

    printf("Tx ready to send to %s:%d\n", ip, port);
}

//===============================================================================
// Pack UDP Message( host → network byte order)
//===============================================================================
void Pack_UDP_Tx_Msg(const TM_Ctx_t* src, uint8_t* buffer, size_t* out_len )
{
    if( src == NULL || buffer == NULL || out_len == NULL) 
    {
        fprintf(stderr, "Pack_UDP_Msg: Invalid input\n");
        *out_len = 0;
        return;
    }

    // Copy structure
    memcpy(buffer, src, sizeof( TM_Ctx_t ));

    // Convert host → network byte order( 32-bit words)
    uint32_t* p =( uint32_t*)buffer;
    size_t num_words = sizeof( TM_Ctx_t) / 4;
    for( size_t i = 0; i < num_words; ++i) { p[i] = htonl(p[i]); }

    *out_len = sizeof( TM_Ctx_t);
}

//===============================================================================
// IPC Client Tx
//===============================================================================
void IPC_UDP_C_Tx(int sockfd, const struct sockaddr_in* dest_addr, const TM_Ctx_t* packet )
{
    uint8_t buffer[BUFFER_SIZE] = {0};
    size_t  len = 0;

    Pack_UDP_Tx_Msg( packet, buffer, &len );
    if( len == 0 ){ fprintf(stderr, "IPC_UDP_C_Tx: Nothing to send\n"); return; }

	if( 0 )
	{
		printf("sizeof(TM_Ctx_t) = %zu\n", sizeof( TM_Ctx_t ));
		printf("Sending %zu bytes\n", len);   // whatever length you pass to sendto
	}
	
    ssize_t sent = sendto( sockfd, buffer, len,  0,( const struct sockaddr* )dest_addr,  sizeof( *dest_addr ));
    if( sent < 0 ){ perror("sendto failed"); } else if((size_t)sent != len) { fprintf(stderr, "sendto: partial send( %zd of %zu bytes)\n", sent, len); }
    // else success – add debug printf 
} 
//=======================================================================================================================================================








//=======================================================================================================================================================
//===============================================================================
// Client Rx Init — listens on SERVER_TX_PORT( receives from Server )
//===============================================================================
void IPC_UDP_C_Rx_init(int *sockfd, struct sockaddr_in *addr, int port, const char *ip )
{
    *sockfd = socket(AF_INET, SOCK_DGRAM, 0 );
    if( *sockfd < 0 ) 
    {
        perror("Client Rx socket creation failed" );
        exit(1 );
    }

    int reuse = 1;
    setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse ));

    // Non-blocking
    fcntl(*sockfd, F_SETFL, fcntl(*sockfd, F_GETFL, 0 ) | O_NONBLOCK );

    memset(addr, 0, sizeof(*addr ));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port );                    // ← SERVER_TX_PORT( 5001 )
    addr->sin_addr.s_addr = inet_addr(ip );

    if( bind(*sockfd,( struct sockaddr* )addr, sizeof(*addr )) < 0 ) {
        perror("Client Rx bind failed" );
        exit(1 );
    }
    printf("Client Rx( non-blocking ) listening on %s:%d  ← receiving from Server\n", ip, port );
}

//===============================================================================
// Convert UDP 
//===============================================================================

void Convert_UDP_Msg( const uint8_t* buffer, size_t len, TM_Stx_t* TM_in )
{
    if (buffer == NULL || TM_in == NULL || len < sizeof(TM_Stx_t)) {
        fprintf(stderr, "Convert_UDP_Msg: Invalid input (len=%zu)\n", len);
        return;
    }

    // Packet is already little-endian (Python used '<' formats).
    // Just copy the bytes — do NOT call ntohl/ntohs.
    memcpy( TM_in, buffer, sizeof(TM_Stx_t));
}

//===============================================================================
// Client Rx — Non-blocking + drain backlog (keep only newest packet)
//===============================================================================
int  IPC_UDP_C_Rx(int sockfd, TM_Stx_t* TM_in )   // 1=got packet, 0=no data, -1=error
{
    uint8_t buffer[BUFFER_SIZE];
    struct sockaddr_in sender_addr;
    socklen_t addr_len;
    ssize_t recv_len;
    int got_packet = 0;

    // Drain the socket completely — keep overwriting with the newest packet
    while (1)
    {
        addr_len = sizeof(sender_addr);
        recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&sender_addr, &addr_len);
        if (recv_len > 0)
        {
        	#if 0
       		printf(">> Packet received (%zd bytes)\n", recv_len);
    		// ---- RAW PACKET DUMP ----
    		printf("Raw packet (%zd bytes):\n", recv_len);
    		for (ssize_t i = 0; i < recv_len; i++)
    		{
    		    printf("%02X ", buffer[i]);
    		    if ((i + 1) % 16 == 0)  printf("\n");
    		}
    		if (recv_len % 16 != 0)  printf("\n");
    		printf("--------------------\n");      
    		#endif        

            Convert_UDP_Msg(buffer, (size_t)recv_len, TM_in );
            got_packet = 1;
        }
        else if (recv_len < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;  		// No more packets in the queue → done
            else{ perror("Client recvfrom failed"); return -1;  }       // real error
        }
        else break;// recv_len == 0 (should not happen with UDP)
        // continue looping to see if there are more packets
    }

    return got_packet;   // 1 = at least one packet was received (latest is in TM_in )
                         // 0 = no packets available
}

//=====================================================================================================
// EOF









