
//===============================================================================
// IPC_UDP_C_Tx.c
//===============================================================================

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




//===============================================================================
// IPC_UDP_C_Rx.c
//===============================================================================

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

//===============================================================================
//EOF



