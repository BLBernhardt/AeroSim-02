//345678901234567890123456789012345678901234567890123456789012345678901234567890121345678901234567890
//===================================================================================================
// aero_model.c
//===================================================================================================

#include "aero_model.h"
#include <stdio.h>
#include <string.h>

int AeroModel_Init(AeroModel* model)
{
    if (!model) return 0;
    memset(model, 0, sizeof(AeroModel));

	// 1D  Table Loader
    if( !CoeffTable1D_LoadFromFile(&model->CL_alpha, "0-AeroData/CL_alpha.tbl" )) 
    {
        printf("ERROR: Failed to load CL_alpha.tbl\n");
        return 0;
    }
    
    // 2D  Table Loader
    if (!CoeffTable2D_LoadFromFile(&model->CL_alpha_mach, "0-AeroData/CL_alpha_mach.tbl")) 
    {
        printf("ERROR: Failed to load CL_alpha_mach.tbl\n");
        return 0;
    }

    model->tables_loaded = 1;
    printf("SUCCESS: AeroModel Tables initialized.\n");
    return 1;
}

void AeroModel_Destroy(AeroModel* model)
{
    if (model) model->tables_loaded = 0;
}

void AeroModel_PrintSummary(const AeroModel* model)
{
    if (!model) return;
    printf("\n=== AeroModel Summary ===\n");
    printf("Tables Loaded: %s\n\n", model->tables_loaded ? "YES" : "NO");
    
    printf("1D Tables:\n");
    printf("   CL_alpha       : %zu points\n", model->CL_alpha.size);
    
    
    printf("2D Tables:\n");
    printf("   CL_alpha_mach: %zu x %zu\n", model->CL_alpha_mach.size1, model->CL_alpha_mach.size2);
     
}

//===================================================================================================

