//===============================================================================
// Client main
//===============================================================================


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
    int port = CLIENT_TX_PORT;           
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

    close( tx_sockfd );
    close( rx_sockfd );
    printf("\n Client shutdown,  Tx Rx Closed \n");

    return 0;
}

//===============================================================================
//EOF


