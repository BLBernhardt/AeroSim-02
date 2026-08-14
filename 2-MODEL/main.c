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
    double mach = 1.1;

    CL = CoeffTable2D_Interpolate(&aero.CL_alpha_mach, alpha, mach );
    printf("2D: At Alpha=%.2f deg,  → CL = %.4f\n", alpha, CL);
    CL = CoeffTable1D_Interpolate( &aero.CL_alpha, alpha );
    printf("1D: At Alpha=%.2f deg,  → CL = %.4f\n", alpha, CL);

    AeroModel_Destroy(&aero);
    return 0;
}






