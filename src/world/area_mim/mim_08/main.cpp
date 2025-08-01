#include "mim_08.h"

namespace mim_08 {

#include "../common/SetForeverForestFog.inc.cpp"

#include "../common/ManageSnowfall.inc.cpp"

EvtScript EVS_SetupMusic = {
    Call(SetMusicTrack, 0, SONG_FOREVER_FOREST, 0, 8)
    Call(PlayAmbientSounds, AMBIENT_SPOOKY)
    Return
    End
};

s32 KeyList[] = {
    ITEM_RUINS_KEY,
    ITEM_NONE
};

API_CALLABLE(GetPlayerDistFromMapCenter) {
    script->varTable[0] = get_xz_dist_to_player(0.0f, 0.0f);
    return ApiStatus_DONE2;
}

EvtScript EVS_ExitWalk_n = EVT_EXIT_WALK(60, mim_08_ENTRY_0, "mim_03", mim_03_ENTRY_1);
EvtScript EVS_ExitWalk_w = EVT_EXIT_WALK(60, mim_08_ENTRY_1, "mim_07", mim_07_ENTRY_2);
EvtScript EVS_ExitWalk_s = EVT_EXIT_WALK(60, mim_08_ENTRY_2, "mim_01", mim_01_ENTRY_0);
EvtScript EVS_ExitWalk_e = EVT_EXIT_WALK(60, mim_08_ENTRY_3, "mim_03", mim_03_ENTRY_0);

EvtScript EVS_BindExitTriggers = {
    BindTrigger(Ref(EVS_ExitWalk_n), TRIGGER_FLOOR_ABOVE, COLLIDER_deilin, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_w), TRIGGER_FLOOR_ABOVE, COLLIDER_deiliw, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_s), TRIGGER_FLOOR_ABOVE, COLLIDER_deilis, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_e), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    Return
    End
};

EvtScript EVS_EnterMap = {
    Call(GetEntryID, LVar0)
    Switch(LVar0)
        CaseEq(mim_08_ENTRY_0)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
            IfLt(GB_StoryProgress, STORY_MOD_CUTSCENE_4)
                // Call(NpcFaceNpc, NPC_DupiOaklie, NPC_DupiOaklie, 0)
                Call(SetNpcYaw, NPC_DupiOaklie, 180)
                Call(SetNpcPos, NPC_DupiOaklie, 55, 33, 27)
                Call(SetNpcPos, NPC_Oaklie, GEN_CHEST_OAKLIE_X, -10, GEN_CHEST_OAKLIE_Z)
                Call(AwaitPlayerApproach, GEN_CUTSCENE4_X, GEN_CUTSCENE4_Z, 60)
                Exec(EVS_Cutscene4)
                Set(GB_StoryProgress, STORY_MOD_CUTSCENE_4)
            EndIf
        CaseEq(mim_08_ENTRY_1)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
        CaseEq(mim_08_ENTRY_2)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
            IfLt(GB_StoryProgress, STORY_MOD_CUTSCENE_2)
                Exec(EVS_Cutscene2)
                Set(GB_StoryProgress, STORY_MOD_CUTSCENE_2)
            EndIf
        CaseEq(mim_08_ENTRY_3)
            Set(LVar0, Ref(EVS_BindExitTriggers))
            Exec(EnterWalk)
    EndSwitch
    Return
    End
};

EvtScript EVS_ShakeTree = {
    Loop(6)
        Set(LVar0, 2)
        Call(TranslateModel, MODEL_o507, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_o508, LVar0, 0, LVar0)
        Wait(1)
        Set(LVar0, -2)
        Call(TranslateModel, MODEL_o507, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_o508, LVar0, 0, LVar0)
        Wait(1)
    EndLoop
    Set(LVar0, 0)
    Call(TranslateModel, MODEL_o507, LVar0, 0, LVar0)
    Call(TranslateModel, MODEL_o508, LVar0, 0, LVar0)
    Wait(1)
    Set(MV_ShakedTree, true)
    Return
    End
};

EvtScript EVS_Main ={
    Set(GB_WorldLocation, LOCATION_FOREVER_FOREST)
    Call(SetSpriteShading, SHADING_MIM_08)
    EVT_SETUP_CAMERA_MIM()
    Set(AB_MIM_2, GB_MIM_CurrentMapID)
    Call(GetMapID, GB_MIM_CurrentMapID)
    ExecWait(EVS_SetupGates)
    Call(MakeNpcs, true, Ref(DefaultNPCs))
    BindPadlock(Ref(EVS_UnlockChest), TRIGGER_WALL_PRESS_A, EVT_ENTITY_INDEX(0), Ref(KeyList), 0, 1)
    ExecWait(EVS_MakeEntities)
    Set(LVar0, Ref(EVS_BindExitTriggers))
    Exec(EVS_EnterMap)
    Wait(1)
    Exec(EVS_SetupMusic)
    BindTrigger(Ref(EVS_ShakeTree), TRIGGER_WALL_HAMMER, COLLIDER_o473, 1, 0)
    Call(SetForeverForestFog)
    Exec(EVS_ManageSnowfall)
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
};

}; // namespace mim_08

MapSettings mim_08_settings = {
    .main = &mim_08::EVS_Main,
    .entryList = &mim_08::Entrances,
    .entryCount = ENTRY_COUNT(mim_08::Entrances),
    .background = &gBackgroundImage,
};
