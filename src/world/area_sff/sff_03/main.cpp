#include "sff_03.h"

namespace sff_03 {

#include "../common/SetForeverForestFog.inc.cpp"

#include "../common/ManageSnowfall.inc.cpp"

#include "world/common/entity/Pipe.inc.c"

EvtScript EVS_PlayForestMusic = {
    Call(SetMusicTrack, 0, SONG_FOREVER_FOREST, 0, 8)
    Call(PlayAmbientSounds, AMBIENT_SPOOKY)
    Return
    End
};

EvtScript EVS_JrTroopaMusic = {
    Call(SetMusicTrack, 0, SONG_JR_TROOPA_THEME, 0, 8)
    Return
    End
};

EvtScript EVS_ExitWalk_n = EVT_EXIT_WALK(60, sff_03_ENTRY_0, "sff_01", sff_01_ENTRY_3);
EvtScript EVS_ExitWalk_w = EVT_EXIT_WALK(60, sff_03_ENTRY_1, "sff_01", sff_01_ENTRY_2);
EvtScript EVS_ExitWalk_s = EVT_EXIT_WALK(60, sff_03_ENTRY_2, "sff_04", sff_04_ENTRY_2);
EvtScript EVS_ExitWalk_e = EVT_EXIT_WALK(60, sff_03_ENTRY_3, "sff_05", sff_05_ENTRY_0);

EvtScript EVS_GotoMap_sff_01_4 = {
    Call(GotoMap, Ref("mim_01"), mim_01_ENTRY_4)
    Wait(100)
    Return
    End
};

EvtScript EVS_ExitPipe_sff_01_4 = EVT_EXIT_PIPE_VERTICAL(sff_03_ENTRY_4, COLLIDER_GreenPipeTop, EVS_GotoMap_sff_01_4);


EvtScript EVS_BindExitTriggers = {
    BindTrigger(Ref(EVS_ExitWalk_n), TRIGGER_FLOOR_ABOVE, COLLIDER_deilin, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_w), TRIGGER_FLOOR_ABOVE, COLLIDER_deiliw, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_s), TRIGGER_FLOOR_ABOVE, COLLIDER_deilis, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_e), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    BindTrigger(Ref(EVS_ExitPipe_sff_01_4), TRIGGER_FLOOR_ABOVE, COLLIDER_GreenPipeTop, 1, 0)
    Return
    End
};

EvtScript EVS_EnterMap = {
    Call(GetEntryID, LVar0)
    Switch(LVar0)
        CaseEq(sff_03_ENTRY_0)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(sff_03_ENTRY_1)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(sff_03_ENTRY_2)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(sff_03_ENTRY_3)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(sff_03_ENTRY_4)
            EVT_ENTER_PIPE_VERTICAL(EVS_BindExitTriggers)
    EndSwitch
    Return
    End
};

EvtScript EVS_Main ={
    Set(GB_WorldLocation, LOCATION_FOREVER_FOREST)
    Call(SetSpriteShading, SHADING_MIM_07)
    EVT_SETUP_CAMERA_MIM()
    Set(AB_SFF_2, GB_MIM_CurrentMapID)
    Call(GetMapID, GB_MIM_CurrentMapID)
    ExecWait(EVS_SetupGates)
    Call(MakeNpcs, true, Ref(DefaultNPCs))
    ExecWait(EVS_MakeEntities)
    Set(LVar0, Ref(EVS_BindExitTriggers))
    Exec(EVS_EnterMap)
    Wait(1)
    Exec(EVS_PlayForestMusic)
    ExecWait(EVS_SetupExitHint)
    Call(SetForeverForestFog)
    Exec(EVS_ManageSnowfall)
    Call(GetEntryID, LVar0)
    IfEq(LVar0, sff_03_ENTRY_3)
        Call(UseSettingsFrom, CAM_DEFAULT, 500, -100, 0)
        Call(SetPanTarget, CAM_DEFAULT, 530, 0, 0)
        Call(SetCamSpeed, CAM_DEFAULT, Float(90.0))
        Call(PanToTarget, CAM_DEFAULT, 0, true)
        Wait(1)
        Thread
            Call(PanToTarget, CAM_DEFAULT, 0, false)
            Call(SetCamSpeed, CAM_DEFAULT, Float(3.0))
        EndThread
    Else
        Call(SetCamSpeed, CAM_DEFAULT, Float(3.0))
    EndIf
    Return
    End
};

EntryList Entrances = {
    {    0.0,    0.0, -530.0,  180.0 },
    { -530.0,    0.0,    0.0,   90.0 },
    {    0.0,    0.0,  530.0,    0.0 },
    {  530.0,    0.0,    0.0,  270.0 },
    {  169, 35, 285,  GEN_GREEN_PIPE_DIR },
};

}; // namespace sff_03

MapSettings sff_03_settings = {
    .main = &sff_03::EVS_Main,
    .entryList = &sff_03::Entrances,
    .entryCount = ENTRY_COUNT(sff_03::Entrances),
    .background = &gBackgroundImage,
};
