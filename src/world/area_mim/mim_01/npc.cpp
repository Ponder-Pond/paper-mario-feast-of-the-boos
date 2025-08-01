#include "mim_01.h"

namespace mim_01 {

#include "world/common/enemy/ForestFuzzy_Wander.inc.c"

EvtScript EVS_Cutscene1 = {
    Call(DisablePlayerInput, true)
    Call(func_802D1270, GEN_MARIO_CUTSCENE1_X, GEN_MARIO_CUTSCENE1_Z, Float(4.0))
    Exec(EVS_UseGate_West)
    Wait(5)
    Call(AwaitPlayerApproach, GEN_CUTSCENE1_X, GEN_CUTSCENE1_Z, 250)
    Call(DisablePartnerAI, 0)
    Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    Call(GetPlayerPos, LVar0, 0, LVar2)
    Add(LVar0, 65)
    Add(LVar2, 30)
    Call(NpcFlyTo, NPC_PARTNER, LVar0, LVar1, LVar2, 30, 0, EASING_LINEAR)
    Wait(5)
    Call(GetNpcYaw, NPC_PARTNER, LVar3)
    Add(LVar3, 180)
    Call(InterpNpcYaw, NPC_PARTNER, LVar3, 12)
    Wait(20)
    Call(GetNpcYaw, NPC_PARTNER, LVar3)
    Sub(LVar3, 180)
    Call(InterpNpcYaw, NPC_PARTNER, LVar3, 12)
    Wait(20)
    Call(PlaySoundAtNpc, NPC_PARTNER, SOUND_EMOTE_QUESTION, SOUND_SPACE_DEFAULT)
    Call(ShowEmote, NPC_PARTNER, EMOTE_QUESTION, 0, 20, EMOTER_NPC, 0, 0, 0, 0)
    Call(InterpPlayerYaw, 180)
    Wait(20)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_PARTNER, MSG_FotB_0000)
    Wait(10)
    Call(GetNpcYaw, NPC_PARTNER, LVar3)
    Add(LVar3, 180)
    Call(InterpNpcYaw, NPC_PARTNER, LVar3, 25)
    Wait(10)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, MSG_FotB_0001)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBow_Laugh, ANIM_WorldBow_Laugh, 0, MSG_FotB_0002)
    Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    Call(GetPlayerPos, LVar0, 0, LVar2)
    Sub(LVar0, 20)
    Sub(LVar2, 1)
    Call(NpcFlyTo, NPC_PARTNER, LVar0, LVar1, LVar2, 30, 0, EASING_LINEAR)
    Call(GetNpcYaw, NPC_PARTNER, LVar3)
    Add(LVar3, 180)
    Call(InterpNpcYaw, NPC_PARTNER, LVar3, 25)
    Wait(10)
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_monw, COLLIDER_FLAGS_UPPER_MASK)
    Call(ParentColliderToModel, COLLIDER_monw2, MODEL_w1)
    BindTrigger(Ref(EVS_DisabledGate_West),  TRIGGER_WALL_PRESS_A, COLLIDER_monw2, 1, 0)
    Call(EnablePartnerAI)
    Call(DisablePlayerInput, false)
    Return
    End
};

NpcData NpcData_FreezyFuzzy = {
    .id = NPC_FreezyFuzzy,
    .settings = &N(NpcSettings_FreezyFuzzy_Wander),
    .pos = { GEN_FREEZY_FUZZY_VEC },
    .flags = 0,
    .yaw = GEN_FREEZY_FUZZY_DIR,
    .drops = FREEZY_FUZZY_DROPS,
    .territory = {
        .wander = {
            .centerPos  = { GEN_FREEZY_FUZZY_VEC },
            .wanderSize = { 400 },
            .moveSpeedOverride = NO_OVERRIDE_MOVEMENT_SPEED,
            .wanderShape = SHAPE_CYLINDER,
            .detectPos  = { GEN_FREEZY_FUZZY_VEC },
            .detectSize = { 400 },
            .detectShape = SHAPE_CYLINDER,
            .isFlying = true,
        }
    },
    .animations = FREEZY_FUZZY_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
};

NpcGroupList DefaultNPCs = {
    NPC_GROUP(NpcData_FreezyFuzzy, BTL_FOB1_FORMATION_00, BTL_FOB1_STAGE_00),
    {}
};

}; // namespace mim_01
