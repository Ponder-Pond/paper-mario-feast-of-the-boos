#include "sff_07.h"

namespace sff_07 {

#include "../common/ManageSnowfall.inc.cpp"

#include "world/common/entity/Pipe.inc.c"

EvtScript EVS_SetupMusic = {
    Call(SetMusicTrack, 0, SONG_POP_DIVA_SONG, 0, 8)
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

EvtScript EVS_FocusCameraOnPlayer = {
    Label(0)
        Call(GetPlayerPos, LVar0, LVar1, LVar2)
        Call(SetCamTarget, CAM_DEFAULT, LVar0, LVar1, LVar2)
        Wait(1)
        Goto(0)
    Return
    End
};

EvtScript EVS_EnterJumping = {
    Call(DisablePlayerInput, true)
    Call(DisablePlayerPhysics, true)
    Call(SetPlayerActionState, ACTION_STATE_JUMP)
    Wait(1)
    Call(SetPlayerJumpscale, Float(1.0))
    Call(PlayerJump, 323, 0, -240, 20)
    Call(DisablePlayerPhysics, false)
    Call(DisablePlayerInput, false)
    Call(SetPlayerActionState, ACTION_STATE_IDLE)
    Return
    End
};

EvtScript EVS_ExitFall_mim_11_6 = {
    Loop(0)
        Wait(1)
        Call(GetPlayerPos, LVar0, LVar1, LVar2)
        IfLt(LVar1, -30)
            BreakLoop
        EndIf
    EndLoop
    Call(DisablePlayerInput, true)
    Call(DisablePlayerPhysics, true)
    Wait(30)
    Call(GotoMap, Ref("mim_11"), mim_11_ENTRY_6)
    Wait(100)
    Call(DisablePlayerInput, false)
    Call(DisablePlayerPhysics, false)
    Return
    End
};


EvtScript EVS_ExitWalk_Cellar = EVT_EXIT_WALK(60, mim_13_ENTRY_1, "mim_13", mim_13_ENTRY_1);

EvtScript EVS_BindExitTriggers = {
    Exec(EVS_ExitFall_mim_11_6)
    BindTrigger(Ref(EVS_ExitWalk_Cellar), TRIGGER_FLOOR_ABOVE, COLLIDER_CellarStairs, 1, 0)
    Return
    End
};

// EvtScript EVS_EnterMap = {
//     // Call(GetLoadType, LVar1)
//     // IfEq(LVar1, LOAD_FROM_FILE_SELECT)
//     //     Exec(EnterSavePoint)
//     //     Exec(EVS_BindExitTriggers)
//     //     Return
//     // EndIf
//     Set(LVar0, Ref(EVS_BindExitTriggers))
//     Exec(EnterWalk)
//     Return
//     End
// };

EvtScript EVS_EnterMap = {
    Call(GetEntryID, LVar0)
    Switch(LVar0)
        CaseEq(mim_13_ENTRY_0)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EVS_EnterJumping)
        CaseEq(mim_13_ENTRY_1)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
    EndSwitch
    Return
    End
};

EvtScript EVS_Main ={
    Set(GB_WorldLocation, LOCATION_BOOS_MANSION)
    Call(SetSpriteShading, SHADING_NONE)
    EVT_SETUP_CAMERA_DEFAULT(0, 0, 0)
    // Call(MakeNpcs, true, Ref(DefaultNPCs))
    Call(SetFogAndBackgroundColor)
    Exec(EVS_ManageMansionSnowfall)
    ExecWait(EVS_MakeEntities)
    Exec(EVS_SetupMusic)
    Exec(EVS_EnterMap)
    Wait(1)
    Return
    End
};

EntryList Entrances = {
    {  GEN_ENTRY0_VEC, GEN_ENTRY0_DIR },
    {  GEN_ENTRY1_VEC, GEN_ENTRY1_DIR },
};

}; // namespace sff_07

MapSettings mim_13_settings = {
    .main = &mim_13::EVS_Main,
    .entryList = &mim_13::Entrances,
    .entryCount = ENTRY_COUNT(mim_13::Entrances),
    .background = &gBackgroundImage,
};
