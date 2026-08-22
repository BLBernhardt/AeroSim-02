
//===========================================================================
// Server Main
//===========================================================================


#include "IPC_UDP_Common.h"
#include "Server_TxRx.h"


volatile sig_atomic_t running = 1;

void handle_sigint(int sig)
{
    (void)sig;
    running = 0;
}

int main(void)
{
    int tx_sockfd, rx_sockfd;
    struct sockaddr_in tx_addr, rx_addr;
    char buffer[BUFFER_SIZE];
    int var1 = 0;
    int var2 = 100;

    signal(SIGINT, handle_sigint);

    IPC_UDP_S_Tx_init(&tx_sockfd, &tx_addr, SERVER_TX_PORT, DEFAULT_SERVER_IP);
    IPC_UDP_S_Rx_init(&rx_sockfd, &rx_addr, CLIENT_TX_PORT);

    printf("Server started Ports: Tx:%d, Rx:%d      Press Ctrl+C to stop.\n", 
           SERVER_TX_PORT, CLIENT_TX_PORT);

    while (running)
    {
        // Send
        IPC_UDP_S_Tx(tx_sockfd, &tx_addr, var1, var2);

        // Drain receive buffer
        while (running)
        {
            fd_set readfds;
            struct timeval tv = {0, 50000};

            FD_ZERO(&readfds);
            FD_SET(rx_sockfd, &readfds);

            if (select(rx_sockfd + 1, &readfds, NULL, NULL, &tv) <= 0)
                break;

            if (IPC_UDP_S_Rx(rx_sockfd, &rx_addr, buffer, BUFFER_SIZE) > 0)
            {
                printf("Server Received: %s\n", buffer);
            }
        }

        var1 += 1;
        var2 -= 1;
        usleep(200000);
    }

    close(tx_sockfd);
    close(rx_sockfd);
    printf("\nServer shutdown gracefully.\n");
    return 0;
}

//=======================================================================

