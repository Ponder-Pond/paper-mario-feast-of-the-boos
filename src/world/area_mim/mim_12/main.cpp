#include "mim_12.h"

namespace mim_12 {

EvtScript EVS_StartForestMusic = {
    Switch(GB_StoryProgress)
        CaseLt(STORY_CH3_ARRIVED_AT_BOOS_MANSION)
            Call(SetMusicTrack, 0, SONG_BOOS_MANSION, 1, 8)
        CaseDefault
            Call(SetMusicTrack, 0, SONG_FOREVER_FOREST, 0, 8)
    EndSwitch
    Call(ClearAmbientSounds, 250)
    Return
    End
};

EvtScript EVS_StartGustyMusic = {
    Switch(GB_StoryProgress)
        CaseLt(STORY_CH3_TUBBA_WOKE_UP)
            Call(SetMusicTrack, 0, SONG_GUSTY_GULCH, 0, 8)
        CaseLt(STORY_CH3_DEFEATED_TUBBA_BLUBBA)
            Call(SetMusicTrack, 0, SONG_TUBBA_ESCAPE, 0, 8)
        CaseDefault
            Call(SetMusicTrack, 0, SONG_GUSTY_GULCH, 0, 8)
    EndSwitch
    Call(PlayAmbientSounds, AMBIENT_WIND)
    Return
    End
};

EvtScript EVS_SetupMusic = {
    Call(GetEntryID, LVar0)
    IfEq(LVar0, mim_12_ENTRY_0)
        ExecWait(EVS_StartForestMusic)
    Else
        ExecWait(EVS_StartGustyMusic)
    EndIf
    Return
    End
};


EvtScript EVS_ExitWalk_mim_11_1 = EVT_EXIT_WALK(60, mim_12_ENTRY_0, "mim_11", mim_11_ENTRY_1);
EvtScript EVS_ExitWalk_arn_07_2 = EVT_EXIT_WALK(60, mim_12_ENTRY_1, "arn_07", arn_07_ENTRY_2);

EvtScript EVS_BindExitTriggers = {
    BindTrigger(Ref(EVS_ExitWalk_mim_11_1), TRIGGER_FLOOR_ABOVE, COLLIDER_deiliw, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_arn_07_2), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    Return
    End
};

EvtScript EVS_EnterMap = {
    Set(LVar0, Ref(EVS_BindExitTriggers))
    Exec(EnterWalk)
    Call(SetNpcPos, NPC_GateBoo_01, NPC_DISPOSE_LOCATION)
    Call(SetNpcPos, NPC_GateBoo_02, NPC_DISPOSE_LOCATION)
    SetF(LVar0, Float(240.0))
    Loop(20)
        SubF(LVar0, Float(12.0))
        Call(SetNpcImgFXParams, NPC_GateBoo_01, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        Call(SetNpcImgFXParams, NPC_GateBoo_02, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        Wait(1)
    EndLoop
    Wait(10)
    Return
    End
};

EvtScript EVS_Main ={
    Set(GB_WorldLocation, LOCATION_FOREVER_FOREST)
    Call(SetSpriteShading, SHADING_MIM_12)
    Call(SetCamPerspective, CAM_DEFAULT, CAM_UPDATE_FROM_ZONE, 25, 16, 1024) // far clip distance closer than normal, further than MIM
    Call(SetCamBGColor, CAM_DEFAULT, 0, 0, 0)
    Call(SetCamEnabled, CAM_DEFAULT, true)
    Call(MakeNpcs, true, Ref(DefaultNPCs))
    ExecWait(EVS_MakeEntities)
    Exec(EVS_SetupMusic)
    Exec(EVS_SetupForestGate)
    Exec(EVS_EnterMap)
    Wait(1)
    Return
    End
};

EntryList Entrances = {
    { -380.0,    0.0,   10.0,   90.0 },
    {  380.0,    0.0,   10.0,  270.0 },
};

}; // namespace mim_12

MapSettings mim_12_settings = {
    .main = &mim_12::EVS_Main,
    .entryList = &mim_12::Entrances,
    .entryCount = ENTRY_COUNT(mim_12::Entrances),
    .background = &gBackgroundImage,
    .tattle = { MSG_MapTattle_mim_12 },
};
