//============================================================================
// aero_forces.c
//============================================================================


#include "../0-AeroModel/aero_model.h"
#include "../0-AeroModel/coeff_table.h"

/* aero_forces.c */
void Aero_ComputeCoefficients(const AeroModel* aero, double alpha_deg, double mach, 
                              double* CL, double* CD, double* Cm)
{
    if (!aero || !aero->tables_loaded) {
        *CL = *CD = *Cm = 0.0;
        return;
    }

    /* Use 2D tables when Mach is significant, otherwise fall back to 1D */
    if (mach > 0.15) {
        *CL = CoeffTable2D_Interpolate(&aero->CL_alpha_mach, alpha_deg, mach);
        *CD = CoeffTable2D_Interpolate(&aero->CD_alpha_mach, alpha_deg, mach);
    } else {
        *CL = CoeffTable1D_Interpolate(&aero->CL_alpha, alpha_deg);
        *CD = CoeffTable1D_Interpolate(&aero->CD_alpha, alpha_deg);
    }

    *Cm = CoeffTable1D_Interpolate(&aero->Cm_alpha, alpha_deg);
}


