//===========================================================================================
// aero_model.c
//===========================================================================================

#include "aero_model.h"
#include <stdio.h>
#include <string.h>

int AeroModel_Init(AeroModel* model)
{
    if (!model) return 0;
    memset(model, 0, sizeof(AeroModel));

    if( !CoeffTable1D_LoadFromFile(&model->CL_alpha, "1-DATA/CL_alpha.tbl" )) 
    {
        printf("ERROR: Failed to load CL_alpha.tbl\n");
        return 0;
    }
    
    
    
    
    #if 0
    if (!CoeffTable2D_LoadFromFile(&model->CL_alpha_mach, "CL_alpha_mach.tbl")) {
        printf("ERROR: Failed to load CL_alpha_mach.tbl\n");
        return 0;
    }
#endif
    model->tables_loaded = 1;
    printf("SUCCESS: AeroModel (CL only) initialized.\n");
    return 1;
}

void AeroModel_Destroy(AeroModel* model)
{
    if (model) model->tables_loaded = 0;
}

void AeroModel_PrintSummary(const AeroModel* model)
{
    if (!model) return;
    printf("\n=== AeroModel Summary (CL only) ===\n");
    printf("Tables Loaded: %s\n\n", model->tables_loaded ? "YES" : "NO");
    printf("CL_alpha       : %zu points\n", model->CL_alpha.size);
    printf("CL_alpha_mach  : %zu x %zu\n\n", model->CL_alpha_mach.size1, model->CL_alpha_mach.size2);
}

