/* Addresses should be listed in order */
z64_EquippedItemOutlineTex = 0x2000E00;
Flags_GetSwitch = 0x8002049C;
Flags_SetSwitch = 0x800204D0;
Flags_UnsetSwitch = 0x80020510;
Flags_GetUnknown = 0x80020554;
Flags_SetUnknown = 0x80020588;
Flags_UnsetUnknown = 0x800205C8;
Flags_GetTreasure = 0x8002060C;
Flags_SetTreasure = 0x80020624;
Flags_GetClear = 0x80020640;
Flags_SetClear = 0x80020658;
Flags_UnsetClear = 0x80020674;
Flags_GetTempClear = 0x80020694;
Flags_SetTempClear = 0x800206AC;
Flags_UnsetTempClear = 0x800206C8;
Flags_GetCollectible = 0x800206E8;
Flags_SetCollectible = 0x8002071C;
Actor_SetColorFilter = 0x80027090;
z64_Item_DropCollectible = 0x80013678;
z64_Item_DropCollectible2 = 0x800138B0;
z64_Item_DropCollectibleRandom = 0x80013A84;
z64_Flags_GetClear = 0x80020640;
z64_Flags_SetSwitch = 0x800204D0;
z64_Flags_GetSwitch = 0x8002049C;
z64_Flags_SetTempClear = 0x800206AC;
OnePointCutscene_Init = 0x8006b6fc;
Player_InCsMode = 0x8007943c;
EffectSsKakera_Spawn = 0x8001d998;
CutsceneFlags_Get = 0x8005991C;
z64_Gfx_SetupDL_25Opa = 0x8007E298;
z64_Gfx_SetupDL_42Opa = 0x8007E868;
Audio_StopCurrentMusic = 0x800C7684;
sprintf = 0x800CE7b4;
Fault_AddHungupAndCrashImpl = 0x800AF564;
z64_ItemIcons = 0x800F8D2C;
z64_SfxDefaultPos = 0x80104394;
z64_SfxDefaultFreqAndVolScale = 0x801043A0;
z64_SfxDefaultReverb = 0x801043A8;
Message_CloseTextbox = 0x800d6218;
ObjHana_Draw = 0x80abc1c8;
SkelAnime_DrawFlexOpa = 0x80089d8c;
Actor_TalkOfferAccepted = 0x80022930;
Actor_OfferTalk = 0x80022a68;
Message_GetState = 0x800DD464;
Message_ShouldAdvance = 0x800d6110;
Message_ContinueTextbox = 0x800dce80;
Message_CloseTextbox = 0x800d6218;
Actor_OfferGetItem = 0x80022bd4;
z64_segments = 0x80120C38;
__osMallocInit = 0x800CDD90;
__osFree = 0x800CE2B4;
__osMalloc = 0x800CE060;
z64_LoadRoom_addr = 0x80080A3C;
z64_UnloadRoom = 0x80080C98;
sSetupDL = 0x800f7d50;
Actor_WorldDistXZToPoint = 0x80021528;
BgCheck_EntityRaycastDown1 = 0x8002f3a4;
BgCheck_EntityRaycastDown2 = 0x8002f400;
BgCheck_EntityRaycastDown3 = 0x8002f454;
BgCheck_EntityRaycastDown4 = 0x8002F4B8;
BgCheck_EntityRaycastDown5 = 0x8002f520;
BgCheck_AnyRaycastDown1 = 0x8002f644;
WaterBox_GetSurface1 = 0x800347fc;
Flags_GetTempClear = 0x80020694;
Actor_UpdateBgCheckInfo = 0x80021e6c;
gCullBackDList = 0x800f7a68;
ZeldaArena_Malloc = 0x80066c10;
ZeldaArena_Free = 0x80066c90;
ZeldaArena_GetSizes = 0x80066d10;
ArenaImpl_GetSizes = 0x800ce688;
z64_LoadRoom_addr = 0x80080A3C;
z64_UnloadRoom = 0x80080C98;
gActorOverlayTable = 0x800E8530;
gPausePlayerOverlayTable = 0x800FE480;
Collider_UpdateSpheres = 0x80050CE4;
Collider_SetJntSph = 0x8004a874;
CollisionCheck_SetAT = 0x8004bd50;
CollisionCheck_SetAC = 0x8004bf40;
CollisionCheck_SetOC = 0x8004c130;

Matrix_Push = 0x800aa6ec;
Matrix_Pop = 0x800aa724;
Matrix_SetTranslateRotateYXZ = 0x800ab510;
Matrix_Scale = 0x800aa8fc;

/* Overlay symbols */

/* En_Ik (Iron Knuckle) */
OVL_EnIk_SetupStandUp = 0x80a67348;
OVL_EnIk_SetupWalkOrRun = 0x80a676e4;


/* Kaleidoscope */
OVL_KaleidoScope_DrawAmmoCount = 0x80819670;
OVL_KaleidoScope_SetCursorVtx = 0x80819BF4;
OVL_KaleidoScope_QuadTextureIA8 = 0x8081EE60;
OVL_KaleidoScope_MoveCursorToSpecialPos = 0x8081F184;
OVL_KaleidoScope_DrawQuadTextureRGBA32 = 0x8081F1E8;
OVL_KaleidoScope_DrawCursor = 0x8081F760;
OVL_gAmmoItems = 0x80829700;
OVL_gSlotAgeReqs = 0x80829D74;
OVL_sEquipState = 0x80829710;
OVL_sEquipAnimTimer = 0x80829714;
OVL_sEquipMoveTimer = 0x80829718;


/* Door_Shutter */
OVL_DoorShutter_BarAndWaitSwitchFlag = 0x808b8fe8;

/* En_Dekubaba */
OVL_EnDekubaba_SetupPrepareLunge = 0x808fbc50;
OVL_EnDekubaba_SetupGrow = 0x808fb9e0;

/* En_Fd */
OVL_EnFd_Update = 0x80956378;

/* En_Mb */
OVL_EnMb_Init = 0x808e5d4c;
OVL_EnMb_SetupClubWaitPlayerNear = 0x808e6590;
OVL_EnMb_ClubWaitPlayerNear = 0x808e8a54;

/* Player */
OVL_Player_UseItem = 0x80834000;
OVL_LinkInvincibility_addr = 0x80835BD8;
OVL_LinkDamage_addr = 0x80835D08;

/* Bg_Mori_Bigst */
OVL_BgMoriBigst_SetupFall = 0x809273d4;
OVL_BgMoriBigst_SetupDone = 0x809276c0;

/* En_karebaba */
OVL_EnKarebaba_SetupDeadItemDrop = 0x809ba560;

/* En_Tp */
OVL_EnTp_Update = 0x808c39c8;

/* Obj_Roomtimer */
OVL_ObjRoomtimer_MainAction = 0x80b25428;

OVL_EnKusa_Draw = 0x80a80a50;
OVL_EnOkarinaTag_Action2 = 0x80a872d0;
OVL_EnOkarinaTag_Action1 = 0x80a87088;
