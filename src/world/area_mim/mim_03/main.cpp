#include "mim_03.h"

namespace mim_03 {

#include "../common/SetForeverForestFog.inc.cpp"

#include "../common/ManageSnowfall.inc.cpp"

#include "world/common/entity/Pipe.inc.c"

EvtScript EVS_SetupMusic = {
    Call(SetMusicTrack, 0, SONG_FOREVER_FOREST, 0, 8)
    Call(PlayAmbientSounds, AMBIENT_SPOOKY)
    Return
    End
};

API_CALLABLE(GetPlayerDistFromMapCenter) {
    script->varTable[0] = get_xz_dist_to_player(0.0f, 0.0f);
    return ApiStatus_DONE2;
}

EvtScript EVS_ExitWalk_n = EVT_EXIT_WALK(60, mim_03_ENTRY_0, "mim_08", mim_08_ENTRY_3);
EvtScript EVS_ExitWalk_w = EVT_EXIT_WALK(60, mim_03_ENTRY_1, "mim_08", mim_08_ENTRY_0);
EvtScript EVS_ExitWalk_s = EVT_EXIT_WALK(60, mim_03_ENTRY_2, "mim_08", mim_08_ENTRY_2);
EvtScript EVS_ExitWalk_e = EVT_EXIT_WALK(60, mim_03_ENTRY_3, "mim_01", mim_01_ENTRY_1);

EvtScript EVS_GotoMap_mim_01_5 = {
    Call(GotoMap, Ref("mim_01"), mim_01_ENTRY_5)
    Wait(100)
    Return
    End
};

EvtScript EVS_ExitPipe_mim_01_5 = EVT_EXIT_PIPE_VERTICAL(mim_03_ENTRY_4, COLLIDER_BluePipeTop, EVS_GotoMap_mim_01_5);

EvtScript EVS_BindExitTriggers = {
    BindTrigger(Ref(EVS_ExitWalk_n), TRIGGER_FLOOR_ABOVE, COLLIDER_deilin, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_w), TRIGGER_FLOOR_ABOVE, COLLIDER_deiliw, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_s), TRIGGER_FLOOR_ABOVE, COLLIDER_deilis, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_e), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    BindTrigger(Ref(EVS_ExitPipe_mim_01_5), TRIGGER_FLOOR_TOUCH, COLLIDER_BluePipeTop, 1, 0)
    Return
    End
};

EvtScript EVS_HideBluePipe = {
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_BluePipeBody, COLLIDER_FLAGS_UPPER_MASK)
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_BluePipeTop, COLLIDER_FLAGS_UPPER_MASK)
    Call(EnableModel, MODEL_BluePipe, false)
    Return
    End
};

EvtScript EVS_EnterMap = {
    Call(GetEntryID, LVar0)
    Switch(LVar0)
        CaseEq(mim_03_ENTRY_0)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
            IfLt(GB_StoryProgress, STORY_MOD_CUTSCENE_3)
                Call(SetNpcPos, NPC_DupiOaklie, 185, 0, -280)
                Call(AwaitPlayerApproach, 110, -315, 60)
                Exec(EVS_Cutscene3)
                Set(GB_StoryProgress, STORY_MOD_CUTSCENE_3)
            EndIf
        CaseEq(mim_03_ENTRY_1)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(mim_03_ENTRY_2)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(mim_03_ENTRY_3)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(mim_03_ENTRY_4)
            EVT_ENTER_PIPE_VERTICAL(EVS_BindExitTriggers)
        CaseEq(mim_03_ENTRY_5)
            Set(LVar0, Ref(EVS_BindExitTriggers))
    EndSwitch
    Return
    End
};

EvtScript EVS_Main ={
    Set(GB_WorldLocation, LOCATION_FOREVER_FOREST)
    Call(SetSpriteShading, SHADING_MIM_03)
    EVT_SETUP_CAMERA_MIM()
    Set(AB_MIM_2, GB_MIM_CurrentMapID)
    Call(GetMapID, GB_MIM_CurrentMapID)
    Set(AF_MIM03_Oaklie_DialogueToggle, false)
    ExecWait(EVS_SetupGates)
    Call(MakeNpcs, true, Ref(DefaultNPCs))
    Call(SetZoneEnabled, ZONE_stagecutscene, false)
    Call(SetZoneEnabled, ZONE_cutscene, false)
    ExecWait(EVS_MakeEntities)
    IfLt(GB_StoryProgress, STORY_MOD_CUTSCENE_7)
        Exec(EVS_HideBluePipe)
    EndIf
    Set(LVar0, Ref(EVS_BindExitTriggers))
    Exec(EVS_EnterMap)
    Wait(1)
    Exec(EVS_SetupMusic)
    Exec(EVS_ManageSnowfall)
    ExecWait(EVS_SetupExitHint)
    Call(SetForeverForestFog)
    Thread
        Label(0)
        Call(WaitForCam, CAM_DEFAULT, Float(1.0))
        Call(GetPlayerDistFromMapCenter)
        IfLt(LVar0, 350)
            Call(SetCamSpeed, CAM_DEFAULT, Float(1.0))
        Else
            Call(SetCamSpeed, CAM_DEFAULT, Float(3.0))
        EndIf
        Goto(0)
    EndThread
    Return
    End
};
EntryList Entrances = {
    {    0.0,    0.0, -530.0,  180.0 },
    { -530.0,    0.0,    0.0,   90.0 },
    {    0.0,    0.0,  530.0,    0.0 },
    {  530.0,    0.0,    0.0,  270.0 },
    {  -57, 66, 92,  GEN_BLUE_PIPE_DIR },
    {  -7, 31, -34,  GEN_CUTSCENE7_DIR },
};

}; // namespace mim_03

MapSettings mim_03_settings = {
    .main = &mim_03::EVS_Main,
    .entryList = &mim_03::Entrances,
    .entryCount = ENTRY_COUNT(mim_03::Entrances),
    .background = &gBackgroundImage,
};
