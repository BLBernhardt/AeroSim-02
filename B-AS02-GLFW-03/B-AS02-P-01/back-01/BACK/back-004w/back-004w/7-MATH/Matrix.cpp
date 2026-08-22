

/* matrix.c */
#include <stdio.h>
#include <stdlib.h>

#include "Math.h"
#include "Vector.h"
#include "Matrix.h"




/* Construction */
Matrix* Matrix_Create(int rows, int cols)
{
    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    if (m == NULL) return NULL;
    
    m->rows = rows;
    m->cols = cols;
    m->data = (double*)malloc(rows * cols * sizeof(double));
    
    if (m->data == NULL)
    {
        free(m);
        return NULL;
    }
    
    for (int i = 0; i < rows * cols; ++i)
        m->data[i] = 0.0;
    
    return m;
}

Matrix* Matrix_Create4x4(void)
{
    return Matrix_Create(4, 4);
}

void Matrix_Destroy(Matrix* m)
{
    if (m != NULL)
    {
        if (m->data != NULL)
            free(m->data);
        free(m);
    }
}

/* Initialization */
void Matrix_InitIdentity(Matrix* m)
{
    if (m == NULL || m->data == NULL) return;
    
    for (int i = 0; i < m->rows * m->cols; ++i)
        m->data[i] = 0.0;
    
    if (m->rows == 4 && m->cols == 4)
    {
        Matrix_Set(m, 0, 0, 1.0);
        Matrix_Set(m, 1, 1, 1.0);
        Matrix_Set(m, 2, 2, 1.0);
        Matrix_Set(m, 3, 3, 1.0);
    }
}

/* Element Access */
double Matrix_Get(const Matrix* m, int row, int col)
{
    if (m == NULL || row < 0 || col < 0 || 
        row >= m->rows || col >= m->cols)
        return 0.0;
    
    return m->data[row * m->cols + col];
}

void Matrix_Set(Matrix* m, int row, int col, double value)
{
    if (m == NULL || row < 0 || col < 0 || 
        row >= m->rows || col >= m->cols)
        return;
    
    m->data[row * m->cols + col] = value;
}

/* Matrix * Vector3 Multiplication */
struct Vector3 Matrix_MultiplyVector(const Matrix* m, const struct Vector3* v)
{
    struct Vector3 result = {0.0f, 0.0f, 0.0f};
    
    if (m == NULL || v == NULL) return result;
    
    result.x = (float)(Matrix_Get(m, 0, 0) * v->x +
                       Matrix_Get(m, 0, 1) * v->y +
                       Matrix_Get(m, 0, 2) * v->z +
                       Matrix_Get(m, 0, 3));
    
    result.y = (float)(Matrix_Get(m, 1, 0) * v->x +
                       Matrix_Get(m, 1, 1) * v->y +
                       Matrix_Get(m, 1, 2) * v->z +
                       Matrix_Get(m, 1, 3));
    
    result.z = (float)(Matrix_Get(m, 2, 0) * v->x +
                       Matrix_Get(m, 2, 1) * v->y +
                       Matrix_Get(m, 2, 2) * v->z +
                       Matrix_Get(m, 2, 3));
    
    return result;
}

/* Utility */
void Matrix_Print(const Matrix* m)
{
    if (m == NULL) return;
    
    for (int row = 0; row < m->rows; row++)
    {
        printf("| ");
        for (int col = 0; col < m->cols; col++)
        {
            printf("%8.3f ", Matrix_Get(m, row, col));
            if (col != m->cols - 1)
                printf(", ");
        }
        printf(" |\n");
    }
}


//=====================================================================================
