#include "ultra64.h"
#include "linkAdultLUT.h"
#include "gLinkAdultSkel.h"

#define LOAD_VANILLA 0xFFFFFFFF

// Eye/Mouth textures
// To use these replace each line here with the CI8 texture data that should be in your Skel.c file if you properly included them in your model.
// The order must be exactly the same and all textures must be present
__attribute__((section(".data"))) u8 adultEyesOpenTex[64][32] = {0};
__attribute__((section(".data"))) u8 adultEyesHalfTex[64][32] = {0};
__attribute__((section(".data"))) u8 adultEyesClosedTex[64][32] = {0};
__attribute__((section(".data"))) u8 adultEyesRightTex[64][32] = {0};
__attribute__((section(".data"))) u8 AdultEyesLeftTex[64][32] = {0};
__attribute__((section(".data"))) u8 AdultEyesWideTex[64][32] = {0};
__attribute__((section(".data"))) u8 AdultEyesDownTex[64][32] = {0};
__attribute__((section(".data"))) u8 AdultEyesWincingTex[64][32] = {0};

__attribute__((section(".data"))) u8 adultMouthClosedTex[32][32] = {0};
__attribute__((section(".data"))) u8 adultMouthHalfTex[32][32] = {0};
__attribute__((section(".data"))) u8 adultMouthOpenTex[32][32] = {0};
__attribute__((section(".data"))) u8 adultMouthSmileTex[32][32] = {0};

ADULT_LINK_LUT_t ADULT_LINK_LUT = {
    .HEADER = "MODLOADER64",
//	.HEADER = "~FAST64~",
    
    .hiltSheathedMtx = {
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
                    0xFD35FECA,
                    0x004E0001,
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

    .shieldBackMtx = {
        .m = {
            {
                0xFFFF0000,
                0x00000000,
                0x0000FFFF,
                0x00000000
            },
            {
                0x00000000,
                0x00010000,
                0x03A7005E,
                0x001D0001, 
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
    // Set to LOAD_VANILLA to use vanilla parts. These will be automatically added to the model by the compilation script
    // Set to gsSPEndDisplayList to skip drawing part
    
    .DL_WAIST =                     { gsSPBranchList(gLinkAdultSkel_bone001_gLinkAdultWaistLimb_mesh_layer_Opaque) },
    .DL_RTHIGH =                    { gsSPBranchList(gLinkAdultSkel_bone003_gLinkAdultRightThighLimb_mesh_layer_Opaque) },
    .DL_RSHIN =                     { gsSPBranchList(gLinkAdultSkel_bone004_gLinkAdultRightLegLimb_mesh_layer_Opaque) },
    .DL_RFOOT =                     { gsSPBranchList(gLinkAdultSkel_bone005_gLinkAdultRightFootLimb_mesh_layer_Opaque) },
    .DL_LTHIGH =                    { gsSPBranchList( gLinkAdultSkel_bone006_gLinkAdultLeftThighLimb_mesh_layer_Opaque ) },
    .DL_LSHIN =                     { gsSPBranchList( gLinkAdultSkel_bone007_gLinkAdultLeftLegLimb_mesh_layer_Opaque ) },
    .DL_LFOOT =                     { gsSPBranchList( gLinkAdultSkel_bone008_gLinkAdultLeftFootLimb_mesh_layer_Opaque ) },
    .DL_HEAD =                      { gsSPBranchList( gLinkAdultSkel_bone010_gLinkAdultHeadLimb_mesh_layer_Opaque ) },
    .DL_HAT =                       { gsSPBranchList( gLinkAdultSkel_bone011_gLinkAdultHatLimb_mesh_layer_Opaque ) },
    .DL_COLLAR =                    { gsSPBranchList( gLinkAdultSkel_bone012_gLinkAdultCollarLimb_mesh_layer_Opaque ) },
    .DL_LSHOULDER =                 { gsSPBranchList( gLinkAdultSkel_bone013_gLinkAdultLeftShoulderLimb_mesh_layer_Opaque ) },
    .DL_LFOREARM =                  { gsSPBranchList( gLinkAdultSkel_bone014_gLinkAdultLeftArmLimb_mesh_layer_Opaque ) },
    .DL_RSHOULDER =                 { gsSPBranchList( gLinkAdultSkel_bone016_gLinkAdultRightShoulderLimb_mesh_layer_Opaque ) },
    .DL_RFOREARM =                  { gsSPBranchList( gLinkAdultSkel_bone017_gLinkAdultRightArmLimb_mesh_layer_Opaque ) },
    .DL_TORSO =                     { gsSPBranchList( gLinkAdultSkel_bone020_gLinkTorsoLimb_mesh_layer_Opaque ) },
    .DL_LHAND =                     { gsSPBranchList( gLinkAdultSkel_bone015_gLinkAdultLeftHandLimb_mesh_layer_Opaque) },
    // Left Fist DL. Change this to use a separate left fist model, otherwise it will use the open left hand
	.DL_LFIST =                     { gsSPBranchList( gLinkAdultSkel_bone015_gLinkAdultLeftHandLimb_mesh_layer_Opaque) },
    .DL_LHAND_BOTTLE =              { gsSPBranchList( gLinkAdultSkel_bone015_gLinkAdultLeftHandLimb_mesh_layer_Opaque) },
    .DL_RHAND =                     { gsSPBranchList( gLinkAdultSkel_bone018_gLinkAdultRightHandLimb_mesh_layer_Opaque) },
    // Right Fist DL. Change this to use a separate right fist model, otherwise it will use the open left hand
	.DL_RFIST =                     { gsSPBranchList( gLinkAdultSkel_bone018_gLinkAdultRightHandLimb_mesh_layer_Opaque) },
    .DL_SWORD_SHEATH =              { LOAD_VANILLA },
    .DL_SWORD_HILT =                { LOAD_VANILLA },
    .DL_SWORD_BLADE =               { LOAD_VANILLA },
    .DL_LONGSWORD_HILT =            { LOAD_VANILLA },
    .DL_LONGSWORD_BLADE =           { LOAD_VANILLA },
    .DL_LONGSWORD_BROKEN =          { LOAD_VANILLA },
    .DL_SHIELD_HYLIAN =             { LOAD_VANILLA },
    .DL_SHIELD_MIRROR =             { LOAD_VANILLA },
    .DL_HAMMER =                    { LOAD_VANILLA },
    .DL_BOTTLE =                    { LOAD_VANILLA },
    .DL_BOW =                       { LOAD_VANILLA },
    .DL_OCARINA_TIME =              { LOAD_VANILLA },
    .DL_HOOKSHOT =                  { LOAD_VANILLA },
    .DL_UPGRADE_LFOREARM =          { LOAD_VANILLA },
    .DL_UPGRADE_LHAND =             { LOAD_VANILLA },
    .DL_UPGRADE_LFIST =             { LOAD_VANILLA },
    .DL_UPGRADE_RFOREARM =          { LOAD_VANILLA },
    .DL_UPGRADE_RHAND =             { LOAD_VANILLA },
    .DL_UPGRADE_RFIST =             { LOAD_VANILLA },
    .DL_BOOT_LIRON =                { LOAD_VANILLA },
    .DL_BOOT_RIRON =                { LOAD_VANILLA },
    .DL_BOOT_LHOVER =               { LOAD_VANILLA },
    .DL_BOOT_RHOVER =               { LOAD_VANILLA },
	// Left Forearm FPS DL (unused by adult)
    .DL_FPS_LFOREARM =              { gsSPBranchList( gLinkAdultSkel_bone014_gLinkAdultLeftArmLimb_mesh_layer_Opaque) },
    // Left Hand FPS DL for Hookshot
	.DL_FPS_LHAND =                 { gsSPBranchList( gLinkAdultSkel_bone015_gLinkAdultLeftHandLimb_mesh_layer_Opaque) },
	// Right Forearm FPS DL (unused by adult)
	.DL_FPS_RFOREARM =              { gsSPBranchList( gLinkAdultSkel_bone017_gLinkAdultRightArmLimb_mesh_layer_Opaque) },
    // Right Hand FPS DL for Bow
	.DL_FPS_RHAND =                 { gsSPBranchList( gLinkAdultSkel_bone018_gLinkAdultRightHandLimb_mesh_layer_Opaque) },
    .DL_FPS_HOOKSHOT =              { LOAD_VANILLA },
    .DL_HOOKSHOT_CHAIN =            { LOAD_VANILLA },
    .DL_HOOKSHOT_HOOK =             { LOAD_VANILLA },
    .DL_HOOKSHOT_AIM =              { LOAD_VANILLA },
    .DL_BOW_STRING =                { LOAD_VANILLA },
    .DL_BLADEBREAK =                { LOAD_VANILLA },
    // Aggregate parts. Do not edit unless you know what you're doing
    .DL_SWORD_SHEATHED =            { 
                                        gsSPMatrix(&ADULT_LINK_LUT.hiltSheathedMtx, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW), // Push Hilt Matrix. Should be at 0x5010
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_SWORD_HILT),
                                        gsSPPopMatrix(G_MTX_MODELVIEW), // Pop Hilt Matrix
                                        gsSPBranchList(ADULT_LINK_LUT.DL_SWORD_SHEATH),
                                    },
    .DL_SHIELD_HYLIAN_BACK =        {
                                        gsSPMatrix(&ADULT_LINK_LUT.shieldBackMtx, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW), // Push Shield Matrix. Should be at 0x5050
                                        gsSPBranchList(ADULT_LINK_LUT.DL_SHIELD_HYLIAN), 
                                    },
    .DL_SHIELD_MIRROR_BACK =        {
                                        gsSPMatrix(&ADULT_LINK_LUT.shieldBackMtx, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW), // Push Shield Matrix. Should be at 0x5050
                                        gsSPBranchList(ADULT_LINK_LUT.DL_SHIELD_MIRROR),
                                    },
    .DL_SWORD_SHIELD_HYLIAN =       {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_SWORD_SHEATHED),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_SHIELD_HYLIAN_BACK),
                                    },
    .DL_SWORD_SHIELD_MIRROR =       {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_SWORD_SHEATHED),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_SHIELD_MIRROR_BACK),
                                    },
    .DL_SHEATH0_HYLIAN =            {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_SWORD_SHEATH),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_SHIELD_HYLIAN_BACK),
                                    },
    .DL_SHEATH0_MIRROR =            {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_SWORD_SHEATH),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_SHIELD_MIRROR_BACK),
                                    },
    .DL_LFIST_SWORD =               {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_SWORD_HILT),
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_SWORD_BLADE),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_LFIST),
                                    },
    .DL_LFIST_LONGSWORD =           {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_LONGSWORD_HILT),
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_LONGSWORD_BLADE),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_LFIST),
                                    },
    .DL_LFIST_LONGSWORD_BROKEN =    {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_LONGSWORD_HILT),
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_LONGSWORD_BROKEN),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_LFIST),
                                    },
    .DL_LFIST_HAMMER =              {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_HAMMER),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_LFIST),
                                    },
    .DL_RFIST_SHIELD_HYLIAN =       {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_SHIELD_HYLIAN),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_RFIST),
                                    },
    .DL_RFIST_SHIELD_MIRROR =       {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_SHIELD_MIRROR),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_RFIST),
                                    },
    .DL_RFIST_BOW =                 {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_BOW),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_RFIST),
                                    },
    .DL_RFIST_HOOKSHOT =            {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_HOOKSHOT),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_RFIST),
                                    },
    .DL_RHAND_OCARINA_TIME =        {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_OCARINA_TIME),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_RHAND),
                                    },
    .DL_FPS_RHAND_BOW =             {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_BOW),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_FPS_RHAND),
                                    },
    .DL_FPS_LHAND_HOOKSHOT =        {
                                        gsSPDisplayList(ADULT_LINK_LUT.DL_FPS_HOOKSHOT),
                                        gsSPBranchList(ADULT_LINK_LUT.DL_FPS_RHAND),
                                    },
	.skeleton = {
		.sh = {
			.segment = gLinkAdultSkelLimbs,
			.limbCount = 21,
		},
		.dListCount = 18
	}
};
