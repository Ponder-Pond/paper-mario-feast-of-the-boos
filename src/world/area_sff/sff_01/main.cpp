#include "sff_01.h"

namespace sff_01 {

#include "../common/SetForeverForestFog.inc.cpp"

#include "../common/ManageSnowfall.inc.cpp"

#include "world/common/entity/Pipe.inc.c"

EvtScript EVS_SetupMusic = {
    Call(SetMusicTrack, 0, SONG_FOREVER_FOREST, 0, 8)
    Call(PlayAmbientSounds, AMBIENT_SPOOKY)
    Return
    End
};

EvtScript EVS_ExitWalk_n = EVT_EXIT_WALK(60, sff_01_ENTRY_0, "sff_04", sff_04_ENTRY_2);
EvtScript EVS_ExitWalk_w = EVT_EXIT_WALK(60, sff_01_ENTRY_1, "sff_00", sff_00_ENTRY_1);
EvtScript EVS_ExitWalk_s = EVT_EXIT_WALK(60, sff_01_ENTRY_2, "sff_03", sff_03_ENTRY_1);
EvtScript EVS_ExitWalk_e = EVT_EXIT_WALK(60, sff_01_ENTRY_3, "sff_03", sff_03_ENTRY_0);

EvtScript EVS_GotoMap_sff_03_4 = {
    Call(GotoMap, Ref("sff_03"), sff_03_ENTRY_4)
    Wait(100)
    Return
    End
};

EvtScript EVS_GotoMap_sff_02_4 = {
    Call(GotoMap, Ref("sff_02"), sff_02_ENTRY_4)
    Wait(100)
    Return
    End
};

EvtScript EVS_ExitPipe_sff_03_4 = EVT_EXIT_PIPE_VERTICAL(sff_01_ENTRY_4, COLLIDER_GreenPipeTop, EVS_GotoMap_sff_03_4);
EvtScript EVS_ExitPipe_sff_02_4 = EVT_EXIT_PIPE_VERTICAL(sff_01_ENTRY_5, COLLIDER_BluePipeTop, EVS_GotoMap_sff_02_4);

EvtScript EVS_BindExitTriggers = {
    BindTrigger(Ref(EVS_ExitWalk_n), TRIGGER_FLOOR_ABOVE, COLLIDER_deilin, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_w), TRIGGER_FLOOR_ABOVE, COLLIDER_deiliw, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_s), TRIGGER_FLOOR_ABOVE, COLLIDER_deilis, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_e), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    BindTrigger(Ref(EVS_ExitPipe_sff_03_4), TRIGGER_FLOOR_ABOVE, COLLIDER_GreenPipeTop, 1, 0)
    BindTrigger(Ref(EVS_ExitPipe_sff_02_4), TRIGGER_FLOOR_TOUCH, COLLIDER_BluePipeTop, 1, 0)
    Return
    End
};

EvtScript EVS_EnterMap = {
    Call(GetEntryID, LVar0)
    Switch(LVar0)
        CaseEq(sff_01_ENTRY_0)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(sff_01_ENTRY_1)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
            IfLt(GB_StoryProgress, STORY_MOD_CUTSCENE_1)
                Exec(EVS_Cutscene1)
                Set(GB_StoryProgress, STORY_MOD_CUTSCENE_1)
            EndIf
        CaseEq(sff_01_ENTRY_2)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(sff_01_ENTRY_3)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(sff_01_ENTRY_4)
            EVT_ENTER_PIPE_VERTICAL(EVS_BindExitTriggers)
        CaseEq(sff_01_ENTRY_5)
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
    Set(AB_SFF_2, GB_MIM_CurrentMapID)
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

}; // namespace sff_01

MapSettings sff_01_settings = {
    .main = &sff_01::EVS_Main,
    .entryList = &sff_01::Entrances,
    .entryCount = ENTRY_COUNT(sff_01::Entrances),
    .background = &gBackgroundImage,
};
