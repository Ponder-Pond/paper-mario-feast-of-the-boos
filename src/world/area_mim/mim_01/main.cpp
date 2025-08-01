#include "mim_01.h"

namespace mim_01 {

#include "../common/SetForeverForestFog.inc.cpp"

#include "../common/ManageSnowfall.inc.cpp"

#include "world/common/entity/Pipe.inc.c"

EvtScript EVS_SetupMusic = {
    Call(SetMusicTrack, 0, SONG_FOREVER_FOREST, 0, 8)
    Call(PlayAmbientSounds, AMBIENT_SPOOKY)
    Return
    End
};

EvtScript EVS_ExitWalk_n = EVT_EXIT_WALK(60, mim_01_ENTRY_0, "mim_08", mim_08_ENTRY_2);
EvtScript EVS_ExitWalk_w = EVT_EXIT_WALK(60, mim_01_ENTRY_1, "mim_10", mim_10_ENTRY_1);
EvtScript EVS_ExitWalk_s = EVT_EXIT_WALK(60, mim_01_ENTRY_2, "mim_07", mim_07_ENTRY_1);
EvtScript EVS_ExitWalk_e = EVT_EXIT_WALK(60, mim_01_ENTRY_3, "mim_07", mim_07_ENTRY_0);

EvtScript EVS_GotoMap_mim_07_4 = {
    Call(GotoMap, Ref("mim_07"), mim_07_ENTRY_4)
    Wait(100)
    Return
    End
};

EvtScript EVS_GotoMap_mim_03_4 = {
    Call(GotoMap, Ref("mim_03"), mim_03_ENTRY_4)
    Wait(100)
    Return
    End
};

EvtScript EVS_ExitPipe_mim_07_4 = EVT_EXIT_PIPE_VERTICAL(mim_01_ENTRY_4, COLLIDER_GreenPipeTop, EVS_GotoMap_mim_07_4);
EvtScript EVS_ExitPipe_mim_03_4 = EVT_EXIT_PIPE_VERTICAL(mim_01_ENTRY_5, COLLIDER_BluePipeTop, EVS_GotoMap_mim_03_4);

EvtScript EVS_BindExitTriggers = {
    BindTrigger(Ref(EVS_ExitWalk_n), TRIGGER_FLOOR_ABOVE, COLLIDER_deilin, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_w), TRIGGER_FLOOR_ABOVE, COLLIDER_deiliw, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_s), TRIGGER_FLOOR_ABOVE, COLLIDER_deilis, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_e), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    BindTrigger(Ref(EVS_ExitPipe_mim_07_4), TRIGGER_FLOOR_ABOVE, COLLIDER_GreenPipeTop, 1, 0)
    BindTrigger(Ref(EVS_ExitPipe_mim_03_4), TRIGGER_FLOOR_TOUCH, COLLIDER_BluePipeTop, 1, 0)
    Return
    End
};

EvtScript EVS_EnterMap = {
    Call(GetEntryID, LVar0)
    Switch(LVar0)
        CaseEq(mim_01_ENTRY_0)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(mim_01_ENTRY_1)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
            IfLt(GB_StoryProgress, STORY_MOD_CUTSCENE_1)
                Exec(EVS_Cutscene1)
                Set(GB_StoryProgress, STORY_MOD_CUTSCENE_1)
            EndIf
        CaseEq(mim_01_ENTRY_2)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(mim_01_ENTRY_3)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(mim_01_ENTRY_4)
            EVT_ENTER_PIPE_VERTICAL(EVS_BindExitTriggers)
        CaseEq(mim_01_ENTRY_5)
            EVT_ENTER_PIPE_VERTICAL(EVS_BindExitTriggers)
    EndSwitch
    Return
    End
};

EvtScript EVS_Main ={
    Set(GB_WorldLocation, LOCATION_FOREVER_FOREST)
    Call(SetSpriteShading, SHADING_MIM_01)
    EVT_SETUP_CAMERA_MIM()
    Set(GF_MAP_ForeverForest, true)
    Set(AB_MIM_2, GB_MIM_CurrentMapID)
    Call(GetMapID, GB_MIM_CurrentMapID)
    ExecWait(EVS_SetupGates)
    Call(MakeNpcs, true, Ref(DefaultNPCs))
    ExecWait(EVS_MakeEntities)
    ExecWait(EVS_SetupExitHint)
    Set(LVar0, Ref(EVS_BindExitTriggers))
    Exec(EVS_EnterMap)
    Wait(1)
    Call(SetForeverForestFog)
    Exec(EVS_SetupMusic)
    Exec(EVS_ManageSnowfall)
    Call(SetCamSpeed, CAM_DEFAULT, Float(3.0))
    Return
    End
};

EntryList Entrances = {
    {    0.0,    0.0, -530.0,  180.0 },
    { -530.0,    0.0,    0.0,   90.0 },
    {    0.0,    0.0,  530.0,    0.0 },
    {  530.0,    0.0,    0.0,  270.0 },
    {  144, 35, 314,  GEN_PIPE_GREEN_DIR },
    {  289, 35, -183,  GEN_PIPE_BLUE_DIR },
};

}; // namespace mim_01

MapSettings mim_01_settings = {
    .main = &mim_01::EVS_Main,
    .entryList = &mim_01::Entrances,
    .entryCount = ENTRY_COUNT(mim_01::Entrances),
    .background = &gBackgroundImage,
};
