#ifndef _BG_CHECK_H_
# define _BG_CHECK_H_


#include <stdint.h>
#include "z64_math.h"

struct z64_actor_t;

#define BG_ACTOR_MAX 50
#define BGACTOR_NEG_ONE -1
#define BGCHECK_SCENE BG_ACTOR_MAX
#define BGCHECK_Y_MIN -32000.0f
#define BGCHECK_XYZ_ABSMAX 32760.0f
#define BGCHECK_SUBDIV_OVERLAP 50
#define BGCHECK_SUBDIV_MIN 150.0f


typedef struct ScaleRotPos {
    /* 0x00 */ z64_xyzf_t scale;
    /* 0x0C */ z64_xyz_t rot;
    /* 0x14 */ z64_xyzf_t pos;
} ScaleRotPos; // size = 0x20

typedef struct WaterBox {
    /* 0x00 */ int16_t xMin;
    /* 0x02 */ int16_t ySurface;
    /* 0x04 */ int16_t zMin;
    /* 0x06 */ int16_t xLength;
    /* 0x08 */ int16_t zLength;
    /* 0x0C */ uint32_t properties;
} WaterBox; // size = 0x10

typedef struct CollisionPoly {
    /* 0x00 */ uint16_t type;
    union {
        uint16_t vtxData[3];
        struct {
            /* 0x02 */ uint16_t flags_vIA; // 0xE000 is poly exclusion flags (xpFlags), 0x1FFF is vtxId
            /* 0x04 */ uint16_t flags_vIB; // 0xE000 is flags, 0x1FFF is vtxId
                                      // 0x2000 = poly IsFloorConveyor surface
            /* 0x06 */ uint16_t vIC;
        };
    };
    /* 0x08 */ z64_xyz_t normal; // Unit normal vector
                             // Value ranges from -0x7FFF to 0x7FFF, representing -1.0 to 1.0; 0x8000 is invalid

    /* 0x0E */ int16_t dist; // Plane distance from origin along the normal
} CollisionPoly; // size = 0x10


typedef struct BgCamInfo {
    /* 0x0 */ uint16_t setting; // camera setting described by CameraSettingType enum
    /* 0x2 */ int16_t count; // only used when `bgCamFuncData` is a list of points used for crawlspaces
    /* 0x4 */ z64_xyz_t* bgCamFuncData; // s16 data grouped in threes (ex. Vec3s), is usually of type `BgCamFuncData`, but can be a list of points of type `Vec3s` for crawlspaces
} BgCamInfo; // size = 0x8

typedef struct SurfaceType {
    uint32_t data[2];
} SurfaceType;

typedef struct CollisionHeader {
    /* 0x00 */ z64_xyz_t minBounds; // minimum coordinates of poly bounding box
    /* 0x06 */ z64_xyz_t maxBounds; // maximum coordinates of poly bounding box
    /* 0x0C */ uint16_t numVertices;
    /* 0x10 */ z64_xyz_t* vtxList;
    /* 0x14 */ uint16_t numPolygons;
    /* 0x18 */ CollisionPoly* polyList;
    /* 0x1C */ SurfaceType* surfaceTypeList;
    /* 0x20 */ BgCamInfo* bgCamList;
    /* 0x24 */ uint16_t numWaterBoxes;
    /* 0x28 */ WaterBox* waterBoxes;
} CollisionHeader;

typedef struct SSList {
    uint16_t head; // first SSNode index
} SSList;

typedef struct SSNode {
    int16_t polyId;
    uint16_t next; // next SSNode index
} SSNode;


typedef struct SSNodeList {
    /* 0x00 */ uint16_t max;          // original name: short_slist_node_size
    /* 0x02 */ uint16_t count;        // original name: short_slist_node_last_index
    /* 0x04 */ SSNode* tbl;      // original name: short_slist_node_tbl
    /* 0x08 */ uint8_t* polyCheckTbl; // points to an array of bytes, one per static poly. Zero initialized when starting a
                                 // bg check, and set to 1 if that poly has already been tested.
} SSNodeList;

typedef struct StaticLookup {
    SSList floor;
    SSList wall;
    SSList ceiling;
} StaticLookup;

typedef struct DynaLookup {
    uint16_t polyStartIndex;
    SSList ceiling;
    SSList wall;
    SSList floor;
} DynaLookup;

typedef struct BgActor {
    /* 0x00 */ struct z64_actor_t* actor;
    /* 0x04 */ CollisionHeader* colHeader;
    /* 0x08 */ DynaLookup dynaLookup;
    /* 0x10 */ uint16_t vtxStartIndex;
    /* 0x14 */ ScaleRotPos prevTransform;
    /* 0x34 */ ScaleRotPos curTransform;
    /* 0x54 */ Sphere16 boundingSphere;
    /* 0x5C */ float minY;
    /* 0x60 */ float maxY;
} BgActor; // size = 0x64

typedef struct DynaSSNodeList {
    SSNode* tbl;
    int32_t count;
    int32_t max;
} DynaSSNodeList;

typedef struct DynaCollisionContext {
    /* 0x0000 */ uint8_t bitFlag;
    /* 0x0004 */ BgActor bgActors[BG_ACTOR_MAX];
    /* 0x138C */ uint16_t bgActorFlags[BG_ACTOR_MAX];
    /* 0x13F0 */ CollisionPoly* polyList;
    /* 0x13F4 */ z64_xyz_t* vtxList;
    /* 0x13F8 */ DynaSSNodeList polyNodes;
    /* 0x1404 */ int32_t polyNodesMax;
    /* 0x1408 */ int32_t polyListMax;
    /* 0x140C */ int32_t vtxListMax;
} DynaCollisionContext; // size = 0x1410

typedef struct CollisionContext {
    /* 0x00 */ CollisionHeader* colHeader; // scene's static collision
    /* 0x04 */ z64_xyzf_t minBounds;            // minimum coordinates of collision bounding box
    /* 0x10 */ z64_xyzf_t maxBounds;            // maximum coordinates of collision bounding box
    /* 0x1C */ z64_xyz_s32_t subdivAmount;         // x, y, z subdivisions of the scene's static collision
    /* 0x28 */ z64_xyzf_t subdivLength;         // x, y, z subdivision worldspace lengths
    /* 0x34 */ z64_xyzf_t subdivLengthInv;      // inverse of subdivision length
    /* 0x40 */ StaticLookup* lookupTbl;    // 3d array of length subdivAmount
    /* 0x44 */ SSNodeList polyNodes;
    /* 0x50 */ DynaCollisionContext dyna;
    /* 0x1460 */ uint32_t memSize; // Size of all allocated memory plus CollisionContext
} CollisionContext; // size = 0x1464

struct z64_game_t;

extern float BgCheck_EntityRaycastDown1(CollisionContext* colCtx, CollisionPoly** outGroundPoly, z64_xyzf_t* pos);
extern float BgCheck_EntityRaycastDown2(struct z64_game_t* globalCtx, CollisionContext* colCtx, CollisionPoly** outGroundPoly, z64_xyzf_t* pos);
extern float BgCheck_EntityRaycastDown3(CollisionContext* colCtx, CollisionPoly** outGroundPoly, int32_t* bgId, z64_xyzf_t* pos);
extern float BgCheck_EntityRaycastDown4(CollisionContext* colCtx, CollisionPoly** outGroundPoly, int32_t* bgId, struct z64_actor_t* actor, z64_xyzf_t* pos);
extern float BgCheck_EntityRaycastDown5(struct z64_game_t* globalCtx, CollisionContext* colCtx, CollisionPoly** outGroundPoly, int32_t* bgId, struct z64_actor_t* actor, z64_xyzf_t* pos);
extern float BgCheck_AnyRaycastDown1(CollisionContext* colCtx, CollisionPoly* outGroundPoly, z64_xyzf_t* pos);
extern int32_t WaterBox_GetSurface1(struct z64_game_t* globalCtx, CollisionContext* colCtx, float x, float z, float* ySurface, WaterBox** outWaterBox);
#endif
