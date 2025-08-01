#include "mim_03.h"

namespace mim_03 {

#include "world/common/enemy/ForestFuzzy_Wander.inc.c"

#include "world/common/enemy/PiranhaPlant.inc.c"

#include "world/common/enemy/RedPanser_Stationary.inc.c"

// #include "world/common/enemy/HammerBros_Wander.inc.c"

#include "world/common/npc/Oaklie.inc.c"

EvtScript EVS_Bow_SpookNpc = {
    ChildThread
        Loop(0)
            Call(ShakeCam, CAM_DEFAULT, 0, 10, Float(0.5))
            Wait(1)
        EndLoop
    EndChildThread
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_SpookLoop, ANIM_WorldBow_SpookLoop, 0, NPC_PARTNER, MSG_FotB_0009)
    Return
    End
};

EvtScript EVS_Cutscene3 = {
    Call(DisablePlayerInput, true)
    Call(PlayerFaceNpc, NPC_DupiOaklie, true)
    Call(func_802D1270, 110, -315, Float(4.0))
    Wait(5)
    Call(DisablePartnerAI, 0)
    Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    Call(GetPlayerPos, LVar0, 0, LVar2)
    Sub(LVar0, 30)
    Call(SetNpcSpeed, NPC_PARTNER, Float(4.0))
    Call(NpcFlyTo, NPC_PARTNER, LVar0, LVar1, LVar2, 0, 0, EASING_LINEAR)
    Call(PlaySoundAtPlayer, SOUND_EMOTE_IDEA, SOUND_SPACE_DEFAULT)
    Call(ShowEmote, 0, EMOTE_EXCLAMATION, 0, 25, EMOTER_PLAYER, 0, 0, 0, 0)
    Call(PlaySoundAtNpc, NPC_PARTNER, SOUND_EMOTE_IDEA, SOUND_SPACE_DEFAULT)
    Call(ShowEmote, NPC_PARTNER, EMOTE_EXCLAMATION, 0, 25, EMOTER_NPC, 0, 0, 0, 0)
    Wait(25)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_PARTNER, MSG_FotB_0007)
    WaitSecs(3)
    Call(PlaySoundAtPlayer, SOUND_EMOTE_QUESTION, SOUND_SPACE_DEFAULT)
    Call(ShowEmote, 0, EMOTE_QUESTION, 0, 25, EMOTER_PLAYER, 0, 0, 0, 0)
    Wait(25)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_PARTNER, MSG_FotB_0008)
    Call(SetCamLeadPlayer, CAM_DEFAULT, true)
    Wait(1)
    Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    Call(GetNpcPos, NPC_DupiOaklie, LVar0, 0, LVar2)
    Sub(LVar0, 40)
    Call(NpcFlyTo, NPC_PARTNER, LVar0, LVar1, LVar2, 30, 0, EASING_LINEAR)
    Call(SetNpcAnimation, NPC_PARTNER, ANIM_WorldBow_SpookBegin)
    Wait(6)
    Call(SetNpcAnimation, NPC_PARTNER, ANIM_WorldBow_SpookLoop)
    Thread
        Call(SetNpcScale, NPC_PARTNER, Float(2.0), Float(2.0), Float(2.0))
    EndThread
    Call(PlaySoundAtNpc, NPC_PARTNER, SOUND_BOW_SPOOK, SOUND_SPACE_DEFAULT)
    ExecWait(EVS_Bow_SpookNpc)
    Call(SetNpcScale, NPC_PARTNER, Float(2.0), Float(2.0), Float(2.0))
    Wait(1)
    Call(SetNpcScale, NPC_PARTNER, Float(1.9), Float(1.9), Float(1.9))
    Wait(1)
    Call(SetNpcScale, NPC_PARTNER, Float(1.8), Float(1.8), Float(1.8))
    Wait(1)
    Call(SetNpcScale, NPC_PARTNER, Float(1.7), Float(1.7), Float(1.7))
    Wait(1)
    Call(SetNpcScale, NPC_PARTNER, Float(1.6), Float(1.6), Float(1.6))
    Wait(1)
    Call(SetNpcScale, NPC_PARTNER, Float(1.5), Float(1.5), Float(1.5))
    Wait(1)
    Call(SetNpcScale, NPC_PARTNER, Float(1.4), Float(1.4), Float(1.4))
    Wait(1)
    Call(SetNpcScale, NPC_PARTNER, Float(1.3), Float(1.3), Float(1.3))
    Wait(1)
    Call(SetNpcScale, NPC_PARTNER, Float(1.2), Float(1.2), Float(1.2))
    Wait(1)
    Call(SetNpcScale, NPC_PARTNER, Float(1.1), Float(1.1), Float(1.1))
    Wait(1)
    Call(SetNpcScale, NPC_PARTNER, Float(1.0), Float(1.0), Float(1.0))
    Wait(1)
    Call(SetNpcAnimation, NPC_PARTNER, ANIM_WorldBow_Idle)
    WaitSecs(3)
   Thread
        Call(SetNpcAnimation, NPC_DupiOaklie, ANIM_Oaklie_Dupi_Jump)
        Wait(10)
        Call(SetNpcAnimation, NPC_DupiOaklie, ANIM_Oaklie_Dupi_Fall)
    EndThread
    Call(PlaySoundAtNpc, NPC_DupiOaklie, SOUND_NPC_JUMP, SOUND_SPACE_DEFAULT)
    Call(GetNpcPos, NPC_DupiOaklie, LVar0, LVar1, LVar2)
    Add(LVar0, 120)
    Add(LVar1, 10)
    Call(NpcJump0, NPC_DupiOaklie, LVar0, LVar1, LVar2, 10)
    Call(SetNpcAnimation, NPC_DupiOaklie, ANIM_Oaklie_Dupi_Land)
    Call(SetNpcPos, NPC_DupiOaklie, NPC_DISPOSE_LOCATION)
    Call(PlaySoundAtNpc, NPC_PARTNER, SOUND_EMOTE_IDEA, SOUND_SPACE_DEFAULT)
    Call(ShowEmote, NPC_PARTNER, EMOTE_EXCLAMATION, 0, 25, EMOTER_NPC, 0, 0, 0, 0)
    Wait(25)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Flail, ANIM_WorldBow_Flail, 0, NPC_PARTNER, MSG_FotB_000A)
    Call(GetNpcYaw, NPC_PARTNER, LVar3)
    Add(LVar3, 180)
    Call(InterpNpcYaw, NPC_PARTNER, LVar3, 25)
    Wait(10)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, MSG_FotB_000B)
    Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    Call(GetPlayerPos, LVar0, 0, LVar2)
    Sub(LVar0, 30)
    Call(NpcFlyTo, NPC_PARTNER, LVar0, LVar1, LVar2, 30, 0, EASING_LINEAR)
    Call(GetNpcYaw, NPC_PARTNER, LVar3)
    Add(LVar3, 180)
    Call(InterpNpcYaw, NPC_PARTNER, LVar3, 25)
    Wait(10)
    Call(ResetCam, CAM_DEFAULT, 6)
    Call(EnablePartnerAI)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_Cutscene7 = {
    Thread
        Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_CLEAR_BITS, COLLIDER_BluePipeBody, COLLIDER_FLAGS_UPPER_MASK)
        Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_CLEAR_BITS, COLLIDER_BluePipeTop, COLLIDER_FLAGS_UPPER_MASK)
        Call(EnableModel, MODEL_BluePipe, true)
        Wait(1)
    EndThread
    Call(DisablePlayerInput, true)
    Call(DisablePartnerAI, 0)
    Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    Call(GetPlayerPos, LVar0, 0, LVar2)
    Sub(LVar0, 20)
    Sub(LVar2, 20)
    Call(SetNpcPos, NPC_PARTNER, LVar0, LVar1, LVar2)
    Call(PlayerFaceNpc, NPC_Oaklie, 0)
    Call(NpcFaceNpc, NPC_PARTNER, NPC_Oaklie, 0)
    Call(SpeakToPlayer, NPC_Oaklie, ANIM_Oaklie_Talk, ANIM_Oaklie_Idle, 0, MSG_FotB_0024)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_Oaklie, MSG_FotB_0025)
    Call(NpcFacePlayer, NPC_PARTNER, 0)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBow_Laugh, ANIM_WorldBow_Laugh, 0, MSG_FotB_0026)
    Call(SetPlayerAnimation, ANIM_Mario1_ThumbsUp)
    Call(SetNpcAnimation, NPC_PARTNER, ANIM_WorldBow_Laugh)
    WaitSecs(1)
    Call(PlayerFaceNpc, NPC_Oaklie, 0)
    Call(SetPlayerAnimation, ANIM_Mario1_Idle)
    Call(SetNpcAnimation, NPC_PARTNER, ANIM_WorldBow_Idle)
    Set(GB_StoryProgress, STORY_MOD_CUTSCENE_7)
    Exec(EVS_SetupMusic)
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

// EvtScript EVS_NpcIdle_RedPanser = {
//     IfEq(GF_MIM03_DefeatedRedPanser, false)
//         Loop(0)
//             Call(GetSelfVar, 0, LVar0)
//             IfEq(LVar0, 1)
//                 BreakLoop
//             EndIf
//             Wait(1)
//         EndLoop
//         Call(DisablePlayerInput, true)
//         Call(SetNpcAnimation, NPC_RedPanser, ANIM_Panser_Red_Still)
//         Call(PlaySoundAtNpc, NPC_RedPanser, SOUND_NPC_JUMP, SOUND_SPACE_DEFAULT)
//         Call(SetNpcJumpscale, NPC_RedPanser, Float(1.0))
//         Call(GetPlayerPos, LVar0, LVar1, LVar2)
//         Sub(LVar0, 30)
//         Call(NpcJump0, NPC_RedPanser, LVar0, LVar1, LVar2, 20)
//         // Wait(5)
//         Call(SetPlayerAnimation, ANIM_MarioW2_Flail)
//         Call(SetNpcAnimation, NPC_RedPanser, ANIM_Panser_Red_Talk)
//         Call(StartBossBattle, SONG_SPECIAL_BATTLE)
//         Call(DisablePlayerInput, false)
//     Else
//         Call(RemoveNpc, NPC_RedPanser)
//     EndIf
//     Return
//     End
// };

EvtScript EVS_NpcIdle_RedPanser = {
    IfEq(GF_MIM03_DefeatedRedPanser, false)
        Loop(0)
            Call(GetSelfVar, 0, LVar0)
            IfEq(LVar0, 1)
                BreakLoop
            EndIf
            Wait(1)
        EndLoop
        Call(DisablePlayerInput, true)
        Call(SetNpcAnimation, NPC_RedPanser, ANIM_Panser_Red_Still)
        Call(PlaySoundAtNpc, NPC_RedPanser, SOUND_NPC_JUMP, SOUND_SPACE_DEFAULT)
        Call(SetNpcJumpscale, NPC_RedPanser, Float(1.0))
        Call(GetPlayerPos, LVar0, LVar1, LVar2)
        Call(GetNpcPos, NPC_RedPanser, LVar3, 0, 0)
        IfLt(LVar3, LVar0)
            Sub(LVar0, 30)
        Else
            Add(LVar0, 30)
        EndIf
        Call(NpcJump0, NPC_RedPanser, LVar0, LVar1, LVar2, 20)
        Call(NpcFacePlayer, NPC_RedPanser, 0)
        Call(PlayerFaceNpc, NPC_RedPanser, 0)
        Call(SetPlayerAnimation, ANIM_MarioW2_Flail)
        Call(SetNpcAnimation, NPC_RedPanser, ANIM_Panser_Red_Talk)
        Call(StartBossBattle, SONG_SPECIAL_BATTLE)
        Call(DisablePlayerInput, false)
    Else
        Call(RemoveNpc, NPC_RedPanser)
    EndIf
    Return
    End
};

EvtScript EVS_NpcDefeat_RedPanser = {
    Call(GetBattleOutcome, LVar0)
    Switch(LVar0)
        CaseEq(OUTCOME_PLAYER_WON)
            // Call(GetNpcPos, NPC_SELF, LVar0, LVar1, LVar2)
            // Call(DropItemEntity, ITEM_FIRE_FLOWER, LVar0, LVar1, LVar2, ITEM_SPAWN_MODE_FALL_NEVER_VANISH, GF_MIM03_Item_FireFlower)
            Set(GF_MIM03_DefeatedRedPanser, true)
            Call(DisablePlayerInput, false)
            Call(DoNpcDefeat)
        CaseEq(OUTCOME_PLAYER_LOST)
        CaseEq(OUTCOME_PLAYER_FLED)
    EndSwitch
    Return
    End
};

EvtScript EVS_NpcInit_RedPanser = {
    Call(BindNpcIdle, NPC_SELF, Ref(EVS_NpcIdle_RedPanser))
    Call(BindNpcDefeat, NPC_SELF, Ref(EVS_NpcDefeat_RedPanser))
    Return
    End
};

NpcData NpcData_RedPanser[] = {
    {
        .id = NPC_RedPanser,
        .settings = &N(NpcSettings_RedPanser_Stationary),
        .pos = { GEN_RED_PANSER_VEC },
        .flags = ENEMY_FLAG_FLYING,
        .init = &EVS_NpcInit_RedPanser,
        .yaw = GEN_RED_PANSER_DIR,
        .drops = RED_PANSER_DROPS,
        .animations = RED_PANSER_ANIMS,
    },
    // RED_PANSER_FIREBALL_HITBOX(NPC_RedPanser_Fireball),
};

NpcData NpcData_PiranhaPlant[] = {
    {
        .id = NPC_PiranhaPlant,
        .settings = &N(NpcSettings_PiranhaPlant),
        .pos = { GEN_PIRANHA_PLANT_VEC },
        .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING | ENEMY_FLAG_NO_SHADOW_RAYCAST,
        .yaw = GEN_PIRANHA_PLANT_DIR,
        .drops = PIRANHA_PLANT_DROPS,
        .territory = {
            .wander = {
                .centerPos  = { GEN_PIRANHA_PLANT_VEC },
                .wanderSize = { 400 },
                .moveSpeedOverride = NO_OVERRIDE_MOVEMENT_SPEED,
                .wanderShape = SHAPE_CYLINDER,
                .detectPos  = { GEN_PIRANHA_PLANT_VEC },
                .detectSize = { 400 },
                .detectShape = SHAPE_CYLINDER,
                .isFlying = true,
            }
        },
        .animations = PIRANHA_PLANT_ANIMS,
        .aiDetectFlags = AI_DETECT_SIGHT,
    },
    PIRANHA_PLANT_HITBOX(NPC_PiranhaPlant_Hitbox)
};

NpcData NpcData_DupiOaklie = {
    .id = NPC_DupiOaklie,
    .settings = &N(NpcSettings_Oaklie),
    .pos = { NPC_DISPOSE_LOCATION },
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_FLYING,
    // .init = &EVS_NpcInit_Oaklie,
    .yaw = 180,
    .drops = NO_DROPS,
    .animations = DUPI_OAKLIE_ANIMS,
    .tattle = MSG_NpcTattle_Oaklie,
};

EvtScript EVS_NpcInteract_Oaklie = {
    IfEq(GF_MIM03_SpokeToOaklie, false)
        Call(DisablePlayerInput, true)
        Call(DisablePartnerAI, 0)
        Call(SpeakToPlayer, NPC_SELF, ANIM_Oaklie_Talk, ANIM_Oaklie_Idle, 0, MSG_FotB_0028)
        Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_Oaklie, MSG_FotB_0029)
        Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Laugh, ANIM_WorldBow_Laugh, 0, NPC_Oaklie, MSG_FotB_002A)
        Call(SpeakToPlayer, NPC_SELF, ANIM_Oaklie_Talk, ANIM_Oaklie_Idle, 0, MSG_FotB_002B)
        Call(PlaySoundAtNpc, NPC_PARTNER, SOUND_EMOTE_QUESTION, SOUND_SPACE_DEFAULT)
        Call(ShowEmote, NPC_PARTNER, EMOTE_QUESTION, 0, 25, EMOTER_NPC, 0, 0, 0, 0)
        Wait(25)
        Call(SpeakToPlayer, NPC_SELF, ANIM_Oaklie_Talk, ANIM_Oaklie_Idle, 0, MSG_FotB_002C)
        Set(GF_MIM03_SpokeToOaklie, true)
        Call(EnablePartnerAI)
        Call(DisablePlayerInput, false)
    Else
        Call(DisablePlayerInput, true)
        Call(DisablePartnerAI, 0)
        Call(SpeakToPlayer, NPC_SELF, ANIM_Oaklie_Talk, ANIM_Oaklie_Idle, 0, MSG_FotB_002C)
        Call(EnablePartnerAI)
        Call(DisablePlayerInput, false)
    EndIf
    Return
    End
};

EvtScript EVS_NpcInit_Oaklie = {
    Switch(GB_StoryProgress)
        CaseOrEq(STORY_MOD_INTRO)
        CaseOrEq(STORY_MOD_CUTSCENE_1)
        CaseOrEq(STORY_MOD_CUTSCENE_2)
        CaseOrEq(STORY_MOD_CUTSCENE_3)
        CaseOrEq(STORY_MOD_CUTSCENE_4)
        CaseOrEq(STORY_MOD_CUTSCENE_5)
            Call(RemoveNpc, NPC_SELF)
        EndCaseGroup
        CaseEq(STORY_MOD_CUTSCENE_6)
            Call(BindNpcInteract, NPC_SELF, Ref(EVS_NpcInteract_Oaklie))
            Exec(EVS_Cutscene7)
        CaseEq(STORY_MOD_CUTSCENE_7)
            Call(BindNpcInteract, NPC_SELF, Ref(EVS_NpcInteract_Oaklie))
    EndSwitch
    Return
    End
};

NpcData NpcData_Oaklie = {
    .id = NPC_Oaklie,
    .settings = &N(NpcSettings_Oaklie),
    .pos = { 38, 31, 11 },
    .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_FLYING,
    .init = &EVS_NpcInit_Oaklie,
    .yaw = GEN_OAKLIE_DIR,
    .drops = NO_DROPS,
    .animations = OAKLIE_ANIMS,
    .tattle = MSG_NpcTattle_Oaklie,
};


NpcGroupList DefaultNPCs = {
    NPC_GROUP(NpcData_FreezyFuzzy, BTL_FOB1_FORMATION_04, BTL_FOB1_STAGE_00),
    NPC_GROUP(NpcData_RedPanser, BTL_FOB1_FORMATION_05, BTL_FOB1_STAGE_00),
    NPC_GROUP(NpcData_PiranhaPlant, BTL_FOB1_FORMATION_06, BTL_FOB1_STAGE_00),
    NPC_GROUP(NpcData_DupiOaklie),
    NPC_GROUP(NpcData_Oaklie),
    {}
};

}; // namespace mim_03
