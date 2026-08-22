//===========================================================================================
// aero_model.h
//===========================================================================================

#ifndef AERO_MODEL_H
#define AERO_MODEL_H

#include "coeff_table.h"

typedef struct {
    CoeffTable1D CL_alpha;
    CoeffTable2D CL_alpha_mach;
    int tables_loaded;
} AeroModel;

int  AeroModel_Init(AeroModel* model);
void AeroModel_Destroy(AeroModel* model);
void AeroModel_PrintSummary(const AeroModel* model);

#endif

