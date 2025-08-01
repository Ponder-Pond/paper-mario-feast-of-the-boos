#include "mim_10.h"

namespace mim_10 {

EvtScript EVS_SetToadTownMusic = {
    Switch(GB_StoryProgress)
        CaseRange(STORY_CH3_STAR_SPRIT_DEPARTED, STORY_CH4_STAR_SPIRIT_RESCUED)
            Call(SetMusicTrack, 0, SONG_SHY_GUY_INVASION, 0, 8)
        CaseDefault
            Call(SetMusicTrack, 0, SONG_TOAD_TOWN, 0, 8)
    EndSwitch
    Return
    End
};

EvtScript EVS_SetupMusic = {
    IfEq(GB_StoryProgress, STORY_CH3_SAW_BOO_ENTER_FOREST)
        Exec(EVS_SetToadTownMusic)
        Return
    EndIf
    Call(GetLoadType, LVar1)
    IfEq(LVar1, LOAD_FROM_FILE_SELECT)
        Exec(EVS_SetToadTownMusic)
        Return
    EndIf
    Call(GetPlayerPos, LVar1, LVar2, LVar3)
    IfLt(LVar1, 0)
        Exec(EVS_SetToadTownMusic)
        Wait(30)
    Else
        Call(SetMusicTrack, 0, SONG_FOREVER_FOREST, 0, 8)
        Wait(15)
        Call(ClearAmbientSounds, 250)
        Wait(15)
        Goto(20)
    EndIf
    Label(10)
    Wait(1)
    Call(GetPlayerPos, LVar1, LVar2, LVar3)
    IfGe(LVar1, 100)
        Call(FadeOutMusic, 0, 1000)
        Wait(15)
        Wait(15)
        Call(SetMusicTrack, 0, SONG_FOREVER_FOREST, 0, 8)
        Wait(30)
        Goto(20)
    EndIf
    Goto(10)
    Label(20)
    Wait(1)
    Call(GetPlayerPos, LVar1, LVar2, LVar3)
    IfLt(LVar1, 0)
        Call(FadeOutMusic, 0, 1000)
        Wait(30)
        Exec(EVS_SetToadTownMusic)
        Wait(30)
        Goto(10)
    EndIf
    Goto(20)
    Return
    End
};

EvtScript EVS_ExitWalk_mac_02_1 = EVT_EXIT_WALK(60, mim_10_ENTRY_0, "mac_02", mac_02_ENTRY_1);

EvtScript EVS_ExitWalk_mim_01_1 = {
    SetGroup(EVT_GROUP_EXIT_MAP)
    Call(UseExitHeading, 60, mim_10_ENTRY_1)
    Exec(ExitWalk)
    IfEq(GB_StoryProgress, STORY_CH3_INVITED_TO_BOOS_MANSION)
        IfEq(GF_StartedChapter3, false)
            Set(GF_StartedChapter3, true)
            Call(FadeOutMusic, 0, 1500)
            Call(GotoMapSpecial, Ref("kmr_22"), kmr_22_ENTRY_3, TRANSITION_BEGIN_OR_END_CHAPTER)
            Wait(100)
            Return
        EndIf
    EndIf
    Call(GotoMap, Ref("mim_01"), mim_01_ENTRY_1)
    Wait(100)
    Return
    End
};

EvtScript EVS_BindExitTriggers = {
    BindTrigger(Ref(EVS_ExitWalk_mac_02_1), TRIGGER_FLOOR_ABOVE, COLLIDER_deiliw, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_mim_01_1), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    Return
    End
};

EvtScript EVS_EnterMap = {
    Call(GetLoadType, LVar1)
    IfEq(LVar1, LOAD_FROM_FILE_SELECT)
        Exec(EnterSavePoint)
        Exec(EVS_BindExitTriggers)
        Return
    EndIf
    Set(LVar0, Ref(EVS_BindExitTriggers))
    Exec(EnterWalk)
    Return
    End
};

EvtScript EVS_Main ={
    Set(GB_WorldLocation, LOCATION_TOAD_TOWN)
    Call(SetSpriteShading, SHADING_NONE)
    EVT_SETUP_CAMERA_DEFAULT(0, 0, 0)
    Call(MakeNpcs, true, Ref(DefaultNPCs))
    Exec(EVS_SetupBootlerTrigger)
    ExecWait(EVS_MakeEntities)
    Exec(EVS_SetupMusic)
    Exec(EVS_EnterMap)
    Wait(1)
    Set(GF_MAC01_RowfBadgesChosen, false)
    Return
    End
};

EntryList Entrances = {
    { -385.0,   20.0,   10.0,   90.0 },
    {  385.0,    0.0,   10.0,  270.0 },
};

}; // namespace mim_10

MapSettings mim_10_settings = {
    .main = &mim_10::EVS_Main,
    .entryList = &mim_10::Entrances,
    .entryCount = ENTRY_COUNT(mim_10::Entrances),
    .background = &gBackgroundImage,
};
