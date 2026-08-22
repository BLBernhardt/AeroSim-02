
//=============================================================================
// Client_main.c
//
//		TEST:   sudo tshark -i any -f "udp port 5001" -V
//
//




#include "../IPC_UDP_Common.h"
#include "Client_TxRx.h"

int tx_sockfd, rx_sockfd;
struct sockaddr_in tx_addr, rx_addr;

volatile sig_atomic_t running = 1;

void handle_sigint(int sig) 
{
    (void)sig;
    running = 0;
}

void UDP_init()
{
    signal(SIGINT, handle_sigint);
 //   IPC_UDP_C_Tx_init(&tx_sockfd, &tx_addr, SERVER_TX_PORT, DEFAULT_SERVER_IP);   // Send to Server
    IPC_UDP_C_Rx_init(&rx_sockfd, &rx_addr, SERVER_TX_PORT, DEFAULT_CLIENT_IP);   // Receive on Server's Tx port
    printf("Client started - Tx to:%d, Rx on Server Tx port:%d\n", CLIENT_TX_PORT, SERVER_TX_PORT);
}

void UDP_Rx()
{
    TM_Param_t received = {0};

    int got = IPC_UDP_C_Rx( rx_sockfd, &received );

    if( got == 1 )
    {
        printf("RX  frame=%u  Alpha=%.2f  Mach/AirSpeed=%.2f  Pitch=%.2f\n",
               received.frame_cnt,
               received.Alpha,
               received.AirSpeed,      // or Mach if that is what server sends
               received.Pitch);
    }
    else if (got == -1)
    {
        printf("RX error\n");
    }
    // got == 0 → silent (no data this cycle)
}

int main(void)
{
    UDP_init();

    while (running)
    {
        UDP_Rx();
        usleep(100000);        // 10 Hz receive rate - adjust as needed
    }

    close(tx_sockfd);
    close(rx_sockfd);
    printf("\nClient shutdown gracefully.\n");
    return 0;
}


//=================================================================================================================
//EOF



