#include "ultra64.h"
#include "linkChildLUT.h"
#include "gLinkChildSkel.h"
#define LOAD_VANILLA 0xFFFFFFFF

// Eye/Mouth textures
// To use these replace each line here with the CI8 texture data that should be in your Skel.c file if you properly included them in your model.
// The order must be exactly the same and all textures must be present

__attribute__((section(".data"))) u8 childEyesOpenTex[64][32] = {0};
__attribute__((section(".data"))) u8 childEyesHalfTex[64][32] = {0};
__attribute__((section(".data"))) u8 childEyesClosedTex[64][32] = {0};
__attribute__((section(".data"))) u8 childEyesRightTex[64][32] = {0};
__attribute__((section(".data"))) u8 childEyesLeftTex[64][32] = {0};
__attribute__((section(".data"))) u8 childEyesWideTex[64][32] = {0};
__attribute__((section(".data"))) u8 childEyesDownTex[64][32] = {0};
__attribute__((section(".data"))) u8 childEyesWincingTex[64][32] = {0};

__attribute__((section(".data"))) u8 childMouthClosedTex[32][32] = {0};
__attribute__((section(".data"))) u8 childMouthHalfTex[32][32] = {0};
__attribute__((section(".data"))) u8 childMouthOpenTex[32][32] = {0};
__attribute__((section(".data"))) u8 childMouthSmileTex[32][32] = {0};


CHILD_LINK_LUT_t CHILD_LINK_LUT = {
    .HEADER = "MODLOADER64",
    
    .hiltMtx = {
        .m = {
                {
                    0x00010000,
                    0x00000000,
                    0x00000001,
                    0x00000000,
                },
                {
                    0x00000000,
                    0x00010000,
                    0xFE48FF2D,
                    0x00000001,
                },
                {
                    0x00000000,
                    0x00000000,
                    0x00000000,
                    0x00000000,
                },
                {
                    0x00000000,
                    0x00000000,
                    0x00000000,
                    0x00000000,
                }
        }
    },

    .dekuOddMtx = {
        .m = {
            {
                0xFFFF0000,
                0x00000000,
                0x0000FFFF,
                0x00000000,
            },
            {
                0x00000000,
                0x00010000,
                0x02210000,
                0x00500001,
            },
            {
                0x00000000,
                0x00000000,
                0x00000000,
                0x00000000,
            },
            {
                0x00000000,
                0x00000000,
                0x00000000,
                0x00000000,
            }
        }
    },

    .dekuBackMtx = {
        .m = {
            {
                0x00000000,
                0xFFFF0000,
                0x00010000,
                0x00000000,
            },
            {
                0x0000FFFF,
                0x00000000,
                0x00000000,
                0xFFA60001
            },
            {
                0x00000000,
                0x00000000,
                0x00000000,
                0x00000000,
            },
            {
                0x00000000,
                0x00000000,
                0x00000000,
                0x00000000,
            }
        }
    },

    // Base Parts
    // Update this table with the gsSPBranchList to the DL parts present in the model
    // Set to LOAD_VANILLA to use vanilla parts. These will be imported by the patcher
    // Set to gsSPEndDisplayList to skip drawing part
    
    .LUT_DL_SHIELD_DEKU =           { LOAD_VANILLA },
    .LUT_DL_WAIST =                 { gsSPBranchList(gLinkChildSkel_bone001_gLinkChildWaistLimb_mesh_layer_Opaque) },
    .LUT_DL_RTHIGH =                { gsSPBranchList(gLinkChildSkel_bone003_gLinkChildRightThighLimb_mesh_layer_Opaque) },
    .LUT_DL_RSHIN =                 { gsSPBranchList(gLinkChildSkel_bone004_gLinkChildRightShinLimb_mesh_layer_Opaque) },
    .LUT_DL_RFOOT =                 { gsSPBranchList(gLinkChildSkel_bone005_gLinkChildRightFootLimb_mesh_layer_Opaque) },
    .LUT_DL_LTHIGH =                { gsSPBranchList(gLinkChildSkel_bone006_gLinkChildLeftThighLimb_mesh_layer_Opaque) },
    .LUT_DL_LSHIN =                 { gsSPBranchList(gLinkChildSkel_bone007_gLinkChildLeftShinLimb_mesh_layer_Opaque) },
    .LUT_DL_LFOOT =                 { gsSPBranchList(gLinkChildSkel_bone008_gLinkChildLeftFootLimb_mesh_layer_Opaque) },
    .LUT_DL_HEAD =                  { gsSPBranchList(gLinkChildSkel_bone010_gLinkChildHeadLimb_mesh_layer_Opaque) },
    .LUT_DL_HAT =                   { gsSPBranchList(gLinkChildSkel_bone011_gLinkChildHatLimb_mesh_layer_Opaque) },
    .LUT_DL_COLLAR =                { gsSPBranchList(gLinkChildSkel_bone012_gLinkChildCollarLimb_mesh_layer_Opaque) },
    .LUT_DL_LSHOULDER =             { gsSPBranchList(gLinkChildSkel_bone013_gLinkChildLeftshoulderLimb_mesh_layer_Opaque) },
    .LUT_DL_LFOREARM =              { gsSPBranchList(gLinkChildSkel_bone014_gLinkChildLeftForearmLimb_mesh_layer_Opaque) },
    .LUT_DL_RSHOULDER =             { gsSPBranchList(gLinkChildSkel_bone016_gLinkChildRightshoulderLimb_mesh_layer_Opaque) },
    .LUT_DL_RFOREARM =              { gsSPBranchList(gLinkChildSkel_bone017_gLinkChildRightForearmLimb_mesh_layer_Opaque) },
    .LUT_DL_TORSO =                 { gsSPBranchList(gLinkChildSkel_bone020_gLinkChildTorsoLimb_mesh_layer_Opaque) },
    .LUT_DL_LHAND =                 { gsSPBranchList(gLinkChildSkel_bone015_gLinkChildLeftHandLimb_mesh_layer_Opaque) },
    .LUT_DL_LFIST =                 { gsSPBranchList(gLinkChildSkel_bone015_gLinkChildLeftHandLimb_mesh_layer_Opaque) },
    .LUT_DL_LHAND_BOTTLE =          { gsSPBranchList(gLinkChildSkel_bone015_gLinkChildLeftHandLimb_mesh_layer_Opaque) },
    .LUT_DL_RHAND =                 { gsSPBranchList(gLinkChildSkel_bone018_gLinkChildRightHandLimb_mesh_layer_Opaque) },
    .LUT_DL_RFIST =                 { gsSPBranchList(gLinkChildSkel_bone018_gLinkChildRightHandLimb_mesh_layer_Opaque) },
    .LUT_DL_SWORD_SHEATH =          { LOAD_VANILLA },
    .LUT_DL_SWORD_HILT =            { LOAD_VANILLA },
    .LUT_DL_SWORD_BLADE =           { LOAD_VANILLA },
    .LUT_DL_SLINGSHOT =             { LOAD_VANILLA },
    .LUT_DL_OCARINA_FAIRY =         { LOAD_VANILLA },
    .LUT_DL_OCARINA_TIME =          { LOAD_VANILLA },
    .LUT_DL_DEKU_STICK =            { LOAD_VANILLA },
    .LUT_DL_BOOMERANG =             { LOAD_VANILLA },
    .LUT_DL_SHIELD_HYLIAN_BACK =    { LOAD_VANILLA },
    .LUT_DL_BOTTLE =                { LOAD_VANILLA },
    .LUT_DL_MASTER_SWORD =          { LOAD_VANILLA },
    .LUT_DL_GORON_BRACELET =        { LOAD_VANILLA },
    .LUT_DL_FPS_RIGHT_ARM =         { gsSPBranchList(&CHILD_LINK_LUT.LUT_DL_RHAND) },
    .LUT_DL_SLINGSHOT_STRING =      { LOAD_VANILLA },
    .LUT_DL_MASK_BUNNY =            { LOAD_VANILLA },
    .LUT_DL_MASK_GERUDO =           { LOAD_VANILLA },
    .LUT_DL_MASK_GORON =            { LOAD_VANILLA },
    .LUT_DL_MASK_KEATON =           { LOAD_VANILLA },
    .LUT_DL_MASK_SPOOKY =           { LOAD_VANILLA },
    .LUT_DL_MASK_TRUTH =            { LOAD_VANILLA },
    .LUT_DL_MASK_ZORA =             { LOAD_VANILLA },
    .LUT_DL_MASK_SKULL =            { LOAD_VANILLA },

    // Aggregate DLs
    .DL_SWORD_SHEATHED =            {
                                        gsSPMatrix(&CHILD_LINK_LUT.hiltMtx, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW),
                                        gsSPDisplayList(CHILD_LINK_LUT.LUT_DL_SWORD_HILT),
                                        gsSPPopMatrix(G_MTX_MODELVIEW),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_SWORD_SHEATH),
                                    },
    .LUT_DL_SWORD_SHEATHED =        { gsSPBranchList(CHILD_LINK_LUT.DL_SWORD_SHEATHED) },
    .DL_SHIELD_DEKU_ODD =           {
                                        gsSPMatrix(&CHILD_LINK_LUT.dekuBackMtx, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_SHIELD_DEKU),
                                    },
    .LUT_DL_SHIELD_DEKU_ODD =       { gsSPBranchList(CHILD_LINK_LUT.DL_SHIELD_DEKU_ODD) },
    .DL_SHIELD_DEKU_BACK =          {
                                        gsSPMatrix(&CHILD_LINK_LUT.dekuOddMtx, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_SHIELD_DEKU),
                                    },
    .LUT_DL_SHIELD_DEKU_BACK =      { gsSPBranchList(CHILD_LINK_LUT.DL_SHIELD_DEKU_BACK) },
    .DL_SWORD_SHIELD_HYLIAN =       {
                                        gsSPDisplayList(CHILD_LINK_LUT.DL_SWORD_SHEATHED),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_SHIELD_HYLIAN_BACK),
                                    },
    .LUT_DL_SWORD_SHIELD_HYLIAN =   { gsSPBranchList(CHILD_LINK_LUT.DL_SWORD_SHIELD_HYLIAN) },
    .DL_SWORD_SHIELD_DEKU =         {
                                        gsSPDisplayList(CHILD_LINK_LUT.DL_SWORD_SHEATHED),
                                        gsSPBranchList(CHILD_LINK_LUT.DL_SHIELD_DEKU_BACK),
                                    },
    .LUT_DL_SWORD_SHIELD_DEKU =     { gsSPBranchList(CHILD_LINK_LUT.DL_SWORD_SHIELD_DEKU) },
    .DL_SHEATH0_HYLIAN =            {
                                        gsSPDisplayList(CHILD_LINK_LUT.DL_SWORD_SHEATHED),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_SHIELD_HYLIAN_BACK),
                                    },
    .LUT_DL_SHEATH0_HYLIAN =        { gsSPBranchList(CHILD_LINK_LUT.DL_SHEATH0_HYLIAN) },
    .DL_SHEATH0_DEKU =              {
                                        gsSPDisplayList(CHILD_LINK_LUT.LUT_DL_SWORD_SHEATH),
                                        gsSPBranchList(CHILD_LINK_LUT.DL_SHIELD_DEKU_BACK),
                                    },
    .LUT_DL_SHEATH0_DEKU =          { gsSPBranchList(CHILD_LINK_LUT.DL_SHEATH0_DEKU) },
    .DL_LFIST_SWORD =               {
                                        gsSPDisplayList(CHILD_LINK_LUT.LUT_DL_SWORD_HILT),
                                        gsSPDisplayList(CHILD_LINK_LUT.LUT_DL_SWORD_BLADE),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_LFIST),
                                    },
    .LUT_DL_LFIST_SWORD =           { gsSPBranchList(CHILD_LINK_LUT.DL_LFIST_SWORD) },
    .DL_LHAND_PEDESTALSWORD =       { 
                                        gsSPDisplayList(CHILD_LINK_LUT.LUT_DL_MASTER_SWORD),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_LHAND),
                                    },
    .LUT_DL_LHAND_PEDESTALSWORD =   { gsSPBranchList(CHILD_LINK_LUT.DL_LHAND_PEDESTALSWORD) },
    .DL_LFIST_BOOMERANG =           { 
                                        gsSPDisplayList(CHILD_LINK_LUT.LUT_DL_BOOMERANG),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_LFIST),
                                    },
    .LUT_DL_LFIST_BOOMERANG =       { gsSPBranchList(CHILD_LINK_LUT.DL_LFIST_BOOMERANG) },
    .DL_RFIST_SHIELD_DEKU =         {
                                        gsSPDisplayList(CHILD_LINK_LUT.LUT_DL_SHIELD_DEKU),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_RFIST),
                                    },
    .LUT_DL_RFIST_SHIELD_DEKU =     { gsSPBranchList(CHILD_LINK_LUT.DL_RFIST_SHIELD_DEKU) },
    .DL_RFIST_SLINGSHOT =           {
                                        gsSPDisplayList(CHILD_LINK_LUT.LUT_DL_SLINGSHOT),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_RFIST),
                                    },
    .LUT_DL_RFIST_SLINGSHOT =       { gsSPBranchList(CHILD_LINK_LUT.DL_RFIST_SLINGSHOT) },
    .DL_RHAND_OCARINA_FAIRY =       {
                                        gsSPDisplayList(CHILD_LINK_LUT.LUT_DL_OCARINA_FAIRY),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_RHAND),
                                    },
    .LUT_DL_RHAND_OCARINA_FAIRY =   { gsSPBranchList(CHILD_LINK_LUT.DL_RHAND_OCARINA_FAIRY) },
    .DL_RHAND_OCARINA_TIME =        {
                                        gsSPDisplayList(CHILD_LINK_LUT.LUT_DL_OCARINA_TIME),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_RHAND)
                                    },
    .LUT_DL_RHAND_OCARINA_TIME =    { gsSPBranchList(CHILD_LINK_LUT.DL_RHAND_OCARINA_TIME) },
    .DL_FPS_RARM_SLINGSHOT =        { 
                                        gsSPDisplayList(CHILD_LINK_LUT.LUT_DL_SLINGSHOT),
                                        gsSPBranchList(CHILD_LINK_LUT.LUT_DL_FPS_RIGHT_ARM),
                                    },
    .LUT_DL_FPS_RARM_SLINGSHOT =    { gsSPBranchList(CHILD_LINK_LUT.DL_FPS_RARM_SLINGSHOT) },
    .skeleton = {
		.sh = {
			.segment = gLinkChildSkelLimbs,
			.limbCount = 21,
		},
		.dListCount = 18
	}
};
