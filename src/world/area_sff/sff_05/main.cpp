#include "sff_05.h"

namespace sff_05 {

#include "world/common/entity/Pipe.inc.c"

#include "world/common/atomic/TexturePan.inc.c"

#include "../common/ManageSnowfall.inc.cpp"

EvtScript EVS_SetupMusic = {
    Switch(GB_StoryProgress)
        CaseLt(STORY_CH3_ARRIVED_AT_BOOS_MANSION)
            Call(SetMusicTrack, 0, SONG_BOOS_MANSION, 1, 8)
        CaseDefault
            Call(SetMusicTrack, 0, SONG_FOREVER_FOREST, 0, 8)
    EndSwitch
    Call(PlayAmbientSounds, AMBIENT_SPOOKY)
    Return
    End
};

API_CALLABLE(SetFogAndBackgroundColor) {
    enable_world_fog();
    set_world_fog_dist(950, 1010);
    set_world_fog_color(75, 120, 170, 255);

    enable_entity_fog();
    set_entity_fog_dist(950, 1010); // Was 990, 1000
    set_entity_fog_color(75, 120, 170, 255); // Was 0, 0, 0

    gCameras[CAM_DEFAULT].bgColor[0] = 24;
    gCameras[CAM_DEFAULT].bgColor[1] = 24;
    gCameras[CAM_DEFAULT].bgColor[2] = 24;
    return ApiStatus_DONE2;
}

API_CALLABLE(func_8024066C_BB76DC) {
    Camera* cam = &gCameras[gCurrentCameraID];
    s32 retVal = ApiStatus_BLOCK;

    if (isInitialCall) {
        script->functionTemp[1] = 0;
        script->functionTemp[2] = 0;
        script->functionTemp[3] = 100;
    }

    switch (script->functionTemp[1]) {
        case 0:
            script->functionTemp[2] = script->functionTemp[2] + script->functionTemp[3];
            if (script->functionTemp[2] >= 16000) {
                script->functionTemp[1] = 1;
            }
            break;
        case 1:
            script->functionTemp[2] += script->functionTemp[3];
            script->functionTemp[3] -= 2;
            if (script->functionTemp[2] >= 18000) {
                retVal = ApiStatus_DONE2;
            }
            break;
    }
    cam->overrideSettings.boomPitch = (script->functionTemp[2] * 0.0010500001f) + 40.0f;
    cam->overrideSettings.viewPitch = (script->functionTemp[2] * 0.0025833333f) + -63.6f;
    return retVal;
}

API_CALLABLE(func_80240790_BB7800) {
    Camera* cam = &gCameras[CAM_DEFAULT];
    f32 angle1, angle2, moveAngle;

    if (isInitialCall) {
        script->functionTemp[1] = 0;
    }

    angle1 = atan2(89.0f, 84.0f, 75.0f, 309.0f);
    angle2 = atan2(89.0f, 84.0f, 245.0f, 85.0f);
    moveAngle = (((angle2 - angle1) / 40.0f) * script->functionTemp[1]) + angle1;
    cam->movePos.x = (s32) (sin_deg(moveAngle) * 100.0f) + 89;
    cam->movePos.z = (s32)(-cos_deg(moveAngle) * 100.0f) + 84;
    cam->panActive = true;

    script->functionTemp[1]++;
    if (script->functionTemp[1] < 41) {
        return ApiStatus_BLOCK;
    } else {
        return ApiStatus_DONE2;
    }
}

EvtScript EVS_Scene_ReachedMansion = {
    Call(DisablePlayerInput, true)
    Call(DisablePlayerPhysics, true)
    Call(SetPlayerPos, -800, 0, 0)
    Call(SetNpcPos, NPC_PARTNER, -800, 0, 0)
    Call(SetCamSpeed, CAM_DEFAULT, Float(90.0))
    Call(SetCamType, CAM_DEFAULT, CAM_CONTROL_LOOK_AT_POINT, true)
    Call(SetCamPitch, CAM_DEFAULT, Float(40.0), Float(-63.59375))
    Call(SetCamDistance, CAM_DEFAULT, 720)
    Call(SetCamPosA, CAM_DEFAULT, 89, 84)
    Call(SetCamPosB, CAM_DEFAULT, 89, 84)
    Call(SetCamPosC, CAM_DEFAULT, 0, 0)
    Call(SetPanTarget, CAM_DEFAULT, 75, 0, 309)
    Call(PanToTarget, CAM_DEFAULT, 0, true)
    Call(func_8024066C_BB76DC)
    Wait(30)
    ChildThread
        Call(func_80240790_BB7800)
        Wait(90)
        Call(SetCamSpeed, CAM_DEFAULT, Float(90.0))
        Loop(0)
            Call(GetPlayerPos, LVar0, LVar1, LVar2)
            Call(UseSettingsFrom, CAM_DEFAULT, LVar0, LVar1, LVar2)
            Call(SetPanTarget, CAM_DEFAULT, LVar0, LVar1, LVar2)
            Call(SetCamDistance, CAM_DEFAULT, -350)
            Call(WaitForCam, CAM_DEFAULT, Float(1.0))
        EndLoop
    EndChildThread
    Wait(50)
    Call(SetPlayerPos, -701, 0, -34)
    Call(SetNpcPos, NPC_PARTNER, -701, 0, -34)
    Call(DisablePlayerPhysics, false)
    Call(SetPlayerFlagBits, PS_FLAG_NO_FLIPPING, true)
    Call(PlayerMoveTo, -407, 103, 120)
    Call(SetPlayerFlagBits, PS_FLAG_NO_FLIPPING, false)
    Wait(10)
    Thread
        Wait(15)
        Call(PlayerFaceNpc, NPC_PARTNER, false)
    EndThread
    Call(DisablePartnerAI, 0)
    Call(GetCurrentPartnerID, LVar0)
    Switch(LVar0)
        CaseEq(PARTNER_GOOMBARIO)
            Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldGoombario_Talk, ANIM_WorldGoombario_Idle, 5, MSG_CH3_0024)
        CaseEq(PARTNER_KOOPER)
            Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldKooper_Talk, ANIM_WorldKooper_Idle, 5, MSG_CH3_0025)
        CaseEq(PARTNER_BOMBETTE)
            Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBombette_Talk, ANIM_WorldBombette_Idle, 5, MSG_CH3_0026)
        CaseEq(PARTNER_PARAKARRY)
            Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldParakarry_Talk, ANIM_WorldParakarry_Idle, 5, MSG_CH3_0027)
    EndSwitch
    Call(EnablePartnerAI)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_EnterJumping = {
    Call(DisablePlayerInput, true)
    Call(DisablePlayerPhysics, true)
    Call(SetPlayerActionState, ACTION_STATE_JUMP)
    Wait(1)
    Call(SetPlayerJumpscale, Float(1.0))
    Call(PlayerJump, -62, 0, -553, 20)
    Call(DisablePlayerPhysics, false)
    Call(DisablePlayerInput, false)
    Call(SetPlayerActionState, ACTION_STATE_IDLE)
    Return
    End
};

EvtScript EVS_ExitFall_sff_07_0 = {
    Loop(0)
        Wait(1)
        Call(GetPlayerPos, LVar0, LVar1, LVar2)
        IfLt(LVar1, -35)
            BreakLoop
        EndIf
    EndLoop
    Call(DisablePlayerInput, true)
    Call(DisablePlayerPhysics, true)
    Wait(30)
    Call(GotoMap, Ref("sff_07"), sff_07_ENTRY_0)
    Wait(100)
    Call(DisablePlayerInput, false)
    Call(DisablePlayerPhysics, false)
    Return
    End
};

EvtScript EVS_GotoMap_sff_07_0 = {
    Call(GotoMap, Ref("sff_07"), sff_07_ENTRY_0)
    Wait(100)
    Return
    End
};

// interesting broken, unused snippet for starting next peach sequence
EvtScript EVS_ExitWarp_osr_03_4 = {
    Call(FadeOutMusic, 0, 1500)
    Call(GotoMapSpecial, Ref("osr_03"), osr_03_ENTRY_4, 0xB)
    Wait(100)
};

EvtScript EVS_ExitWalk_sff_03_3 = {
    SetGroup(EVT_GROUP_EXIT_MAP)
    Call(UseExitHeading, 60, sff_05_ENTRY_0)
    Exec(ExitWalk)
    Set(GF_MIM_ChoosingPath, false)
    Call(GotoMap, Ref("sff_03"), sff_03_ENTRY_3)
    Wait(100)
    Return
    End
};

EvtScript EVS_ExitWalk_sff_06_0 = EVT_EXIT_WALK(60, sff_05_ENTRY_1, "sff_06", sff_06_ENTRY_0);

EvtScript EVS_ExitWalk_obk_01_0 = EVT_EXIT_DOUBLE_DOOR_SET_SOUNDS(sff_05_ENTRY_2, "obk_01", obk_01_ENTRY_0,
    COLLIDER_ttd, MODEL_doa, MODEL_o166, DOOR_SOUNDS_CREAKY);

EvtScript EVS_BindExitTriggers = {
    BindTrigger(Ref(EVS_ExitWalk_sff_03_3), TRIGGER_FLOOR_ABOVE, COLLIDER_deiliw, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_sff_06_0), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_obk_01_0), TRIGGER_WALL_PRESS_A, COLLIDER_ttd, 1, 0)
    Exec(EVS_ExitFall_sff_07_0)
    Return
    End
};

EvtScript EVS_EnterMap = {
    Set(AF_SFF_01, true)
    // Call(GetLoadType, LVar1)
    // IfEq(LVar1, LOAD_FROM_FILE_SELECT)
    //     Exec(EnterSavePoint)
    //     Exec(EVS_BindExitTriggers)
    //     Return
    // EndIf
    Call(GetEntryID, LVar0)
    Switch(LVar0)
        CaseEq(sff_05_ENTRY_0)
            Set(AF_SFF_01, false)
            // IfLt(GB_StoryProgress, STORY_CH3_ARRIVED_AT_BOOS_MANSION)
            //     Set(GB_StoryProgress, STORY_CH3_ARRIVED_AT_BOOS_MANSION)
            //     ExecWait(EVS_Scene_ReachedMansion)
            //     Call(SetCamSpeed, CAM_DEFAULT, Float(4.0))
            //     Call(GetPlayerPos, LVar0, LVar1, LVar2)
            //     Call(UseSettingsFrom, CAM_DEFAULT, LVar0, LVar1, LVar2)
            //     Call(SetPanTarget, CAM_DEFAULT, LVar0, LVar1, LVar2)
            //     Call(WaitForCam, CAM_DEFAULT, Float(1.0))
            //     Call(PanToTarget, CAM_DEFAULT, 0, false)
            //     Exec(EVS_BindExitTriggers)
            // Else
                Set(LVar0, Ref(EVS_BindExitTriggers))
                Exec(EnterWalk)
            // EndIf
        CaseEq(sff_05_ENTRY_1)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(sff_05_ENTRY_2)
            Call(UseDoorSounds, DOOR_SOUNDS_CREAKY)
            Set(LVar2, MODEL_doa)
            Set(LVar3, MODEL_o166)
            ExecWait(EnterDoubleDoor)
            Exec(EVS_BindExitTriggers)
        CaseEq(sff_05_ENTRY_3)
            IfEq(GF_MIM11_WarpPipe, false)
                Call(DisablePlayerInput, true)
                Call(DisablePlayerPhysics, true)
                Call(GetPlayerPos, LVar0, LVar1, LVar2)
                Call(SetNpcPos, NPC_PARTNER, LVar0, LVar1, LVar2)
                Call(SetPlayerPos, LVar0, NPC_DISPOSE_POS_Y, LVar2)
                Wait(30)
                Call(PlaySound, SOUND_GROW)
                Set(GF_MIM11_WarpPipe, true)
                Wait(30)
                Call(SetPlayerActionState, ACTION_STATE_IDLE)
                Call(SetPlayerPos, LVar0, LVar1, LVar2)
                Call(SetNpcPos, NPC_PARTNER, LVar0, LVar1, LVar2)
                Call(DisablePlayerPhysics, false)
                Call(DisablePlayerInput, false)
            EndIf
            Set(LVarA, Ref(EVS_BindExitTriggers))
            ExecWait(N(EVS_Pipe_EnterVertical))
        CaseEq(sff_05_ENTRY_4)
            Exec(EVS_BindExitTriggers)
        CaseEq(sff_05_ENTRY_5)
            Call(InterpPlayerYaw, 227, 0)
            Exec(EVS_BindExitTriggers)
        CaseEq(sff_05_ENTRY_6)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EVS_EnterJumping)
    EndSwitch
    Return
    End
};

EvtScript EVS_WindowColorShift = {
    Loop(0)
        Call(EnableModel, MODEL_RedWindows, true)
        Call(EnableModel, MODEL_GreenWindows, false)
        Call(EnableModel, MODEL_BlueWindows, false)
        Wait(32)
        Call(EnableModel, MODEL_RedWindows, false)
        Call(EnableModel, MODEL_GreenWindows, true)
        Call(EnableModel, MODEL_BlueWindows, false)
        Wait(32)
        Call(EnableModel, MODEL_RedWindows, false)
        Call(EnableModel, MODEL_GreenWindows, false)
        Call(EnableModel, MODEL_BlueWindows, true)
        Wait(32)
    EndLoop
    Return
    End
};

EvtScript EVS_Main ={
    Set(GB_WorldLocation, LOCATION_BOOS_MANSION)
    Set(GF_MAP_BoosMansion, true)
    Call(SetSpriteShading, SHADING_MIM_11)
    EVT_SETUP_CAMERA_NO_LEAD(0, 0, 0)
    Call(EnableGroup, MODEL_g62, false)
    Call(SetModelCustomGfx, MODEL_RedWindows, CUSTOM_GFX_NONE, ENV_TINT_REMAP)
    Call(SetModelCustomGfx, MODEL_GreenWindows, CUSTOM_GFX_NONE, ENV_TINT_REMAP)
    Call(SetModelCustomGfx, MODEL_BlueWindows, CUSTOM_GFX_NONE, ENV_TINT_REMAP)
    Exec(EVS_WindowColorShift)
    Call(SetFogAndBackgroundColor)
    Exec(EVS_ManageMansionSnowfall)
    // Call(MakeNpcs, true, Ref(DefaultNPCs))
    ExecWait(EVS_MakeEntities)
    // Exec(N(D_802430E0_BBA150))
    Exec(EVS_SetupFoliage)
    Exec(EVS_SetupMusic)
    Exec(EVS_EnterMap)
    Wait(1)
    Return
    End
};

EntryList Entrances = {
    { -635.0,    0.0,    5.0,   90.0 },
    {  635.0,    0.0,    5.0,  270.0 },
    {  117.0,   37.0, -201.0,  227.0 },
    { -270.0,   35.0, -225.0,  180.0 },
    {  -65.0,    0.0,  -22.0,  315.0 },
    {  -56.0,    0.0,  -25.0,  227.0 },
    {  GEN_ENTRY6_VEC, GEN_ENTRY6_DIR },
};

}; // namespace sff_05

MapSettings sff_05_settings = {
    .main = &sff_05::EVS_Main,
    .entryList = &sff_05::Entrances,
    .entryCount = ENTRY_COUNT(sff_05::Entrances),
    .background = &gBackgroundImage,
};
