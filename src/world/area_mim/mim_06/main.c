#include "mim_06.h"

#include "../common/SetForeverForestFog.inc.cpp"

EvtScript EVS_ExitWalk_n = EVT_EXIT_WALK(60, mim_06_ENTRY_0, "mim_07", mim_07_ENTRY_2);
EvtScript EVS_ExitWalk_w = EVT_EXIT_WALK(60, mim_06_ENTRY_1, "mim_01", mim_01_ENTRY_1);
EvtScript EVS_ExitWalk_s = EVT_EXIT_WALK(60, mim_06_ENTRY_2, "mim_01", mim_01_ENTRY_1);
EvtScript EVS_ExitWalk_e = EVT_EXIT_WALK(60, mim_06_ENTRY_3, "mim_05", mim_05_ENTRY_1);

EvtScript EVS_BindExitTriggers = {
    BindTrigger(Ref(EVS_ExitWalk_n), TRIGGER_FLOOR_ABOVE, COLLIDER_deilin, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_w), TRIGGER_FLOOR_ABOVE, COLLIDER_deiliw, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_s), TRIGGER_FLOOR_ABOVE, COLLIDER_deilis, 1, 0)
    BindTrigger(Ref(EVS_ExitWalk_e), TRIGGER_FLOOR_ABOVE, COLLIDER_deilie, 1, 0)
    Return
    End
};

EvtScript EVS_Main ={
    Set(GB_WorldLocation, LOCATION_FOREVER_FOREST)
    Call(SetSpriteShading, SHADING_MIM_06)
    EVT_SETUP_CAMERA_MIM()
    Set(AB_MIM_2, GB_MIM_CurrentMapID)
    Call(GetMapID, GB_MIM_CurrentMapID)
    ExecWait(EVS_SetupGates)
    Call(MakeNpcs, true, Ref(DefaultNPCs))
    Set(LVar0, Ref(EVS_BindExitTriggers))
    Exec(EnterWalk)
    Wait(1)
    Exec(EVS_SetupMusic)
    ExecWait(N(D_802439F8_BA8178))
    Call(SetForeverForestFog)
    Call(SetCamSpeed, CAM_DEFAULT, Float(3.0))
    Return
    End
};
