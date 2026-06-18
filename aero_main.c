
//============================================================================
// aero_main.c
//============================================================================
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

#include "structures.h"
#include "0-Math/Math.h"
#include "0-Math/Quaternion.h"

#include "0-AeroModel/aero_model.h"
#include "0-AeroModel/coeff_table.h"

void Aero_ComputeCoefficients(const AeroModel* aero, double alpha_deg, double mach, 
                              double* CL, double* CD, double* Cm);
int aero_main(void)
{
    AeroModel aero = {0};

    if (!AeroModel_Init(&aero))  return 1;

    AeroModel_PrintSummary(&aero);

    /* Example usage */
    double alpha = 8.0;
    double mach = 0.3;
    double CL, CD, Cm;

    Aero_ComputeCoefficients(&aero, alpha, mach, &CL, &CD, &Cm);

    printf("At Alpha=%.2f deg, Mach=%.2f → CL=%.4f, CD=%.4f, Cm=%.4f\n", 
           alpha, mach, CL, CD, Cm);

    AeroModel_Destroy(&aero);
    return 0;
}



