

//==============================================================================================================
// Client_TxRx.c
#include "../IPC_UDP_Common.h"
#include "Client_TxRx.h"
#include <fcntl.h>
#include <errno.h>      // errno, EAGAIN, EWOULDBLOCK




//===============================================================================
// IPC Client Tx Init  (now non-blocking)
//===============================================================================
void IPC_UDP_C_Tx_init(int *sockfd, struct sockaddr_in *addr, int port, const char *ip)
{
    *sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (*sockfd < 0) {
        perror("Tx socket creation failed");
        exit(1);
    }

    // Make the socket non-blocking
    fcntl(*sockfd, F_SETFL, fcntl(*sockfd, F_GETFL, 0) | O_NONBLOCK);

    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port   = htons(port);
    addr->sin_addr.s_addr = inet_addr(ip);

    printf("Tx ready to send to %s:%d (non-blocking)\n", ip, port);
}

//===============================================================================
// Pack UDP Message (host → network byte order)
//===============================================================================
void Pack_UDP_Tx_Msg(const Client_Tx_t* src, uint8_t* buffer, size_t* out_len)
{
    if (src == NULL || buffer == NULL || out_len == NULL) {
        fprintf(stderr, "Pack_UDP_Msg: Invalid input\n");
        *out_len = 0;
        return;
    }

    // Copy structure
    memcpy(buffer, src, sizeof(Client_Tx_t));

    // Convert host → network byte order (32-bit words)
    uint32_t* p = (uint32_t*)buffer;
    size_t num_words = sizeof(Client_Tx_t) / 4;
    for (size_t i = 0; i < num_words; ++i) {
        p[i] = htonl(p[i]);
    }

    *out_len = sizeof(Client_Tx_t);
}

//===============================================================================
// IPC Client Tx  (non-blocking)
// Returns:  1 = sent successfully
//           0 = would block (EAGAIN / EWOULDBLOCK) – try again later
//          -1 = real error
//===============================================================================
int IPC_UDP_C_Tx(int sockfd, const struct sockaddr_in* dest_addr, const Client_Tx_t* packet)
{
    uint8_t buffer[BUFFER_SIZE] = {0};
    size_t len = 0;

    Pack_UDP_Tx_Msg(packet, buffer, &len);
    if (len == 0) {
        fprintf(stderr, "IPC_UDP_C_Tx: Nothing to send\n");
        return -1;
    }

    ssize_t sent = sendto(sockfd, buffer, len, 0,
                          (const struct sockaddr*)dest_addr, sizeof(*dest_addr));

    if (sent < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Socket send buffer is full – non-blocking, try again later
            return 0;
        } else {
            perror("sendto failed");
            return -1;
        }
    }

    if ((size_t)sent != len) {
        fprintf(stderr, "sendto: partial send (%zd of %zu bytes)\n", sent, len);
        return -1;
    }

    return 1;   // success
}

#if 0
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
void Pack_UDP_Tx_Msg(const Client_Tx_t* src, uint8_t* buffer, size_t* out_len )
{
    if( src == NULL || buffer == NULL || out_len == NULL) 
    {
        fprintf(stderr, "Pack_UDP_Msg: Invalid input\n");
        *out_len = 0;
        return;
    }

    // Copy structure
    memcpy(buffer, src, sizeof( Client_Tx_t ));

    // Convert host → network byte order( 32-bit words)
    uint32_t* p =( uint32_t*)buffer;
    size_t num_words = sizeof( Client_Tx_t) / 4;
    for( size_t i = 0; i < num_words; ++i) { p[i] = htonl(p[i]); }

    *out_len = sizeof( Client_Tx_t);
}

//===============================================================================
// IPC Client Tx
//===============================================================================
void IPC_UDP_C_Tx(int sockfd, const struct sockaddr_in* dest_addr, const Client_Tx_t* packet )
{
    uint8_t buffer[BUFFER_SIZE] = {0};
    size_t  len = 0;

    Pack_UDP_Tx_Msg( packet, buffer, &len );
    if( len == 0 ){ fprintf(stderr, "IPC_UDP_C_Tx: Nothing to send\n"); return; }

	if( 0 )
	{
		printf("sizeof(Client_Tx_t) = %zu\n", sizeof( Client_Tx_t ));
		printf("Sending %zu bytes\n", len);   // whatever length you pass to sendto
	}
	
    ssize_t sent = sendto( sockfd, buffer, len,  0,( const struct sockaddr* )dest_addr,  sizeof( *dest_addr ));
    if( sent < 0 ){ perror("sendto failed"); } else if((size_t)sent != len) { fprintf(stderr, "sendto: partial send( %zd of %zu bytes)\n", sent, len); }
    // else success – add debug printf 
} 
//=======================================================================================================================================================
#endif








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
void Convert_UDP_Msg(const uint8_t* buffer, size_t len, Server_Tx_t* dst )
{
    if( buffer == NULL || dst == NULL || len < sizeof( Server_Tx_t )) {
        fprintf(stderr, "Parse_UDP_Msg: Invalid input( len=%zu )\n", len );
        return;
    }

    // Copy raw bytes first
    memcpy(dst, buffer, sizeof( Server_Tx_t ));
#if 1
    // Convert EVERY 4-byte word from network to host order
    uint32_t* p =( uint32_t* )dst;
    size_t num_words = sizeof( Server_Tx_t ) / 4;

 //   printf("Debug: Received %zu bytes, converting %zu words\n", len, num_words ); // Temporary debug

    for( size_t i = 0; i < num_words; ++i ) 
    {
        p[i] = ntohl(p[i] );
    }
#endif

#if 0
    // Handle the last few bytes that are not full 32-bit words
    if( sizeof(TM_sTx ) % 4 != 0  ) {
        // Handle remaining bytes( Stick_Enable, key, spinner )
        size_t remainder = sizeof(TM_sTx ) % 4;
        // Usually not needed for this struct, but safe
    }
#endif


}

//===============================================================================
// Client Rx — Non-blocking + drain backlog (keep only newest packet)
//===============================================================================
int  IPC_UDP_C_Rx(int sockfd, Server_Tx_t* TMrx)   // 1=got packet, 0=no data, -1=error
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
        recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                            (struct sockaddr*)&sender_addr, &addr_len);

        if (recv_len > 0)
        {
            // Valid packet received → convert it (overwrites previous)
            Convert_UDP_Msg(buffer, (size_t)recv_len, TMrx);
            got_packet = 1;
            // continue looping to see if there are more packets
        }
        else if (recv_len < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // No more packets in the queue → done
                break;
            }
            else
            {
                perror("Client recvfrom failed");
                return -1;          // real error
            }
        }
        else
        {
            // recv_len == 0 (should not happen with UDP)
            break;
        }
    }

    return got_packet;   // 1 = at least one packet was received (latest is in TMrx)
                         // 0 = no packets available
}


//=====================================================================================================
// EOF









