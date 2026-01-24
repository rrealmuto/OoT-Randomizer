#ifndef MATRIX_H
#define MATRIX_H

#include "z64.h"


typedef enum MatrixMode {
    /* 0 */ MTXMODE_NEW,  // generates a new matrix
    /* 1 */ MTXMODE_APPLY // applies transformation to the current matrix
} MatrixMode;

void Matrix_Init(struct GameState* gameState);
void Matrix_Push();
void Matrix_Pop();
void Matrix_Get(MtxF* dest);
void Matrix_Put(MtxF* src);

/* Basic operations */

void Matrix_Mult(MtxF* mf, uint8_t mode);
void Matrix_Translate(float x, float y, float z, uint8_t mode);
void Matrix_Scale(float x, float y, float z, uint8_t mode);
void Matrix_RotateX(float x, uint8_t mode);
void Matrix_RotateY(float y, uint8_t mode);
void Matrix_RotateZ(float z, uint8_t mode);

/* Compound operations */

void Matrix_RotateZYX(int16_t x, int16_t y, int16_t z, uint8_t mode);
void Matrix_TranslateRotateZYX(z64_xyzf_t* translation, z64_xyz_t* rotation);
void Matrix_SetTranslateRotateYXZ(float translateX, float translateY, float translateZ, z64_xyz_t* rot);
void Matrix_SetTranslateScaleMtx2(Mtx* mtx, float scaleX, float scaleY, float scaleZ, float translateX, float translateY,
                                  float translateZ);

#endif