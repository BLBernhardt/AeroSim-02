

//====================================================================
// IPC_UDP_S_Tx.c
//====================================================================


#include "IPC_UDP_Common.h"
//#include "IPC_UDP_S_Tx.h"

void IPC_UDP_S_Tx_init(int *sockfd) {
    if ((*sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
}

void IPC_UDP_S_Tx(int sockfd, const struct sockaddr_in *client_addr, int var1, int var2) {
    char message[BUFFER_SIZE];
    socklen_t addr_len = sizeof(*client_addr);

    snprintf(message, sizeof(message), "%d,%d", var1, var2);

    if (sendto(sockfd, message, strlen(message), 0,
               (struct sockaddr *)client_addr, addr_len) < 0) {
        perror("Send to client failed");
    } else {
        printf("Server Sent to Client: %s\n", message);
    }
}


//====================================================================
// IPC_UDP_S_Rx.c
//====================================================================

//#include "ipc_udp_common.h"
//#include "IPC_UDP_S_Rx.h"

void IPC_UDP_S_Rx_init(int *sockfd, struct sockaddr_in *addr, int port) {
    int opt = 1;
    if ((*sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = INADDR_ANY;

    if (bind(*sockfd, (struct sockaddr *)addr, sizeof(*addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

int IPC_UDP_S_Rx(int sockfd, struct sockaddr_in *client_addr, char *buffer, size_t buffer_size) {
    socklen_t addr_len = sizeof(*client_addr);
    int len = recvfrom(sockfd, buffer, buffer_size - 1, 0,
                       (struct sockaddr *)client_addr, &addr_len);
    if (len < 0) {
        perror("Receive failed");
        return -1;
    }
    buffer[len] = '\0';
    return len;
}











