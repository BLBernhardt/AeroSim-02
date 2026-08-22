// Server_main.c
//===========================================================================
// Server Main
//===========================================================================
#include "IPC_UDP_Common.h"
#include "Server_TxRx.h"

int tx_sockfd, rx_sockfd;
struct sockaddr_in tx_addr, rx_addr;

volatile sig_atomic_t running = 1;

void handle_sigint(int sig) 
{
    (void)sig;
    running = 0;
}

//==============================================================================
void UDP_init() {
    signal(SIGINT, handle_sigint);
    IPC_UDP_S_Tx_init(&tx_sockfd, &tx_addr, SERVER_TX_PORT, DEFAULT_SERVER_IP);
    IPC_UDP_S_Rx_init(&rx_sockfd, &rx_addr, CLIENT_TX_PORT, DEFAULT_SERVER_IP);
    printf("Server started Ports: Tx:%d, Rx:%d Press Ctrl+C to stop.\n",
           SERVER_TX_PORT, CLIENT_TX_PORT);
}

void UDP_TxRx() {
    static TM_Param_t TM = {0};   // Persistent across calls if you want continuity

    // === Populate TM_Param_t with simulation data ===
    // Update these from your simulation state each frame
    TM.Alpha   = 5.0f;      // example values - replace with real sim data
    TM.Beta    = 2.0f;
    TM.Pitch   = 3.0f;
    TM.Yaw     = 10.0f;
    TM.Roll    = -1.5f;
    TM.Mach    = 0.85f;
    TM.CL      = 0.45f;
    TM.CL2     = 0.47f;
    TM.dt      = 0.016f;    // ~60 Hz example

    // Time
    static uint32_t frame = 0;
    TM.frame_cnt++;
    frame++;

    // Simple time progression (replace with real sim time)
    TM.hours = 14;
    TM.min   = 30;
    TM.sec   = (frame / 60) % 60;
    TM.mSec  = (frame % 60) * 16;   // rough ms

    TM.Stick_Enable = 1;
    TM.key = 'A';                   // example input
//    strncpy(TM.spinner, "X29", 3  );

    // Prepare and send
//------------------------------------------------------------------------------------------------------    
    
    uint8_t tx_buf[BUFFER_SIZE] = {0};
    size_t pkt_len = Prepare_UDP_Msg(&TM, sizeof(TM), tx_buf, sizeof(tx_buf));

    IPC_UDP_S_Tx(tx_sockfd, &tx_addr, tx_buf, pkt_len);
//------------------------------------------------------------------------------------------------------   
    // === Receive side ===
    while (running) {
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




