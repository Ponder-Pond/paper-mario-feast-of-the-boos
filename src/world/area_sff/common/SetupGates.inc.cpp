#include "common.h"

EvtScript EVS_SetupGates = {
    IfEq(GF_MIM_ChoosingPath, false)
        Set(GF_MIM_ChoosingPath, true)
        Set(AB_SFF_1, 2)
        Set(LVar1, 2)
        Set(LVar2, -10)
    Else
        Set(GF_MIM_ChoosingPath, false)
        Set(AB_SFF_1, 0)
        Set(LVar1, 0)
        Set(LVar2, 10)
    EndIf
    ExecWait(EVS_SetGateCameraZones)
    Exec(EVS_InitializeGates)
    Call(SetCamSpeed, CAM_DEFAULT, Float(90.0))
    Call(GetPlayerPos, LVar0, LVar1, LVar2)
    Call(UseSettingsFrom, CAM_DEFAULT, LVar0, LVar1, LVar2)
    Call(SetPanTarget, CAM_DEFAULT, LVar0, LVar1, LVar2)
    Call(PanToTarget, CAM_DEFAULT, 0, true)
    Wait(1)
    Call(PanToTarget, CAM_DEFAULT, 0, false)
    BindTrigger(Ref(EVS_UseGate_North), TRIGGER_WALL_PRESS_A, COLLIDER_monn, 1, 0)
    BindTrigger(Ref(EVS_UseGate_South), TRIGGER_WALL_PRESS_A, COLLIDER_mons, 1, 0)
    BindTrigger(Ref(EVS_UseGate_West),  TRIGGER_WALL_PRESS_A, COLLIDER_monw, 1, 0)
    BindTrigger(Ref(EVS_UseGate_East),  TRIGGER_WALL_PRESS_A, COLLIDER_mone, 1, 0)
    Return
    End
};
