#include "sff_01.h"

namespace sff_01 {

EvtScript EVS_SetGateCameraZones = {
    Switch(LVar1)
        CaseEq(0)
            Call(SetZoneEnabled, ZONE_north, true)
            Call(SetZoneEnabled, ZONE_west, true)
            Call(SetZoneEnabled, ZONE_south, true)
            Call(SetZoneEnabled, ZONE_east, true)
            Call(SetZoneEnabled, ZONE_higashi, false)
            Call(SetZoneEnabled, ZONE_nishi, false)
            Call(SetZoneEnabled, ZONE_minami, false)
            Call(SetZoneEnabled, ZONE_kita, false)
            Call(SetZoneEnabled, ZONE_stage, false)
            Set(LVar1, 0)
        CaseEq(1)
            Call(SetZoneEnabled, ZONE_north, false)
            Call(SetZoneEnabled, ZONE_west, false)
            Call(SetZoneEnabled, ZONE_south, false)
            Call(SetZoneEnabled, ZONE_east, false)
            Call(SetZoneEnabled, ZONE_higashi, false)
            Call(SetZoneEnabled, ZONE_nishi, false)
            Call(SetZoneEnabled, ZONE_minami, false)
            Call(SetZoneEnabled, ZONE_kita, false)
            Call(SetZoneEnabled, ZONE_stage, true)
            Set(LVar1, 1)
        CaseEq(2)
            Call(SetZoneEnabled, ZONE_north, false)
            Call(SetZoneEnabled, ZONE_west, true)
            Call(SetZoneEnabled, ZONE_south, false)
            Call(SetZoneEnabled, ZONE_east, false)
            Call(SetZoneEnabled, ZONE_higashi, true)
            Call(SetZoneEnabled, ZONE_nishi, false)
            Call(SetZoneEnabled, ZONE_minami, true)
            Call(SetZoneEnabled, ZONE_kita, true)
            Call(SetZoneEnabled, ZONE_stage, false)
            Set(LVar1, 2)
    EndSwitch
    Return
    End
};

EvtScript EVS_InitializeGates = {
    Call(ParentColliderToModel, COLLIDER_monn, MODEL_n1)
    Call(RotateGroup, MODEL_monn, LVar2, 0, 1, 0)
    Call(UpdateColliderTransform, COLLIDER_monn)
    Call(ParentColliderToModel, COLLIDER_mons, MODEL_s1)
    Call(RotateGroup, MODEL_mons, LVar2, 0, 1, 0)
    Call(UpdateColliderTransform, COLLIDER_mons)
    Call(ParentColliderToModel, COLLIDER_mone, MODEL_e1)
    Call(RotateGroup, MODEL_mone, LVar2, 0, 1, 0)
    Call(UpdateColliderTransform, COLLIDER_mone)
    IfLt(GB_StoryProgress, STORY_MOD_CUTSCENE_1)
        Call(ParentColliderToModel, COLLIDER_monw, MODEL_w1)
        Call(RotateGroup, MODEL_monw, 10, 0, 1, 0)
        Call(UpdateColliderTransform, COLLIDER_monw)
    Else
        Call(ParentColliderToModel, COLLIDER_monw2, MODEL_w1)
        Call(RotateGroup, MODEL_monw, 10, 0, 1, 0)
        Call(UpdateColliderTransform, COLLIDER_monw2)
    EndIf
    Return
    End
};

EvtScript EVS_ResetGates = {
    Call(RotateGroup, MODEL_monn, 0, 0, 1, 0)
    Call(UpdateColliderTransform, COLLIDER_monn)
    Call(RotateGroup, MODEL_mons, 0, 0, 1, 0)
    Call(UpdateColliderTransform, COLLIDER_mons)
    Call(RotateGroup, MODEL_mone, 0, 0, 1, 0)
    Call(UpdateColliderTransform, COLLIDER_mone)
    IfLt(GB_StoryProgress, STORY_MOD_CUTSCENE_1)
        Call(RotateGroup, MODEL_monw, 0, 0, 1, 0)
        Call(UpdateColliderTransform, COLLIDER_monw)
    Else
        Call(RotateGroup, MODEL_monw, 0, 0, 1, 0)
        Call(UpdateColliderTransform, COLLIDER_monw2)
    EndIf
    Return
    End
};

EvtScript EVS_PlayGateSounds = {
    Call(PlaySoundAtCollider, LVar9, SOUND_FOREST_GATE_OPEN, SOUND_SPACE_DEFAULT)
    Wait(19)
    Call(PlaySoundAtCollider, LVar9, SOUND_FOREST_GATE_CLOSE, SOUND_SPACE_DEFAULT)
    Return
    End
};

EvtScript EVS_UseGate = {
    Exec(EVS_PlayGateSounds)
    Switch(AB_SFF_1)
        CaseEq(0)
            Thread
                Call(MakeLerp, 10, 90, 10, EASING_QUADRATIC_IN)
                Label(10)
                Call(UpdateLerp)
                Call(RotateGroup, LVarA, LVar0, 0, 1, 0)
                Call(UpdateColliderTransform, LVar9)
                Wait(1)
                IfEq(LVar1, 1)
                    Goto(10)
                EndIf
            EndThread
            Wait(15)
            Thread
                Call(GetPlayerPos, LVar0, LVar1, LVar2)
                Add(LVar0, LVar7)
                Add(LVar2, LVar8)
                Call(PlayerMoveTo, LVar0, LVar2, 15)
            EndThread
            Set(LVar1, 1)
            Exec(EVS_SetGateCameraZones)
            Call(MakeLerp, 90, 360, 30, EASING_LINEAR)
            Label(20)
            Call(UpdateLerp)
            Call(RotateGroup, LVarA, LVar0, 0, 1, 0)
            Call(UpdateColliderTransform, LVar9)
            Wait(1)
            IfEq(LVar1, 1)
                Goto(20)
            EndIf
            Exec(EVS_ResetGates)
            Set(AB_SFF_1, 1)
            Set(GF_MIM_ChoosingPath, true)
        CaseEq(1)
            Thread
                Call(MakeLerp, 360, 270, 10, EASING_QUADRATIC_IN)
                Label(30)
                Call(UpdateLerp)
                Call(RotateGroup, LVarA, LVar0, 0, 1, 0)
                Call(UpdateColliderTransform, LVar9)
                Wait(1)
                IfEq(LVar1, 1)
                    Goto(30)
                EndIf
            EndThread
            Wait(15)
            Thread
                Call(GetPlayerPos, LVar0, LVar1, LVar2)
                Sub(LVar0, LVar7)
                Sub(LVar2, LVar8)
                Call(PlayerMoveTo, LVar0, LVar2, 15)
            EndThread
            Set(LVar1, 2)
            Exec(EVS_SetGateCameraZones)
            Call(MakeLerp, 270, -10, 30, EASING_LINEAR)
            Label(40)
            Call(UpdateLerp)
            Call(RotateGroup, LVarA, LVar0, 0, 1, 0)
            Call(UpdateColliderTransform, LVar9)
            Wait(1)
            IfEq(LVar1, 1)
                Goto(40)
            EndIf
            Set(AB_SFF_1, 2)
        CaseEq(2)
            Thread
                Call(MakeLerp, -10, 90, 10, EASING_QUADRATIC_IN)
                Label(50)
                Call(UpdateLerp)
                Call(RotateGroup, LVarA, LVar0, 0, 1, 0)
                Call(UpdateColliderTransform, LVar9)
                Wait(1)
                IfEq(LVar1, 1)
                    Goto(50)
                EndIf
            EndThread
            Wait(15)
            Thread
                Call(GetPlayerPos, LVar0, LVar1, LVar2)
                Add(LVar0, LVar7)
                Add(LVar2, LVar8)
                Call(PlayerMoveTo, LVar0, LVar2, 15)
            EndThread
            Set(LVar1, 1)
            Exec(EVS_SetGateCameraZones)
            Call(MakeLerp, 90, 360, 30, EASING_LINEAR)
            Label(60)
            Call(UpdateLerp)
            Call(RotateGroup, LVarA, LVar0, 0, 1, 0)
            Call(UpdateColliderTransform, LVar9)
            Wait(1)
            IfEq(LVar1, 1)
                Goto(60)
            EndIf
            Exec(EVS_ResetGates)
            Set(AB_SFF_1, 1)
    EndSwitch
    Return
    End
};

EvtScript EVS_UseGate_North = {
    Call(DisablePlayerInput, true)
    Set(LVar7, 0)
    Set(LVar8, 70)
    Set(LVar9, 26)
    Set(LVarA, 155)
    ExecWait(EVS_UseGate)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_UseGate_South = {
    Call(DisablePlayerInput, true)
    Set(LVar7, 0)
    Set(LVar8, -70)
    Set(LVar9, 30)
    Set(LVarA, 161)
    ExecWait(EVS_UseGate)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_UseGate_East = {
    Call(DisablePlayerInput, true)
    Set(LVar7, -70)
    Set(LVar8, 0)
    Set(LVar9, 28)
    Set(LVarA, 158)
    ExecWait(EVS_UseGate)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_UseGate_West = {
    Set(LVar9, 32)
    Exec(EVS_PlayGateSounds)
    Call(DisablePlayerInput, true)
    IfNe(AB_SFF_1, 1)
        Thread
            Call(MakeLerp, 10, 90, 10, EASING_QUADRATIC_IN)
            Label(10)
            Call(UpdateLerp)
            Call(RotateGroup, MODEL_monw, LVar0, 0, 1, 0)
            Call(UpdateColliderTransform, COLLIDER_monw)
            Wait(1)
            IfEq(LVar1, 1)
                Goto(10)
            EndIf
        EndThread
        Wait(15)
        Thread
            Call(GetPlayerPos, LVar0, LVar1, LVar2)
            Add(LVar0, 70)
            Call(PlayerMoveTo, LVar0, LVar2, 15)
        EndThread
        Set(LVar1, 1)
        Exec(EVS_SetGateCameraZones)
        Call(MakeLerp, 90, 360, 30, EASING_LINEAR)
        Label(20)
        Call(UpdateLerp)
        Call(RotateGroup, MODEL_monw, LVar0, 0, 1, 0)
        Call(UpdateColliderTransform, COLLIDER_monw)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(20)
        EndIf
        Exec(EVS_ResetGates)
        Set(AB_SFF_1, 1)
        Set(GF_MIM_ChoosingPath, true)
    Else
        Thread
            Call(MakeLerp, 360, 270, 10, EASING_QUADRATIC_IN)
            Label(30)
            Call(UpdateLerp)
            Call(RotateGroup, MODEL_monw, LVar0, 0, 1, 0)
            Call(UpdateColliderTransform, COLLIDER_monw)
            Wait(1)
            IfEq(LVar1, 1)
                Goto(30)
            EndIf
        EndThread
        Wait(15)
        Thread
            Call(GetPlayerPos, LVar0, LVar1, LVar2)
            Sub(LVar0, 70)
            Call(PlayerMoveTo, LVar0, LVar2, 15)
        EndThread
        Set(LVar1, 2)
        Exec(EVS_SetGateCameraZones)
        Call(MakeLerp, 270, 10, 30, EASING_LINEAR)
        Label(40)
        Call(UpdateLerp)
        Call(RotateGroup, MODEL_monw, LVar0, 0, 1, 0)
        Call(UpdateColliderTransform, COLLIDER_monw)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(40)
        EndIf
        Set(AB_SFF_1, 0)
    EndIf
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_DisabledGate_West = {
    Call(DisablePlayerInput, true)
    Call(ShowMessageAtScreenPos, MSG_FotB_0003, 160, 40)
    Call(DisablePlayerInput, false)
    Return
    End
};

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
    IfLt(GB_StoryProgress, STORY_MOD_CUTSCENE_1)
        BindTrigger(Ref(EVS_UseGate_West),  TRIGGER_WALL_PRESS_A, COLLIDER_monw, 1, 0)
    Else
        BindTrigger(Ref(EVS_DisabledGate_West),  TRIGGER_WALL_PRESS_A, COLLIDER_monw2, 1, 0)
    EndIf
    BindTrigger(Ref(EVS_UseGate_East),  TRIGGER_WALL_PRESS_A, COLLIDER_mone, 1, 0)
    Return
    End
};

}; // namespace sff_01
