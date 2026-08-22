//=============================================================================================
// Common_H
//=============================================================================================
#if 0
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
#endif
//========================================================================
// IPC_UDP_C_Rx.h

#if 0
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



//========================================================================
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

#endif


//========================================================================
// IPC_UDP_C_Tx.c

#include "IPC_UDP_Common.h"
#include "Client_RxTx.h"

void IPC_UDP_C_Tx_init
(
    int *sockfd,
    struct sockaddr_in *server_addr,
    int port,
    const char *server_ip
)
{
    if ((*sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(server_addr, 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &server_addr->sin_addr);
}

void IPC_UDP_C_Tx
(
    int sockfd,
    const struct sockaddr_in *server_addr,
    int var1,
    int var2
)
{
    char message[BUFFER_SIZE];
    socklen_t addr_len = sizeof(*server_addr);

    snprintf(message, sizeof(message), "%d,%d", var1, var2);

    if (sendto(sockfd, message, strlen(message), 0,
               (struct sockaddr *)server_addr, addr_len) < 0)
    {
        perror("Client send failed");
    }
    else
    {
        printf("Client Sent: %s\n", message);
    }
}




//========================================================================
// IPC_UDP_C_Rx.c


#include "IPC_UDP_Common.h"
#include "Client_RxTx.h"

void IPC_UDP_C_Rx_init
(
    int *sockfd,
    int port
)
{
    int opt = 1;

    if ((*sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(*sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Client Rx bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Client Rx bound to port %d\n", port);
}



int IPC_UDP_C_Rx
(
    int sockfd,
    char *buffer,
    size_t buffer_size
)
{
    struct sockaddr_in from_addr;
    socklen_t addr_len = sizeof(from_addr);
    int len = recvfrom(sockfd, buffer, buffer_size - 1, 0,
                       (struct sockaddr *)&from_addr, &addr_len);

    if (len < 0)
    {
        perror("Client receive failed");
        return -1;
    }

    buffer[len] = '\0';
    return len;
}



#if 0

//====================================================================================
// Client main
//====================================================================================


#include "IPC_UDP_Common.h"
#include "Client_RxTx.h"

volatile sig_atomic_t running = 1;

void handle_sigint(int sig)
{
    (void)sig;
    running = 0;
}

int main(void)
{
    int tx_sockfd, rx_sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int var1 = 0;
    int var2 = 100;
    int port = CLIENT_TX_PORT;           // Send to server's listening port
    const char *server_ip = DEFAULT_SERVER_IP;

    signal(SIGINT, handle_sigint);

    IPC_UDP_C_Tx_init(&tx_sockfd, &server_addr, port, server_ip);
    IPC_UDP_C_Rx_init(&rx_sockfd, SERVER_TX_PORT);


    printf("Client started (sending to port %d). Press Ctrl+C to stop.\n", port);

    while (running)
    {
        // Send
        IPC_UDP_C_Tx(tx_sockfd, &server_addr, var1, var2);

        // Drain receive buffer to keep up
        while (running)
        {
            fd_set readfds;
            struct timeval tv = {0, 50000};

            FD_ZERO(&readfds);
            FD_SET(rx_sockfd, &readfds);

            if (select(rx_sockfd + 1, &readfds, NULL, NULL, &tv) <= 0)
            {
                break;
            }

            if (IPC_UDP_C_Rx(rx_sockfd, buffer, BUFFER_SIZE) > 0)
            {
                printf("Client Received: %s\n", buffer);
            }
        }

        var1 += 1;
        var2 -= 1;
        usleep(200000);
    }

    close(tx_sockfd);
    close(rx_sockfd);
    printf("\nClient shutdown gracefully.\n");

    return 0;
}

#endif
//=========================================================================================
