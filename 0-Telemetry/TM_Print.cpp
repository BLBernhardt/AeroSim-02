 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <GL/freeglut.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
 
 //#include "0-AeroSim/flightmodel.h"
 #include "../structures.h"
 
 extern struct _TM_Param	TMParam;
 extern struct _Cntrls   	Cntrls;
 
 
 void TM_Print( void );
 
 
void TM_Print()
{
 
 int i = 0;
 
 	if( !Cntrls.abort )
 	{
        // 2. Clear and reprint each line (combined for safety)
        printf("\033[2k  \n");
        printf("\033[2K %10s: %7.2f   %10s: %7.2f  %10s: %7.2f\n",  "test11", 314.23, "test22", 4156.78, "test22", 4156.78  );	
        printf("\033[2K %10s: %7.2f   %10s: %7.2f  %10s: %7.2f\n",  "test12", 314.23, "test22", 4156.78, "test22", 4156.78  );	
        printf("\033[2K %10s: %7.2f   %10s: %7.2f  %10s: %7.2f\n",  "test13", 314.23, "test22", 4156.78, "test22", 4156.78  );	
        printf("\033[2K %20s:%03d \n", "Frame Count:", TMParam.frame_cnt );   


        printf("\033[5A");

        fflush(stdout);
     }

        
}
