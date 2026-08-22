//===========================================================================================
// coeff_table.h
//===========================================================================================

#ifndef COEFF_TABLE_H
#define COEFF_TABLE_H

#include <stddef.h>

#define COEFF_TABLE_MAX_SIZE 256U

typedef struct {
    double breakpoints[COEFF_TABLE_MAX_SIZE];
    double data[COEFF_TABLE_MAX_SIZE];
    size_t size;
} CoeffTable1D;

typedef struct {
    double x1[COEFF_TABLE_MAX_SIZE];
    double x2[COEFF_TABLE_MAX_SIZE];
    double data[COEFF_TABLE_MAX_SIZE][COEFF_TABLE_MAX_SIZE];
    size_t size1;
    size_t size2;
} CoeffTable2D;

/* 1D Functions */
int CoeffTable1D_LoadFromFile(CoeffTable1D* table, const char* filename);
double CoeffTable1D_Interpolate(const CoeffTable1D* table, double x);

/* 2D Functions */
int CoeffTable2D_LoadFromFile(CoeffTable2D* table, const char* filename);
double CoeffTable2D_Interpolate(const CoeffTable2D* table, double x1, double x2);

#endif
