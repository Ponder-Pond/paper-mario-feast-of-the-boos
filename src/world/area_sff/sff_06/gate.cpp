#include "sff_06.h"
#include "sprite/player.h"

namespace sff_06 {

API_CALLABLE(InitializeGustyFogProperties) {
    enable_world_fog();
    set_world_fog_dist(990, 1000);
    set_world_fog_color(0, 8, 24, 255);
    gCameras[CAM_DEFAULT].bgColor[0] = 24;
    gCameras[CAM_DEFAULT].bgColor[1] = 24;
    gCameras[CAM_DEFAULT].bgColor[2] = 24;
    gGameStatusPtr->backgroundDarkness = 0;
    return ApiStatus_DONE2;
}

API_CALLABLE(InitializeForestFogProperties) {
    enable_world_fog();
    set_world_fog_dist(980, 1000);
    set_world_fog_color(0, 8, 24, 255);
    gCameras[CAM_DEFAULT].bgColor[0] = 24;
    gCameras[CAM_DEFAULT].bgColor[1] = 24;
    gCameras[CAM_DEFAULT].bgColor[2] = 24;
    gGameStatusPtr->backgroundDarkness = 232;
    return ApiStatus_DONE2;
}

API_CALLABLE(InterpFogDistance) {
    Bytecode* args = script->ptrReadPos;
    s32 retVal = ApiStatus_BLOCK;

    if (isInitialCall) {
        script->functionTemp[1] = evt_get_variable(script, *args++);
        script->functionTemp[2] = evt_get_variable(script, *args++);
        script->functionTemp[0] = evt_get_variable(script, *args++);
        script->functionTemp[3] = evt_get_variable(script, *args++);
    }

    script->functionTemp[1] += script->functionTemp[3];

    if (script->functionTemp[3] >= 0) {
        if (script->functionTemp[1] >= script->functionTemp[2]) {
            script->functionTemp[1] = script->functionTemp[2];
            retVal = ApiStatus_DONE2;
        }
    } else {
        if (script->functionTemp[2] >= script->functionTemp[1] ) {
            script->functionTemp[1] = script->functionTemp[2];
            retVal = ApiStatus_DONE2;
        }
    }

    set_world_fog_dist(script->functionTemp[1], script->functionTemp[0]);
    return retVal;
}

API_CALLABLE(InterpBackgroundDarkness) {
    Bytecode* args = script->ptrReadPos;
    s32 retVal = ApiStatus_BLOCK;

    if (isInitialCall) {
        script->functionTemp[1] = evt_get_variable(script, *args++);
        script->functionTemp[2] = evt_get_variable(script, *args++);
        script->functionTemp[3] = evt_get_variable(script, *args++);
    }

    script->functionTemp[1] += script->functionTemp[3];

    if (script->functionTemp[3] >= 0) {
        if (script->functionTemp[1] >= script->functionTemp[2]) {
            script->functionTemp[1] = script->functionTemp[2];
            retVal = ApiStatus_DONE2;
        }
    } else {
        if (script->functionTemp[2] >= script->functionTemp[1]) {
            script->functionTemp[1] = script->functionTemp[2];
            retVal = ApiStatus_DONE2;
        }
    }

    gGameStatusPtr->backgroundDarkness = script->functionTemp[1];
    return retVal;
}

API_CALLABLE(InterpSpriteShadingColor) {
    SpriteShadingProfile* shadingProfile = gSpriteShadingProfile;
    Bytecode* args = script->ptrReadPos;

    if (isInitialCall) {
        script->varTable[0] = evt_get_variable(script, *args++);
        script->varTable[1] = evt_get_variable(script, *args++);
        script->varTable[2] = evt_get_variable(script, *args++);
        script->varTable[3] = shadingProfile->ambientColor.r;
        script->varTable[4] = shadingProfile->ambientColor.g;
        script->varTable[5] = shadingProfile->ambientColor.b;
        script->varTable[6] = 0;
        gSpriteShadingProfile->flags |= SPR_SHADING_FLAG_ENABLED;
    }

    script->varTable[6]++;
    shadingProfile->ambientColor.r = update_lerp(EASING_LINEAR, script->varTable[3], script->varTable[0], script->varTable[6], 20);
    shadingProfile->ambientColor.g = update_lerp(EASING_LINEAR, script->varTable[4], script->varTable[1], script->varTable[6], 20);
    shadingProfile->ambientColor.b = update_lerp(EASING_LINEAR, script->varTable[5], script->varTable[2], script->varTable[6], 20);

    if (script->varTable[6] == 20) {
        if (script->varTable[0] == 255) {
            gSpriteShadingProfile->flags &= ~SPR_SHADING_FLAG_ENABLED;
        }
        return ApiStatus_DONE2;
    }
    return ApiStatus_BLOCK;
}

API_CALLABLE(MoveNpcAlongArc) {
    Npc* npc = resolve_npc(script, script->varTable[0]);
    f32 angle, sinAngle, cosAngle;

    if (isInitialCall) {
        script->varTable[6] = script->varTable[5];
        script->varTable[7] = dist2D(npc->pos.x, npc->pos.z, script->varTable[1], script->varTable[2]);
    }

    if (script->varTable[6] > 0) {
        angle = update_lerp(EASING_LINEAR, script->varTable[3], script->varTable[4], (f32) script->varTable[6], (f32) script->varTable[5]);
        sin_cos_deg(angle, &sinAngle, &cosAngle);
        npc->pos.x = script->varTable[1] + (script->varTable[7] * sinAngle);
        npc->pos.z = script->varTable[2] + (script->varTable[7] * cosAngle);
        script->varTable[6]--;
        return ApiStatus_BLOCK;
    } else {
        return ApiStatus_DONE2;
    }
}

EvtScript EVS_FadeToForestShading = {
    Call(InitializeGustyFogProperties)
    Thread
        Call(InterpFogDistance, 990, 980, 1000, -1)
    EndThread
    Thread
        Call(InterpBackgroundDarkness, 0, 232, 20)
    EndThread
    Thread
        Call(InterpSpriteShadingColor, 160, 160, 255)
    EndThread
    Return
    End
};

EvtScript EVS_FadeToGustyShading = {
    Call(InitializeGustyFogProperties)
    Thread
        Call(InterpFogDistance, 980, 990, 1000, 1)
    EndThread
    Thread
        Call(InterpBackgroundDarkness, 232, 0, -20)
    EndThread
    Thread
        Call(InterpSpriteShadingColor, 255, 255, 255)
    EndThread
    Return
    End
};

EvtScript EVS_OpenGate = {
    Call(PlaySoundAtCollider, COLLIDER_o67, SOUND_METAL_GATE_OPEN, SOUND_SPACE_DEFAULT)
    Set(LVar3, 0)
    Sub(LVar3, LVar2)
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_o67, COLLIDER_FLAGS_UPPER_MASK)
    Call(MakeLerp, 0, 80, 30, EASING_LINEAR)
    Label(10)
    Call(UpdateLerp)
    Call(RotateModel, MODEL_o68, LVar0, 0, LVar2, 0)
    Call(RotateModel, MODEL_o67, LVar0, 0, LVar3, 0)
    Wait(1)
    IfNe(LVar1, 0)
        Goto(10)
    EndIf
    Return
    End
};

EvtScript EVS_CloseGate = {
    Set(LVar3, 0)
    Sub(LVar3, LVar2)
    Call(MakeLerp, 80, 0, 30, EASING_LINEAR)
    Label(10)
    Call(UpdateLerp)
    Call(RotateModel, MODEL_o68, LVar0, 0, LVar2, 0)
    Call(RotateModel, MODEL_o67, LVar0, 0, LVar3, 0)
    Wait(1)
    IfNe(LVar1, 0)
        Goto(10)
    EndIf
    Call(PlaySoundAtCollider, COLLIDER_o67, SOUND_METAL_GATE_CLOSE, SOUND_SPACE_DEFAULT)
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_CLEAR_BITS, COLLIDER_o67, COLLIDER_FLAGS_UPPER_MASK)
    Return
    End
};

EvtScript EVS_EnterForest = {
    Exec(EVS_FadeToForestShading)
    Set(LVar2, -1)
    Exec(EVS_OpenGate)
    Wait(20)
    Call(SetPlayerSpeed, Float(3.0))
    Call(PlayerMoveTo, -185, 0, 0)
    Set(LVar2, -1)
    Exec(EVS_CloseGate)
    ExecWait(EVS_StartForestMusic)
    Return
    End
};

EvtScript EVS_LeaveForest = {
    Exec(EVS_FadeToGustyShading)
    Set(LVar2, 1)
    Exec(EVS_OpenGate)
    Wait(20)
    Call(SetPlayerSpeed, Float(3.0))
    Call(PlayerMoveTo, -20, 0, 0)
    Set(LVar2, 1)
    Exec(EVS_CloseGate)
    ExecWait(EVS_StartGustyMusic)
    Return
    End
};

EvtScript EVS_CantOpenGateMessage = {
    Call(DisablePlayerInput, true)
    Call(ShowMessageAtScreenPos, MSG_Menus_00D9, 160, 40)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_UseGate = {
    Call(DisablePlayerInput, true)
    Call(InterruptUsePartner)
    IfEq(AF_SFF06_BackgroundLitUp, false)
        ExecWait(EVS_LeaveForest)
        Set(AF_SFF06_BackgroundLitUp, true)
    Else
        ExecWait(EVS_EnterForest)
        Set(AF_SFF06_BackgroundLitUp, false)
    EndIf
    Wait(20)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_FlyDownToGate = {
    Thread
        Call(SetNpcJumpscale, NPC_GateBoo_02, Float(0.0))
        Call(NpcJump1, NPC_GateBoo_02, -66, 13, -15, 20)
    EndThread
    Call(SetNpcJumpscale, NPC_GateBoo_01, Float(0.0))
    Call(NpcJump1, NPC_GateBoo_01, -89, 14, 28, 20)
    Wait(15)
    Return
    End
};

EvtScript EVS_HaveBoosOpenGate = {
    Thread
        Call(SetNpcFlagBits, NPC_GateBoo_02, NPC_FLAG_IGNORE_PLAYER_COLLISION, true)
        Set(LVar0, NPC_GateBoo_02)
        Set(LVar1, -66)
        Set(LVar2, -66)
        Set(LVar3, 80)
        Set(LVar4, 0)
        Set(LVar5, 30)
        Call(MoveNpcAlongArc)
    EndThread
    Call(SetNpcFlagBits, NPC_GateBoo_01, NPC_FLAG_IGNORE_PLAYER_COLLISION, true)
    Set(LVar0, NPC_GateBoo_01)
    Set(LVar1, -135)
    Set(LVar2, 59)
    Set(LVar3, 40)
    Set(LVar4, 120)
    Set(LVar5, 30)
    Call(MoveNpcAlongArc)
    Return
    End
};

EvtScript EVS_HaveBoosCloseGate = {
    Thread
        Call(SetNpcFlagBits, NPC_GateBoo_02, NPC_FLAG_IGNORE_PLAYER_COLLISION, true)
        Set(LVar0, NPC_GateBoo_02)
        Set(LVar1, -66)
        Set(LVar2, -66)
        Set(LVar3, 0)
        Set(LVar4, 80)
        Set(LVar5, 30)
        Call(MoveNpcAlongArc)
    EndThread
    Call(SetNpcFlagBits, NPC_GateBoo_01, NPC_FLAG_IGNORE_PLAYER_COLLISION, true)
    Set(LVar0, NPC_GateBoo_01)
    Set(LVar1, -135)
    Set(LVar2, 59)
    Set(LVar3, 120)
    Set(LVar4, 40)
    Set(LVar5, 30)
    Call(MoveNpcAlongArc)
    Return
    End
};

EvtScript EVS_Scene_UnlockGate = {
    Call(DisablePlayerInput, true)
    Wait(20 * DT)
    Call(GetCurrentPartnerID, LVar6)
    Call(BringPartnerOut, PARTNER_BOW)
    IfNe(LVar6, PARTNER_BOW)
        Call(SetNpcAnimation, NPC_PARTNER, ANIM_WorldBow_Walk)
        Call(GetNpcPos, NPC_PARTNER, LVar0, LVar1, LVar2)
        Loop(10)
            Add(LVar1, 2)
            Call(SetNpcPos, NPC_PARTNER, LVar0, LVar1, LVar2)
            Wait(1)
        EndLoop
    EndIf
    Wait(10 * DT)
    Call(DisablePartnerAI, 0)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, SPEECH_FACE_SPEAKER_ONLY, MSG_CH3_006C)
    Call(EnablePartnerAI)
    Call(SetNpcPos, NPC_GateBoo_01, -103, 65, 64)
    Call(SetNpcPos, NPC_GateBoo_02, -45, 65, -66)
    Call(PlaySoundAtNpc, NPC_GateBoo_01, SOUND_BOO_VANISH_A, SOUND_SPACE_DEFAULT)
    SetF(LVar0, Float(0.0))
    Loop(20 * DT)
        AddF(LVar0, Float(240.0 / (s32)(20 * DT)))
        Call(SetNpcImgFXParams, NPC_GateBoo_01, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        Call(SetNpcImgFXParams, NPC_GateBoo_02, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        Wait(1)
    EndLoop
    Wait(10 * DT)
    ExecWait(EVS_FlyDownToGate)
    Exec(EVS_FadeToGustyShading)
    Set(LVar2, 1)
    Exec(EVS_OpenGate)
    ExecWait(EVS_HaveBoosOpenGate)
    Wait(10 * DT)
    Thread
        Call(PlayerMoveTo, 25, 0, 60 * DT)
    EndThread
    Wait(3 * DT)
    Call(GetCurrentPartnerID, LVar6)
    IfNe(LVar6, PARTNER_BOW)
        Call(NpcMoveTo, NPC_PARTNER, -38, 23, 60 * DT)
    Else
        Wait(60 * DT)
    EndIf
    Set(LVar2, 1)
    Exec(EVS_CloseGate)
    ExecWait(EVS_StartGustyMusic)
    ExecWait(EVS_HaveBoosCloseGate)
    Wait(10 * DT)
    Call(PlaySoundAtNpc, NPC_GateBoo_01, SOUND_BOO_APPEAR_A, SOUND_SPACE_DEFAULT)
    SetF(LVar0, Float(240.0))
    Loop(20 * DT)
        SubF(LVar0, Float(240.0 / (s32)(20 * DT)))
        Call(SetNpcImgFXParams, NPC_GateBoo_01, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        Call(SetNpcImgFXParams, NPC_GateBoo_02, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        Wait(1)
    EndLoop
    Call(SetNpcPos, NPC_GateBoo_01, NPC_DISPOSE_LOCATION)
    Call(SetNpcPos, NPC_GateBoo_02, NPC_DISPOSE_LOCATION)
    Wait(10 * DT)
    Call(DisablePartnerAI, 0)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, MSG_CH3_006D)
    Call(EnablePartnerAI)
    Wait(5 * DT)
    Call(DisablePlayerPhysics, true)
    Call(SetPlayerAnimation, ANIM_Mario1_Jump)
    Call(GetPlayerPos, LVar0, LVar1, LVar2)
    Call(SetPlayerJumpscale, Float(1.5))
    Call(PlayerJump, LVar0, LVar1, LVar2, 14 * DT)
    Call(SetPlayerAnimation, ANIM_Mario1_Land)
    Call(DisablePlayerPhysics, false)
    Wait(10 * DT)
    Set(GB_StoryProgress, STORY_CH3_UNLOCKED_GUSTY_GULCH)
    Call(PutPartnerAway)
    Call(DisablePlayerInput, false)
    Set(AF_SFF06_BackgroundLitUp, true)
    BindTrigger(Ref(EVS_UseGate), TRIGGER_WALL_PRESS_A, COLLIDER_o67, 1, 0)
    Return
    End
};

EvtScript EVS_SetupForestGate = {
    Switch(GB_StoryProgress)
        CaseLt(STORY_CH3_BOW_JOINED_PARTY)
            Call(InitializeForestFogProperties)
            BindTrigger(Ref(EVS_CantOpenGateMessage), TRIGGER_WALL_PRESS_A, COLLIDER_o67, 1, 0)
        CaseLt(STORY_CH3_UNLOCKED_GUSTY_GULCH)
            Call(InitializeForestFogProperties)
            Loop(0)
                Call(GetPlayerPos, LVar0, LVar1, LVar2)
                IfGe(LVar0, -200)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            ExecWait(EVS_Scene_UnlockGate)
        CaseGe(STORY_CH3_UNLOCKED_GUSTY_GULCH)
            Call(GetEntryID, LVar0)
            IfEq(LVar0, sff_06_ENTRY_0)
                Set(AF_SFF06_BackgroundLitUp, false)
                Call(InitializeForestFogProperties)
            Else
                Set(AF_SFF06_BackgroundLitUp, true)
                Call(InitializeGustyFogProperties)
                Call(EnableSpriteShading, false)
            EndIf
            BindTrigger(Ref(EVS_UseGate), TRIGGER_WALL_PRESS_A, COLLIDER_o67, 1, 0)
    EndSwitch
    Return
    End
};

}; // namespace sff_06
