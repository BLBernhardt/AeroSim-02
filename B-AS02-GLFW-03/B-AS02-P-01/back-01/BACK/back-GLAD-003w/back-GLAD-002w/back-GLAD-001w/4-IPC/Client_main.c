
//=============================================================================

// Client_main.c
#include "../IPC_UDP_Common.h"
#include "Client_TxRx.h"

int tx_sockfd, rx_sockfd;
struct sockaddr_in tx_addr, rx_addr;

volatile sig_atomic_t running = 1;

void handle_sigint(int sig) {
    (void)sig;
    running = 0;
}

void UDP_init()
{
    signal(SIGINT, handle_sigint);
    IPC_UDP_C_Tx_init(&tx_sockfd, &tx_addr, SERVER_TX_PORT, DEFAULT_SERVER_IP);   // Send to Server
    IPC_UDP_C_Rx_init(&rx_sockfd, &rx_addr, SERVER_TX_PORT, DEFAULT_CLIENT_IP);   // Receive on Server's Tx port
    printf("Client started - Tx to:%d, Rx on Server Tx port:%d\n", SERVER_TX_PORT, SERVER_TX_PORT);
}

void UDP_Rx()
{
    // === Receive from Server ===
    TM_Param_t received = {0};
    IPC_UDP_C_Rx( rx_sockfd, &received );
    
    
    

    // TODO: Process received data here (e.g. update your simulation)

    // === Optional: Send back to Server ===
    // static Client_Tx_t reply = {0};
    // ... fill reply ...
    // uint8_t tx_buf[BUFFER_SIZE] = {0};
    // size_t len = Prepare_UDP_Msg(&reply, sizeof(reply), tx_buf, sizeof(tx_buf));
    // IPC_UDP_C_Tx(tx_sockfd, &tx_addr, tx_buf, len);
}

int main(void)
{
    UDP_init();

    while (running)
    {
        UDP_TxRx();
        usleep(100000);        // 10 Hz receive rate - adjust as needed
    }

    close(tx_sockfd);
    close(rx_sockfd);
    printf("\nClient shutdown gracefully.\n");
    return 0;
}
