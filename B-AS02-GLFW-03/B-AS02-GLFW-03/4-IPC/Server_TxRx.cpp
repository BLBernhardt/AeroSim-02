
//===========================================================================================================================
// Server_TxRx.c


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>          // <-- required
#include <errno.h>          // <-- required
		
		


#include "../IPC_UDP_Common.h"
#include "Server_TxRx.h"




// ===================================================================
// UDP Server Tx Init (non-blocking)
// ===================================================================
void IPC_UDP_S_Tx_init(int *sockfd, struct sockaddr_in *client_addr, int port, const char *ip)
{
    if ((*sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror(">>> ERROR <<< Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Make transmit socket non-blocking
    int flags = fcntl(*sockfd, F_GETFL, 0);
    if (flags < 0) {
        perror("fcntl F_GETFL failed");
        exit(EXIT_FAILURE);
    }
    if (fcntl(*sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("fcntl F_SETFL O_NONBLOCK failed");
        exit(EXIT_FAILURE);
    }

    memset(client_addr, 0, sizeof(*client_addr));
    client_addr->sin_family = AF_INET;
    client_addr->sin_port   = htons(port);
    inet_pton(AF_INET, ip, &client_addr->sin_addr);
}

// ===================================================================
// Prepare UDP Message (with byte order conversion)
// ===================================================================
size_t Prepare_UDP_Msg(const void* src, size_t struct_size, uint8_t* buffer, size_t bufferSize)
{
    if (src == NULL || buffer == NULL) {
        fprintf(stderr, "Prepare_UDP_Msg: NULL pointer\n");
        return 0;
    }
    if (bufferSize < struct_size) {
        fprintf(stderr, "Prepare_UDP_Msg: Buffer too small (%zu < %zu)\n",
                bufferSize, struct_size);
        return 0;
    }

    memcpy(buffer, src, struct_size);

    // Convert every 4-byte word to network byte order
    uint32_t* p = (uint32_t*)buffer;
    size_t num_words = struct_size / 4;
    for (size_t i = 0; i < num_words; ++i) {
        p[i] = htonl(p[i]);
    }

    return struct_size;
}

//===================================================================================================================
// IPC Server Tx (non-blocking)
// Returns:  1 = sent successfully
//           0 = would block (send buffer full) – packet dropped
//          -1 = error
//===================================================================================================================
int IPC_UDP_S_Tx(int sockfd, const struct sockaddr_in *client_addr, const uint8_t *pkt, size_t pkt_len)
{
    if (pkt == NULL || pkt_len == 0) {
        fprintf(stderr, "IPC_UDP_S_Tx: Invalid packet (null or zero length)\n");
        return -1;
    }

    socklen_t addr_len = sizeof(*client_addr);
    ssize_t sent = sendto(sockfd, pkt, pkt_len, 0,
                          (const struct sockaddr *)client_addr, addr_len);

    if (sent < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // Send buffer full – drop this packet (prefer latest data next cycle)
            return 0;
        }
        perror("Server sendto failed");
        return -1;
    }

    if ((size_t)sent != pkt_len) {
        fprintf(stderr, "Warning: Partial send (%zd of %zu bytes)\n", sent, pkt_len);
        return -1;
    }

    return 1;   // success
}
//===================================================================================================================
//===============================================================================





//===============================================================================
// IPC Server Rx Init  (non-blocking)
//===============================================================================
void IPC_UDP_S_Rx_init(int *sockfd, struct sockaddr_in *addr, int port, const char *ip)
{
    *sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (*sockfd < 0) {
        perror("Rx socket creation failed");
        exit(1);
    }

    int reuse = 1;
    setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // Make socket non-blocking
    int flags = fcntl(*sockfd, F_GETFL, 0);
    if (flags < 0) {
        perror("fcntl F_GETFL failed");
        exit(1);
    }
    if (fcntl(*sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("fcntl F_SETFL O_NONBLOCK failed");
        exit(1);
    }

    memset(addr, 0, sizeof(*addr));
    addr->sin_family      = AF_INET;
    addr->sin_port        = htons(port);
    addr->sin_addr.s_addr = inet_addr(ip);

    if (bind(*sockfd, (struct sockaddr*)addr, sizeof(*addr)) < 0) {
        perror("Rx bind failed");
        exit(1);
    }

    printf("Rx listening (non-blocking) on %s:%d\n", ip, port);
}


//===============================================================================
// Parse UDP Message
//===============================================================================
void Parse_UDP_Msg(const uint8_t* buffer, size_t len, Client_Tx_t* dst)
{
    if (buffer == NULL || dst == NULL || len < sizeof(Client_Tx_t)) {
        fprintf(stderr, "Parse_UDP_Msg: Invalid input\n");
        return;
    }

    memcpy(dst, buffer, sizeof(Client_Tx_t));

    // Convert from network to host byte order
    uint32_t* p = (uint32_t*)dst;
    size_t num_words = sizeof(Client_Tx_t) / sizeof(uint32_t);

    for (size_t i = 0; i < num_words; ++i) {
        p[i] = ntohl(p[i]);
    }
}


//===============================================================================
// IPC Server Rx (non-blocking)
// Returns:  1 = packet received
//           0 = no data available
//          -1 = error
//===============================================================================
int IPC_UDP_S_Rx(int sockfd, Client_Tx_t* received)
{
    uint8_t buffer[BUFFER_SIZE] = {0};
    struct sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr);

    ssize_t recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                (struct sockaddr*)&sender_addr, &addr_len);

    if (recv_len > 0) {
        Parse_UDP_Msg(buffer, (size_t)recv_len, received);
        return 1;
    }

    if (recv_len < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0;               // no data – normal for non-blocking
        }
        perror("recvfrom failed");
        return -1;
    }

    return 0;
}















