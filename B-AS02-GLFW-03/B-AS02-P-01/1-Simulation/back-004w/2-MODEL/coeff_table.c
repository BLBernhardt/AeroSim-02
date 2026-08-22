
//===========================================================================================
// coeff_table.c
//===========================================================================================

#include "coeff_table.h"
#include <stdio.h>
#include <stdlib.h>

static void skip_comments(FILE* fp)
{
    char line[512];
    long pos;
    while (1) {
        pos = ftell(fp);
        if (!fgets(line, sizeof(line), fp)) break;
        if (line[0] == '#' || line[0] == '/' || line[0] == '\n' || line[0] == '\r') continue;
        fseek(fp, pos, SEEK_SET);
        break;
    }
}

/* 1D */
int CoeffTable1D_LoadFromFile(CoeffTable1D* table, const char* filename)
{
    if (!table || !filename) return 0;
    FILE* fp = fopen(filename, "r");
    if (!fp) return 0;

    table->size = 0;
    skip_comments(fp);

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '/' || line[0] == '\n') continue;
        double bp, val;
        if (sscanf(line, "%lf %lf", &bp, &val) == 2) {
            if (table->size >= COEFF_TABLE_MAX_SIZE) break;
            table->breakpoints[table->size] = bp;
            table->data[table->size] = val;
            table->size++;
        }
    }
    fclose(fp);
    return (table->size >= 2) ? 1 : 0;
}

double CoeffTable1D_Interpolate(const CoeffTable1D* table, double x)
{
    if (!table || table->size < 2) return 0.0;
    if (x <= table->breakpoints[0]) return table->data[0];
    if (x >= table->breakpoints[table->size-1]) return table->data[table->size-1];

    size_t low = 0, high = table->size - 1;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (table->breakpoints[mid] < x) low = mid + 1;
        else high = mid;
    }
    size_t i = (low == 0) ? 0 : low - 1;
    if (i >= table->size - 1) i = table->size - 2;

    double x1 = table->breakpoints[i];
    double x2 = table->breakpoints[i+1];
    double y1 = table->data[i];
    double y2 = table->data[i+1];
    double t = (x - x1) / (x2 - x1);
    return y1 + t * (y2 - y1);
}

/* 2D */
int CoeffTable2D_LoadFromFile(CoeffTable2D* table, const char* filename)
{
    if (!table || !filename) return 0;
    FILE* fp = fopen(filename, "r");
    if (!fp) return 0;

    table->size1 = 0;
    table->size2 = 0;

    skip_comments(fp);

    char line[1024];
    if (fgets(line, sizeof(line), fp)) {
        double m1, m2, m3;
        if (sscanf(line, "%*s %lf %lf %lf", &m1, &m2, &m3) == 3) {
            table->x2[0] = m1;
            table->x2[1] = m2;
            table->x2[2] = m3;
            table->size2 = 3;
        }
    }

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '/' || line[0] == '\n') continue;
        if (table->size1 >= COEFF_TABLE_MAX_SIZE) break;

        double alpha, d1, d2, d3;
        if (sscanf(line, "%lf %lf %lf %lf", &alpha, &d1, &d2, &d3) >= 4) {
            table->x1[table->size1] = alpha;
            table->data[table->size1][0] = d1;
            table->data[table->size1][1] = d2;
            table->data[table->size1][2] = d3;
            table->size1++;
        }
    }
    fclose(fp);
    return (table->size1 >= 2 && table->size2 >= 2) ? 1 : 0;
}

double CoeffTable2D_Interpolate(const CoeffTable2D* table, double x1, double x2)
{
    if (!table || table->size1 < 2 || table->size2 < 2) return 0.0;

    size_t i = 0;
    if (x1 <= table->x1[0]) i = 0;
    else if (x1 >= table->x1[table->size1-1]) i = table->size1 - 2;
    else while (i < table->size1-1 && table->x1[i+1] < x1) ++i;

    size_t j = 0;
    if (x2 <= table->x2[0]) j = 0;
    else if (x2 >= table->x2[table->size2-1]) j = table->size2 - 2;
    else while (j < table->size2-1 && table->x2[j+1] < x2) ++j;

    double x1a = table->x1[i], x1b = table->x1[i+1];
    double x2a = table->x2[j], x2b = table->x2[j+1];

    double f00 = table->data[i][j];
    double f10 = table->data[i+1][j];
    double f01 = table->data[i][j+1];
    double f11 = table->data[i+1][j+1];

    double tx = (x1 - x1a) / (x1b - x1a);
    double ty = (x2 - x2a) / (x2b - x2a);

    return (1-tx)*(1-ty)*f00 + tx*(1-ty)*f10 + (1-tx)*ty*f01 + tx*ty*f11;
}


