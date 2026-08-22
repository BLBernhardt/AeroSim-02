//===========================================================================================
//  main.c
//===========================================================================================

#include <stdio.h>

#include "aero_model.h"


int main(void)
{
    

    AeroModel aero = {0};

    if (!AeroModel_Init(&aero))  return 1;

    AeroModel_PrintSummary(&aero);
    

    double alpha = 8.0;
//    double mach = 0.3;
    double CL = 0.0;

//    if (mach > 0.15) {
//        CL = CoeffTable2D_Interpolate(&aero.CL_alpha_mach, alpha, mach);
//    } else {
        CL = CoeffTable1D_Interpolate( &aero.CL_alpha, alpha );
//    }

    printf("At Alpha=%.2f deg,  → CL = %.4f\n", alpha, CL);

    AeroModel_Destroy(&aero);
    return 0;
}






