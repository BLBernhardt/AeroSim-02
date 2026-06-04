
/* matrix.h */
#ifndef MATRIX_H
#define MATRIX_H

#include "Math.h"
#include "Vector.h"
#include "Matrix.h"



/* Construction / Destruction */
Matrix* 		Matrix_Create(int rows, int cols);
Matrix* 		Matrix_Create4x4(void);           /* Recommended for your use case */
void    		Matrix_Destroy(Matrix* m);

/* Initialization */
void    		Matrix_InitIdentity(Matrix* m);

/* Element Access */
double  		Matrix_Get(const Matrix* m, int row, int col);
void    		Matrix_Set(Matrix* m, int row, int col, double value);

/* Core Operation */
struct Vector3 	Matrix_MultiplyVector(const Matrix* m, const struct Vector3* v);

/* Utility */
void    		Matrix_Print(const Matrix* m);

#endif /* MATRIX_H */
