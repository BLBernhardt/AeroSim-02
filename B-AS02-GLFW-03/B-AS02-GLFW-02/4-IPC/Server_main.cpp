//================================================================================================================================
// Server_main.c
//===========================================================================
// Server Main
//===========================================================================
#include "../IPC_UDP_Common.h"
#include "Server_TxRx.h"

int tx_sockfd, rx_sockfd;
struct sockaddr_in tx_addr, rx_addr;

volatile sig_atomic_t running = 1;

void handle_sigint(int sig) {
    (void)sig;
    running = 0;
}

//==============================================================================
void UDP_init() 
{
    signal(SIGINT, handle_sigint);
    IPC_UDP_S_Tx_init(&tx_sockfd, &tx_addr, SERVER_TX_PORT, DEFAULT_SERVER_IP);
    IPC_UDP_S_Rx_init(&rx_sockfd, &rx_addr, CLIENT_TX_PORT, DEFAULT_SERVER_IP);
    printf("Server started Ports: Tx:%d, Rx:%d Press Ctrl+C to stop.\n", SERVER_TX_PORT, CLIENT_TX_PORT);
}

void UDP_Tx() 
{
    static TM_Param_t packet = {0};   // Persistent across calls if you want continuity

    // === Populate TM_Param_t with simulation data ===
    // Update these from your simulation state each frame
    packet.Alpha   = 5.0f;      // example values - replace with real sim data
    packet.Beta    = 2.0f;
    packet.Pitch   = 3.0f;
    packet.Yaw     = 10.0f;
    packet.Roll    = -1.5f;
    packet.Mach    = 0.85f;
    packet.CL      = 0.45f;
    packet.CL2     = 0.47f;
    packet.dt      = 0.016f;    // ~60 Hz example


    packet.frame_cnt = 02233;
    // Time
//    static uint32_t frame = 0;
 //   packet.frame_cnt++;
//    frame++;

    // Simple time progression (replace with real sim time)
    packet.hours = 14;
    packet.min   = 30;
    packet.sec   = 45;
    packet.mSec  =  5;   // rough ms

    packet.Stick_Enable = 1;
    packet.key = 'A';                   // example input
//    strncpy(packet.spinner, "X29", 3  );

    // Prepare and send
    
    uint8_t tx_buf[BUFFER_SIZE] = {0};
    size_t pkt_len = Prepare_UDP_Msg(&packet, sizeof(packet), tx_buf, sizeof(tx_buf));
    
     //=======================================================================================  
       #if 1
         	//packet.sec   = (frame / 60) % 60;
    		//packet.mSec  = (frame % 60) * 16;   // rough ms
     
     

    uint8_t tx_buf[BUFFER_SIZE] = {0};
        // Fill each byte with its sequential number (1 .. 62)
    for (int i = 0; i < BUFFER_SIZE; i++) {
        tx_buf[i] = (uint8_t)(i + 1);   // 01, 02, 03, ... 62
    }
    size_t pkt_len = 62;
    #endif
  //======================================================================================= 
    
    
    


    IPC_UDP_S_Tx(tx_sockfd, &tx_addr, tx_buf, pkt_len);

    // === Receive side ===
    #if 0
    while (running) 
    {
        fd_set readfds;
        struct timeval tv = {0, 50000};
        FD_ZERO(&readfds);
        FD_SET(rx_sockfd, &readfds);

        if (select(rx_sockfd + 1, &readfds, NULL, NULL, &tv) <= 0)
            break;

        TM_Param_t received = {0};
        IPC_UDP_S_Rx(rx_sockfd, &received);

        // TODO: Process incoming data from client
        // printf("Rx: Alpha=%.2f, Mach=%.2f, frame=%u\n", 
        //        received.Alpha, received.Mach, received.frame_cnt);
    }
    #endif
}

int main(void) {
    UDP_init();

    while (running) {
        UDP_TxRx();
        usleep(200000);   // 5 Hz for now - adjust to your sim rate
    }

    close(tx_sockfd);
    close(rx_sockfd);
    printf("\nServer shutdown gracefully.\n");
    return 0;
}

//===========================================================================================================================
// EOF

		
