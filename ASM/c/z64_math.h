#ifndef Z_MATH_H
#define Z_MATH_H

#include "stdint.h"

typedef struct z64_xyz_t
{
  int16_t x;
  int16_t y;
  int16_t z;
} z64_xyz_t;

typedef struct z64_xyz_s32_t
{
  int32_t x;
  int32_t y;
  int32_t z;
} z64_xyz_s32_t;

typedef struct z64_xyzf_t
{
  float x;
  float y;
  float z;
} z64_xyzf_t;

typedef uint16_t z64_angle_t;
typedef struct
{
  z64_angle_t x;
  z64_angle_t y;
  z64_angle_t z;
} z64_rot_t;

typedef struct Plane {
    z64_xyzf_t normal;
    float originDist;
} Plane; // size = 0x10

typedef struct TriNorm {
    z64_xyzf_t vtx[3];
    Plane plane;
} TriNorm; // size = 0x34

typedef float MtxF_t[4][4];
typedef union MtxF {
    MtxF_t mf;
    struct {
        // Note: The order displayed here is the transpose of the order in which matrices are typically written.
        // For example, [xw, yw, zw] is the translation part of the matrix, not [wx, wy, wz].
        float xx, yx, zx, wx,
              xy, yy, zy, wy,
              xz, yz, zz, wz,
              xw, yw, zw, ww;
    };
} MtxF;


typedef struct Sphere16 {
    z64_xyz_t center;
    int16_t radius;
} Sphere16; // size = 0x08

#endif
