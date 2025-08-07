#include "mim_08.h"

namespace mim_08 {

#include "world/common/enemy/ForestFuzzy_Wander.inc.c"

#include "world/common/enemy/PiranhaPlant.inc.c"

#include "world/common/enemy/Beehive.inc.c"

#include "world/common/enemy/BooBully.inc.c"

#include "world/common/npc/Oaklie.inc.c"

EvtScript EVS_Cutscene2 = {
    Call(DisablePlayerInput, true)
    Call(func_802D1270, GEN_MARIO_CUTSCENE2_X, GEN_MARIO_CUTSCENE2_Z, Float(4.0))
    Exec(EVS_UseGate_South)
    Wait(30)
    Call(GetPlayerPos, LVar0, 0, LVar2)
    Set(LVar0, -90)
    Set(LVar2, 307)
    Call(func_802D1270, LVar0, LVar2, Float(4.0))
    Wait(15)
    Call(DisablePartnerAI, 0)
    Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    Call(GetPlayerPos, LVar0, 0, LVar2)
    Add(LVar0, 30)
    Add(LVar2, 0)
    Call(NpcFlyTo, NPC_PARTNER, LVar0, LVar1, LVar2, 30, 0, EASING_LINEAR)
    Wait(10)
    Call(GetNpcYaw, NPC_PARTNER, LVar3)
    Sub(LVar3, 180)
    Call(InterpNpcYaw, NPC_PARTNER, LVar3, 12)
    Wait(20)
    Call(GetNpcYaw, NPC_PARTNER, LVar3)
    Add(LVar3, 180)
    Call(InterpNpcYaw, NPC_PARTNER, LVar3, 12)
    Wait(20)
    Call(InterpPlayerYaw, 180)
    Wait(20)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, MSG_FotB_0005)
    Wait(5)
    Call(GetNpcYaw, NPC_PARTNER, LVar3)
    Sub(LVar3, 180)
    Call(InterpNpcYaw, NPC_PARTNER, LVar3, 12)
    Wait(20)
    Call(GetNpcYaw, NPC_PARTNER, LVar3)
    Add(LVar3, 180)
    Call(InterpNpcYaw, NPC_PARTNER, LVar3, 12)
    Wait(20)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, MSG_FotB_0006)
    Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    Call(GetPlayerPos, LVar0, 0, LVar2)
    Sub(LVar0, 30)
    Sub(LVar2, 1)
    Call(NpcFlyTo, NPC_PARTNER, LVar0, LVar1, LVar2, 30, 0, EASING_LINEAR)
    Call(GetNpcYaw, NPC_PARTNER, LVar3)
    Add(LVar3, 180)
    Call(InterpNpcYaw, NPC_PARTNER, LVar3, 25)
    Wait(10)
    Call(EnablePartnerAI)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_BooBully_SpookPlayer = {
    Loop(5)
        // Call(SetCamDistance, CAM_DEFAULT, 400)
        // Call(PanToTarget, CAM_DEFAULT, 0, true)
        // Wait(1)
        // Call(SetCamDistance, CAM_DEFAULT, 450)
        // Call(PanToTarget, CAM_DEFAULT, 0, true)
        Call(ShakeCam, CAM_DEFAULT, 0, 10, Float(0.5))
        Wait(1)
    EndLoop
    Call(SetPlayerAnimation, ANIM_Mario1_Flail)
    Call(SetNpcAnimation, NPC_PARTNER, ANIM_WorldBow_Flail)
    Wait(30)
    // Call(SpeakToPlayer, NPC_Bootler, ANIM_Bootler_Spook, ANIM_Bootler_Spook, 0, MSG_CH3_0002)
    // Call(SetNpcAnimation, NPC_PARTNER, ANIM_WorldBow_Idle)
    // Call(SetPlayerAnimation, ANIM_Mario1_Idle)
    Return
    End
};

EvtScript EVS_Cutscene4 = {
    SetF(LVar0, Float(0.0))
    Call(NpcFacePlayer, NPC_BooBully_1, 0)
    Call(SetNpcImgFXParams, NPC_BooBully_1, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
    Call(SetNpcImgFXParams, NPC_BooBully_2, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
    Call(DisablePlayerInput, true)
    Call(func_802D1270, 40, -30, Float(4.0))
    Wait(5)
    Call(DisablePartnerAI, 0)
    Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    Call(GetPlayerPos, LVar0, 0, LVar2)
    Sub(LVar0, 20)
    Sub(LVar2, 20)
    Call(SetNpcSpeed, NPC_PARTNER, Float(4.0))
    Call(NpcFlyTo, NPC_PARTNER, LVar0, LVar1, LVar2, 0, 0, EASING_LINEAR)
    Call(PlayerFaceNpc, NPC_DupiOaklie, 0)
    Call(NpcFaceNpc, NPC_PARTNER, NPC_DupiOaklie, 0)
    Call(PlaySoundAtPlayer, SOUND_EMOTE_IDEA, SOUND_SPACE_DEFAULT)
    Call(ShowEmote, 0, EMOTE_EXCLAMATION, 0, 25, EMOTER_PLAYER, 0, 0, 0, 0)
    Call(PlaySoundAtNpc, NPC_PARTNER, SOUND_EMOTE_IDEA, SOUND_SPACE_DEFAULT)
    Call(ShowEmote, NPC_PARTNER, EMOTE_EXCLAMATION, 0, 25, EMOTER_NPC, 0, 0, 0, 0)
    Wait(25)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_PARTNER, MSG_FotB_000C)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_PARTNER, MSG_FotB_000D)
    Call(SetPlayerAnimation, ANIM_MarioW2_SpeakUp)
    Wait(20)
    Call(PlaySoundAtNpc, NPC_PARTNER, SOUND_EMOTE_IDEA, SOUND_SPACE_DEFAULT)
    Call(ShowEmote, NPC_PARTNER, EMOTE_EXCLAMATION, 0, 25, EMOTER_NPC, 0, 0, 0, 0)
    Wait(25)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_PARTNER, MSG_FotB_000E)
    WaitSecs(1)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_PARTNER, MSG_FotB_000F)
    Call(NpcFacePlayer, NPC_DupiOaklie, 0)
    WaitSecs(3)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_PARTNER, MSG_FotB_0010)
    WaitSecs(1)
    Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    // Sub(LVar0, 50)
    // Sub(LVar2, 50)
    Call(SetNpcPos, NPC_BooBully_1, 1, LVar1, -75)
    Call(PlaySoundAtNpc, NPC_BooBully_1, SOUND_BOO_APPEAR_A, SOUND_SPACE_DEFAULT)
    Thread
        SetF(LVar0, Float(0.0))
        Loop(20)
            AddF(LVar0, Float(12.0))
            Call(SetNpcImgFXParams, NPC_BooBully_1, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
            Wait(1)
        EndLoop
    EndThread
    // Wait(25)
    Call(SetNpcAnimation, NPC_BooBully_1, ANIM_BooBully_Scare)
    Call(PlaySoundAtNpc, NPC_BooBully_1, SOUND_BOO_SPOOK, SOUND_SPACE_DEFAULT)
    ExecWait(EVS_BooBully_SpookPlayer)
    Call(GetNpcPos, NPC_DupiOaklie, LVar2, LVar3, LVar4)
    Thread
        Add(LVar4, 15)
        Loop(8)
            PlayEffect(EFFECT_BIG_SMOKE_PUFF, LVar2, LVar3, LVar4, 1, 1, 1, 1)
            Wait(5)
        EndLoop
    EndThread
    Call(PlaySoundAtNpc, NPC_DupiOaklie, SOUND_GHOST_TRANSFORM, SOUND_SPACE_DEFAULT)
    Call(MakeLerp, 0, 1440, 20, EASING_QUADRATIC_IN)
    Loop(0)
        Call(UpdateLerp)
        Call(SetNpcRotation, NPC_DupiOaklie, 0, LVar0, 0)
        Wait(1)
        IfEq(LVar1, 0)
            BreakLoop
        EndIf
    EndLoop
    Call(PlaySoundAtNpc, NPC_DupiOaklie, SOUND_SMOKE_BURST, SOUND_SPACE_DEFAULT)
    Call(SetNpcPos, NPC_DupiOaklie, NPC_DISPOSE_LOCATION)
    Call(EnableNpcShadow, NPC_DupiOaklie, false)
    Call(SetNpcPos, NPC_Duplighost, LVar2, LVar3, LVar4)
    Call(SetNpcYaw, NPC_Duplighost, 0)
    Call(MakeLerp, -1440, 0, 20, EASING_QUADRATIC_OUT)
    Loop(0)
        Call(UpdateLerp)
        Call(SetNpcRotation, NPC_SELF, 0, LVar0, 0)
        Wait(1)
        IfEq(LVar1, 0)
            BreakLoop
        EndIf
    EndLoop
    Call(SetNpcAnimation, NPC_Duplighost, ANIM_Duplighost_Anim06)
    Wait(10)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Flail, ANIM_WorldBow_Flail, 0, NPC_PARTNER, MSG_FotB_0011)
    Call(GetNpcPos, NPC_BooBully_1, 0, LVar1, 0)
    Call(SetNpcPos, NPC_BooBully_2, 68, LVar1, -8)
    Call(NpcFacePlayer, NPC_BooBully_2, 0)
    Call(PlaySoundAtNpc, NPC_BooBully_2, SOUND_BOO_APPEAR_A, SOUND_SPACE_DEFAULT)
    SetF(LVar0, Float(0.0))
    Loop(20)
        AddF(LVar0, Float(12.0))
        Call(SetNpcImgFXParams, NPC_BooBully_2, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        Wait(1)
    EndLoop
    Call(SpeakToNpc, NPC_BooBully_2, ANIM_BooBully_Laugh, ANIM_BooBully_Laugh, 0, NPC_PARTNER, MSG_FotB_0012)
    Call(SetNpcAnimation, NPC_BooBully_1, ANIM_BooBully_Laugh)
    Call(SetNpcAnimation, NPC_BooBully_2, ANIM_BooBully_Laugh)
    Call(SetNpcVar, NPC_Duplighost, 0, 1)
    Call(EnablePartnerAI)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_BooBullyScareAndVanish = {
    Call(SetNpcFlagBits, LVarA, NPC_FLAG_IGNORE_CAMERA_FOR_YAW, false)
    Call(NpcFacePlayer, LVarA, 0)
    Wait(5)
    // Call(SetNpcAnimation, LVarA, ANIM_BooBully_Spook)
    Call(SetNpcAnimation, LVarA, ANIM_BooBully_Scare)
    Wait(57)
    Call(MakeLerp, 255, 0, 60, EASING_LINEAR)
    Label(0)
        Call(UpdateLerp)
        Call(SetNpcImgFXParams, LVarA, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(0)
        EndIf
    Call(SetNpcPos, LVarA, NPC_DISPOSE_LOCATION)
    Return
    End
};

EvtScript EVS_Cutscene5 = {
    Call(DisablePlayerInput, true)
    Call(SetPlayerPos, GEN_CUTSCENE5_VEC)
    Call(GetPlayerPos, LVar0, 0, LVar2)
    Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    Add(LVar0, 20)
    Add(LVar2, 20)
    Call(SetNpcPos, NPC_PARTNER, LVar0, LVar1, LVar2)
    Call(DisablePartnerAI, 0)
    Call(GetNpcPos, NPC_BooBully_1, LVar0, LVar1, LVar2)
    Call(SetNpcPos, NPC_BooBully_1, -36, LVar1, -69)
    Call(GetNpcPos, NPC_BooBully_2, LVar0, LVar1, LVar2)
    Call(SetNpcPos, NPC_BooBully_2, -6, LVar1, -45)
    Call(SetNpcPos, NPC_Duplighost, 19, 33, -14)
    Call(PlayerFaceNpc, NPC_BooBully_2, 0)
    Call(NpcFaceNpc, NPC_PARTNER, NPC_BooBully_2, 0)
    Call(NpcFacePlayer, NPC_Duplighost, 0)
    Call(SetNpcAnimation, NPC_BooBully_1, ANIM_BooBully_Flail)
    Call(SetNpcAnimation, NPC_BooBully_2, ANIM_BooBully_Flail)
    Call(SetNpcAnimation, NPC_Duplighost, ANIM_Duplighost_Anim0A)
    Call(SpeakToPlayer, NPC_BooBully_2, ANIM_BooBully_Flail, ANIM_BooBully_Flail, 0, MSG_FotB_0013)
    Call(SetNpcAnimation, NPC_BooBully_1, ANIM_BooBully_Idle)
    Call(SetNpcAnimation, NPC_BooBully_2, ANIM_BooBully_Idle)
    // Wait(1)
    Call(SpeakToPlayer, NPC_BooBully_2, ANIM_BooBully_Talk, ANIM_BooBully_Idle, 0, MSG_FotB_0014)
    // Wait(15)
    Call(PlaySoundAtNpc, NPC_BooBully_2, SOUND_BOO_SPOOK, SOUND_SPACE_DEFAULT)
    Set(LVarA, NPC_BooBully_2)
    Exec(EVS_BooBullyScareAndVanish)
    Call(PlaySoundAtNpc, NPC_BooBully_1, SOUND_BOO_SPOOK, SOUND_SPACE_DEFAULT)
    Set(LVarA, NPC_BooBully_1)
    Exec(EVS_BooBullyScareAndVanish)
    Call(PlaySoundAtNpc, NPC_BooBully_1, SOUND_BOO_VANISH_A, SOUND_SPACE_DEFAULT)
    WaitSecs(4)
    Wait(15)
    Call(SetNpcPos, NPC_BooBully_2, NPC_DISPOSE_LOCATION)
    Call(SetNpcPos, NPC_BooBully_1, NPC_DISPOSE_LOCATION)
    Call(GetNpcPos, NPC_Duplighost, LVar0, LVar1, LVar2)
    // Call(PlaySound, SOUND_CHIME_SOLVED_PUZZLE)
    Call(MakeItemEntity, ITEM_RUINS_KEY, LVar0, LVar1, LVar2, ITEM_SPAWN_MODE_TOSS_NEVER_VANISH, GF_MIM08_Item_ChestKey)
    Call(OnDefeatEnemy)
    // Call(RemoveNpc, NPC_Duplighost)
    Call(SetNpcPos, NPC_Duplighost, NPC_DISPOSE_LOCATION)
    Call(NpcFacePlayer, NPC_PARTNER, 0)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, MSG_FotB_0015)
    Call(PlaySoundAtNpc, NPC_PARTNER, SOUND_EMOTE_IDEA, SOUND_SPACE_DEFAULT)
    Call(ShowEmote, NPC_PARTNER, EMOTE_EXCLAMATION, 0, 25, EMOTER_NPC, 0, 0, 0, 0)
    Wait(25)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBow_Flail, ANIM_WorldBow_Flail, 0, MSG_FotB_0016)
    Call(SetNpcFlagBits, NPC_Oaklie, NPC_FLAG_INVISIBLE, true)
    Call(SetNpcPos, GEN_CHEST_OAKLIE_VEC)
    Call(SpeakToPlayer, NPC_Oaklie, ANIM_Oaklie_Talk, ANIM_Oaklie_Idle, 0, MSG_FotB_0017)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_SpookLoop, ANIM_WorldBow_SpookLoop, 0, NPC_Oaklie, MSG_FotB_0018)
    Call(PlaySoundAtPlayer, SOUND_EMOTE_QUESTION, SOUND_SPACE_DEFAULT)
    Call(ShowEmote, 0, EMOTE_QUESTION, 0, 25, EMOTER_PLAYER, 0, 0, 0, 0)
    Wait(25)
    Call(SetPlayerAnimation, ANIM_MarioW2_SpeakUp)
    Wait(20)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, MSG_FotB_0019)
    Call(SetNpcAnimation, NPC_PARTNER, ANIM_WorldBow_Laugh)
    Call(SpeakToPlayer, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, MSG_FotB_001A)
    Call(EnablePartnerAI)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_Cutscene6 = {
    WaitSecs(2)
    Call(DisablePlayerInput, true)
    Call(DisablePartnerAI, 0)
    Call(SetNpcYaw, NPC_Oaklie, 180)
    Call(SetNpcFlagBits, NPC_Oaklie, NPC_FLAG_INVISIBLE, false)
    Wait(15)
    Call(SetNpcPos, NPC_Oaklie, -92, 30, -33)
   Thread
        Call(SetNpcAnimation, NPC_Oaklie, ANIM_Oaklie_Jump)
        Wait(15)
        Call(SetNpcAnimation, NPC_Oaklie, ANIM_Oaklie_Fall)
    EndThread
    Call(PlaySoundAtNpc, NPC_Oaklie, SOUND_NPC_JUMP, SOUND_SPACE_DEFAULT)
    Call(SetNpcJumpscale, NPC_Oaklie, Float(1.0))
    Call(GetPlayerPos, LVar0, 0, LVar2)
    Add(LVar0, 50)
    Add(LVar2, 50)
    Call(NpcJump0, NPC_Oaklie, LVar0, 33, LVar2, 20)
    Call(SetNpcAnimation, NPC_Oaklie, ANIM_Oaklie_Land)
    Call(PlayerFaceNpc, NPC_Oaklie, 0)
    Call(NpcFaceNpc, NPC_PARTNER, NPC_Oaklie, 0)
    Wait(5)
    Call(SetNpcAnimation, NPC_Oaklie, ANIM_Oaklie_Idle)
    Wait(15)
    Call(GetPlayerPos, LVar0, LVar1, LVar2)
    Call(UseSettingsFrom, CAM_DEFAULT, LVar0, LVar1, LVar2)
    Call(SetCamSpeed, CAM_DEFAULT, Float(4.0))
    Call(SetCamDistance, CAM_DEFAULT, 300)
    Call(SetPanTarget, CAM_DEFAULT, LVar0, LVar1, LVar2)
    Call(PanToTarget, CAM_DEFAULT, 0, true)
    Call(SpeakToNpc, NPC_Oaklie, ANIM_Oaklie_Talk, ANIM_Oaklie_Idle, 0, NPC_Oaklie, MSG_FotB_001B)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_PARTNER, MSG_FotB_001C)
    Call(PlaySoundAtNpc, NPC_Oaklie, SOUND_EMOTE_IDEA, SOUND_SPACE_DEFAULT)
    Call(ShowEmote, NPC_Oaklie, EMOTE_EXCLAMATION, 0, 25, EMOTER_NPC, 0, 0, 0, 0)
    Wait(25)
    Call(NpcFacePlayer, NPC_Oaklie, 0)
    Call(SpeakToPlayer, NPC_Oaklie, ANIM_Oaklie_Talk, ANIM_Oaklie_Idle, 0, MSG_FotB_001D)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_Oaklie, MSG_FotB_001E)
    Call(GetNpcYaw, NPC_Oaklie, LVar3)
    Sub(LVar3, 180)
    Call(InterpNpcYaw, NPC_Oaklie, LVar3, 12)
    Wait(20)
    WaitSecs(1)
    Call(SpeakToNpc, NPC_Oaklie, ANIM_Oaklie_Talk, ANIM_Oaklie_Idle, 0, NPC_Oaklie, MSG_FotB_001F)
    Call(GetNpcYaw, NPC_Oaklie, LVar3)
    Add(LVar3, 180)
    Call(InterpNpcYaw, NPC_Oaklie, LVar3, 12)
    Wait(20)
    Call(GetNpcYaw, NPC_Oaklie, LVar3)
    Sub(LVar3, 180)
    Call(InterpNpcYaw, NPC_Oaklie, LVar3, 12)
    Wait(20)
    Call(SpeakToNpc, NPC_Oaklie, ANIM_Oaklie_Talk, ANIM_Oaklie_Idle, 0, NPC_Oaklie, MSG_FotB_0020)
    Call(SpeakToNpc, NPC_PARTNER, ANIM_WorldBow_Talk, ANIM_WorldBow_Idle, 0, NPC_PARTNER, MSG_FotB_0021)
    Call(NpcFacePlayer, NPC_Oaklie, 0)
    Call(SpeakToPlayer, NPC_Oaklie, ANIM_Oaklie_Talk, ANIM_Oaklie_Idle, 0, MSG_FotB_0022)
    Call(GetNpcYaw, NPC_Oaklie, LVar3)
    Sub(LVar3, 180)
    Call(InterpNpcYaw, NPC_Oaklie, LVar3, 12)
    Wait(20)
    Call(SpeakToNpc, NPC_Oaklie, ANIM_Oaklie_Talk, ANIM_Oaklie_Idle, 0, NPC_Oaklie, MSG_FotB_0023)
    Call(FadeOutMusic, 0, 1500)
    Call(GotoMapSpecial, Ref("mim_03"), mim_03_ENTRY_5, TRANSITION_STANDARD)
    Wait(100)
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
            .wanderSize = { 40 },
            .moveSpeedOverride = NO_OVERRIDE_MOVEMENT_SPEED,
            .wanderShape = SHAPE_CYLINDER,
            .detectPos  = { GEN_FREEZY_FUZZY_VEC },
            .detectSize = { 40 },
            .detectShape = SHAPE_CYLINDER,
            .isFlying = true,
        }
    },
    .animations = FREEZY_FUZZY_ANIMS,
    .aiDetectFlags = AI_DETECT_SIGHT,
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

EvtScript EVS_NpcIdle_Beehive = {
    // Label(100)
    // Wait(1)
    // IfEq(MV_ShakedTree, false)
    //     Goto(100)
    // EndIf
    // Call(SetNpcPos, NPC_SELF, -280, 108, 0)
    // Call(SetNpcPos, NPC_Buzzbee -280, 108, 0)
    // Call(SetNpcFlagBits, NPC_Buzzbee, NPC_FLAG_INVISIBLE, false)
    // Wait(15)
    // Loop(6)
    //     Set(LVar0, 2)
    //     Call(TranslateModel, MODEL_Beehive1, LVar0, 0, LVar0)
    //     Call(TranslateModel, MODEL_Beehive2, LVar0, 0, LVar0)
    //     Call(TranslateModel, MODEL_o443, LVar0, 0, LVar0)
    //     Call(TranslateModel, MODEL_o442, LVar0, 0, LVar0)
    //     Wait(1)
    //     Set(LVar0, -2)
    //     Call(TranslateModel, MODEL_Beehive1, LVar0, 0, LVar0)
    //     Call(TranslateModel, MODEL_Beehive2, LVar0, 0, LVar0)
    //     Call(TranslateModel, MODEL_o443, LVar0, 0, LVar0)
    //     Call(TranslateModel, MODEL_o442, LVar0, 0, LVar0)
    //     Wait(1)
    // EndLoop
    // Set(LVar0, 0)
    // Call(TranslateModel, MODEL_Beehive1, LVar0, 0, LVar0)
    // Call(TranslateModel, MODEL_Beehive2, LVar0, 0, LVar0)
    // Call(TranslateModel, MODEL_o443, LVar0, 0, LVar0)
    // Call(TranslateModel, MODEL_o442, LVar0, 0, LVar0)
    // WaitSecs(1)
    // Call(NpcFacePlayer, NPC_Buzzbee, 0)
    // Call(PlayerFaceNpc, NPC_Buzzbee, 0)
    // Call(GetPlayerPos, LVar0, LVar1, LVar2)
    // Call(UseSettingsFrom, CAM_DEFAULT, LVar0, LVar1, LVar2)
    // Call(SetCamSpeed, CAM_DEFAULT, Float(4.0))
    // Call(SetCamDistance, CAM_DEFAULT, 350)
    // Call(GetNpcPos, NPC_Buzzbee, LVar0, LVar1, LVar2)
    // Add(LVar1, 40)
    // Call(SetPanTarget, CAM_DEFAULT, LVar0, LVar1, LVar2)
    // Call(PanToTarget, CAM_DEFAULT, 0, true)
    // Wait(1)
    // Call(SetPlayerAnimation, ANIM_Mario1_Flail)
    // Call(GetPlayerPos, LVar0, 0, LVar2)
    // Sub(LVar0, 20)
    // Sub(LVar2, 40)
    // Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    // Call(SetNpcAnimation, NPC_Buzzbee, ANIM_InsectsDK3_Buzzbee)
    // Call(SetNpcJumpscale, NPC_Buzzbee, Float(0.0))
    // Call(NpcJump1, NPC_Buzzbee, LVar0, LVar1, LVar2, 60)
    // Call(NpcFlyTo, NPC_Buzzbee, LVar0, LVar1, LVar2, 30, 0, EASING_LINEAR)
    // Wait(30)
    // Call(StartBossBattle, SONG_SPECIAL_BATTLE)
    Return
    End
};

EvtScript EVS_NpcDefeat_Beehive = {
//     Call(GetBattleOutcome, LVar0)
//     Switch(LVar0)
//         CaseEq(OUTCOME_PLAYER_WON)
//             Call(DoNpcDefeat)
//         CaseEq(OUTCOME_PLAYER_LOST)
//         CaseEq(OUTCOME_PLAYER_FLED)
//     EndSwitch
    Return
    End
};

EvtScript EVS_NpcInit_Beehive = {
    IfEq(GF_MIM08_DefeatedBeehive, false)
        Call(SetNpcFlagBits, NPC_SELF, NPC_FLAG_INVISIBLE, true)
        Call(SetNpcFlagBits, NPC_SELF, NPC_FLAG_HAS_SHADOW, false)
        // Call(BindNpcIdle, NPC_SELF, Ref(EVS_NpcIdle_Beehive))
        // Call(BindNpcDefeat, NPC_SELF, Ref(EVS_NpcDefeat_Beehive))
    Else
        Call(RemoveNpc, NPC_SELF)
    EndIf
    Return
    End
};

EvtScript EVS_NpcIdle_Buzzbee = {
    Label(100)
    Wait(1)
    IfEq(MV_ShakedTree, false)
        Goto(100)
    EndIf
    Call(SetNpcPos, NPC_SELF, -280, 108, 0)
    Call(SetNpcPos, NPC_Buzzbee -280, 108, 0)
    Call(SetNpcFlagBits, NPC_Buzzbee, NPC_FLAG_INVISIBLE, false)
    Wait(15)
    Loop(6)
        Set(LVar0, 2)
        Call(TranslateModel, MODEL_Beehive1, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_Beehive2, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_o443, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_o442, LVar0, 0, LVar0)
        Wait(1)
        Set(LVar0, -2)
        Call(TranslateModel, MODEL_Beehive1, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_Beehive2, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_o443, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_o442, LVar0, 0, LVar0)
        Wait(1)
    EndLoop
    Set(LVar0, 0)
    Call(TranslateModel, MODEL_Beehive1, LVar0, 0, LVar0)
    Call(TranslateModel, MODEL_Beehive2, LVar0, 0, LVar0)
    Call(TranslateModel, MODEL_o443, LVar0, 0, LVar0)
    Call(TranslateModel, MODEL_o442, LVar0, 0, LVar0)
    Wait(1)
    Call(NpcFacePlayer, NPC_Buzzbee, 0)
    // Call(GetPlayerPos, LVar0, LVar1, LVar2)
    // Call(UseSettingsFrom, CAM_DEFAULT, LVar0, LVar1, LVar2)
    // Call(SetCamSpeed, CAM_DEFAULT, Float(4.0))
    // Call(SetCamDistance, CAM_DEFAULT, 350)
    // Call(GetNpcPos, NPC_Buzzbee, LVar0, LVar1, LVar2)
    // Add(LVar1, 40)
    // Call(SetPanTarget, CAM_DEFAULT, LVar0, LVar1, LVar2)
    // Call(PanToTarget, CAM_DEFAULT, 0, true)
    // Wait(1)
    Call(SetPlayerAnimation, ANIM_Mario1_Flail)
    Call(GetPlayerPos, LVar0, 0, LVar2)
    Sub(LVar0, 20)
    Sub(LVar2, 40)
    Call(GetNpcPos, NPC_PARTNER, 0, LVar1, 0)
    Call(SetNpcAnimation, NPC_Buzzbee, ANIM_InsectsDK3_Buzzbee)
    Call(SetNpcJumpscale, NPC_Buzzbee, Float(0.0))
    Call(NpcJump1, NPC_Buzzbee, LVar0, LVar1, LVar2, 5)
    Call(PlayerFaceNpc, NPC_Buzzbee, 0)
    // Call(NpcFlyTo, NPC_Buzzbee, LVar0, LVar1, LVar2, 30, 0, EASING_LINEAR)
    Wait(15)
    Call(StartBossBattle, SONG_SPECIAL_BATTLE)
    Return
    End
};

EvtScript EVS_NpcDefeat_Buzzbee = {
    Call(GetBattleOutcome, LVar0)
    Switch(LVar0)
        CaseEq(OUTCOME_PLAYER_WON)
            Set(GF_MIM08_DefeatedBeehive, true)
            ExecWait(EVS_DropShrinkStomp)
            Call(DoNpcDefeat)
        CaseEq(OUTCOME_PLAYER_LOST)
        CaseEq(OUTCOME_PLAYER_FLED)
    EndSwitch
    Return
    End
};

EvtScript EVS_NpcInit_Buzzbee = {
    IfEq(GF_MIM08_DefeatedBeehive, false)
        Call(SetNpcFlagBits, NPC_SELF, NPC_FLAG_INVISIBLE, true)
        Call(SetNpcFlagBits, NPC_SELF, NPC_FLAG_HAS_SHADOW, false)
        Call(BindNpcIdle, NPC_SELF, Ref(EVS_NpcIdle_Buzzbee))
        Call(BindNpcDefeat, NPC_SELF, Ref(EVS_NpcDefeat_Buzzbee))
    Else
        Call(RemoveNpc, NPC_SELF)
    EndIf
    Return
    End
};

NpcData NpcData_Beehive[] = {
    {
        .id = NPC_Beehive,
        .settings = &N(NpcSettings_Beehive),
        .pos = { NPC_DISPOSE_LOCATION },
        .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING | ENEMY_FLAG_NO_SHADOW_RAYCAST | ENEMY_FLAG_ACTIVE_WHILE_OFFSCREEN,
        .init = &EVS_NpcInit_Beehive,
        .yaw = GEN_BEEHIVE_DIR,
        .drops = NO_DROPS,
        .animations = BEEHIVE_ANIMS,
    },
    {
        .id = NPC_Buzzbee,
        .settings = &N(NpcSettings_Beehive),
        .pos = { NPC_DISPOSE_LOCATION },
        .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING | ENEMY_FLAG_NO_SHADOW_RAYCAST | ENEMY_FLAG_ACTIVE_WHILE_OFFSCREEN,
        .init = &EVS_NpcInit_Buzzbee,
        .yaw = 0,
        .drops = NO_DROPS,
        .animations = BUZZBEE_ANIMS,
    },
};

EvtScript EVS_NpcIdle_BooBully_1 = {
    Return
    End
};

EvtScript EVS_NpcInit_BooBully_1 = {
    Call(BindNpcIdle, NPC_SELF, Ref(EVS_NpcIdle_BooBully_1))
    Return
    End
};

NpcData NpcData_BooBully_1 = {
    .id = NPC_BooBully_1,
    .settings = &N(NpcSettings_BooBully),
    .pos = { NPC_DISPOSE_LOCATION },
    .flags = COMMON_PASSIVE_FLAGS | ENEMY_FLAG_NO_DELAY_AFTER_FLEE | ENEMY_FLAG_ACTIVE_WHILE_OFFSCREEN | ENEMY_FLAG_DO_NOT_AUTO_FACE_PLAYER | ENEMY_FLAG_NO_DROPS,
    .init = &EVS_NpcInit_BooBully_1,
    .yaw = GEN_BOO_BULLY_DIR,
    .drops = NO_DROPS,
    .animations = BOO_BULLY_ANIMS,
};

EvtScript EVS_NpcIdle_BooBully_2 = {
    // Loop(0)
    //     Call(GetSelfVar, 0, LVar0)
    //     IfEq(LVar0, 1)
    //         BreakLoop
    //     EndIf
    //     Wait(1)
    // EndLoop
    // Call(StartBossBattle, SONG_SPECIAL_BATTLE)
    Return
    End
};

EvtScript EVS_NpcDefeat_BooBully_2 = {
    // Call(GetBattleOutcome, LVar0)
    // Switch(LVar0)
    //     CaseEq(OUTCOME_PLAYER_WON)
    //         IfLt(GB_StoryProgress, STORY_MOD_CUTSCENE_5)
    //             Exec(EVS_Cutscene5)
    //             Set(GB_StoryProgress, STORY_MOD_CUTSCENE_5)
    //         EndIf
    //     CaseEq(OUTCOME_PLAYER_LOST)
    //     CaseEq(OUTCOME_PLAYER_FLED)
    // EndSwitch
    Return
    End
};

EvtScript EVS_NpcInit_BooBully_2 = {
    Call(BindNpcIdle, NPC_SELF, Ref(EVS_NpcIdle_BooBully_2))
    Call(BindNpcDefeat, NPC_SELF, Ref(EVS_NpcDefeat_BooBully_2))
    Return
    End
};

NpcData NpcData_BooBully_2 = {
    .id = NPC_BooBully_2,
    .settings = &N(NpcSettings_BooBully),
    .pos = { NPC_DISPOSE_LOCATION },
    .flags = COMMON_PASSIVE_FLAGS | ENEMY_FLAG_NO_DELAY_AFTER_FLEE | ENEMY_FLAG_ACTIVE_WHILE_OFFSCREEN | ENEMY_FLAG_DO_NOT_AUTO_FACE_PLAYER | ENEMY_FLAG_NO_DROPS,
    .init = &EVS_NpcInit_BooBully_2,
    .yaw = GEN_BOO_BULLY_DIR,
    .drops = NO_DROPS,
    .animations = BOO_BULLY_ANIMS,
};

NpcSettings NpcSettings_Duplighost = {
    .height = 30,
    .radius = 45,
    .level = ACTOR_LEVEL_NONE,
};

EvtScript EVS_NpcIdle_Duplighost = {
    Loop(0)
        Call(GetSelfVar, 0, LVar0)
        IfEq(LVar0, 1)
            BreakLoop
        EndIf
        Wait(1)
    EndLoop
    Call(StartBossBattle, SONG_SPECIAL_BATTLE)
    Return
    End
};

EvtScript EVS_NpcDefeat_Duplighost = {
    Call(GetBattleOutcome, LVar0)
    Switch(LVar0)
        CaseEq(OUTCOME_PLAYER_WON)
            IfLt(GB_StoryProgress, STORY_MOD_CUTSCENE_5)
                Set(GB_StoryProgress, STORY_MOD_CUTSCENE_5)
                ExecWait(EVS_Cutscene5)
                Call(RemoveNpc, NPC_BooBully_1)
                Call(RemoveNpc, NPC_BooBully_2)
                Call(RemoveNpc, NPC_Duplighost)
            EndIf
        CaseEq(OUTCOME_PLAYER_LOST)
        CaseEq(OUTCOME_PLAYER_FLED)
    EndSwitch
    Return
    End
};

EvtScript EVS_NpcInit_Duplighost = {
    Call(BindNpcIdle, NPC_SELF, Ref(EVS_NpcIdle_Duplighost))
    Call(BindNpcDefeat, NPC_SELF, Ref(EVS_NpcDefeat_Duplighost))
    Return
    End
};

NpcData NpcData_Imposter[] = {
    {
        .id = NPC_DupiOaklie,
        .settings = &N(NpcSettings_Oaklie),
        .pos = { NPC_DISPOSE_LOCATION },
        .flags = ENEMY_FLAG_DO_NOT_KILL | ENEMY_FLAG_FLYING | ENEMY_FLAG_SKIP_BATTLE | ENEMY_FLAG_NO_DROPS,
        // .init = &N(EVS_NpcInit_Guardian),
        .yaw = 0,
        .drops = NO_DROPS,
        .animations = DUPI_OAKLIE_ANIMS,
        .tattle = MSG_NpcTattle_KPA_FakePrincessPeach,
    },
    {
        .id = NPC_Duplighost,
        .settings = &NpcSettings_Duplighost,
        .pos = { NPC_DISPOSE_LOCATION },
        .flags = COMMON_PASSIVE_FLAGS | ENEMY_FLAG_NO_DELAY_AFTER_FLEE | ENEMY_FLAG_ACTIVE_WHILE_OFFSCREEN | ENEMY_FLAG_DO_NOT_AUTO_FACE_PLAYER | ENEMY_FLAG_NO_DROPS,
        .init = &EVS_NpcInit_Duplighost,
        .yaw = 0,
        .drops = NO_DROPS,
        .animations = {
            .idle   = ANIM_Duplighost_Anim02,
            .walk   = ANIM_Duplighost_Anim03,
            .run    = ANIM_Duplighost_Anim04,
            .chase  = ANIM_Duplighost_Anim04,
            .anim_4 = ANIM_Duplighost_Anim02,
            .anim_5 = ANIM_Duplighost_Anim02,
            .death  = ANIM_Duplighost_Anim0A,
            .hit    = ANIM_Duplighost_Anim0A,
            .anim_8 = ANIM_Duplighost_Anim02,
            .anim_9 = ANIM_Duplighost_Anim02,
            .anim_A = ANIM_Duplighost_Anim02,
            .anim_B = ANIM_Duplighost_Anim02,
            .anim_C = ANIM_Duplighost_Anim02,
            .anim_D = ANIM_Duplighost_Anim02,
            .anim_E = ANIM_Duplighost_Anim02,
            .anim_F = ANIM_Duplighost_Anim02,
        },
    },
};

EvtScript EVS_NpcIdle_Oaklie = {
    Return
    End
};

EvtScript EVS_NpcInit_Oaklie = {
    // Call(SetSelfVar, 0, 0)
    // Call(BindNpcIdle, NPC_SELF, Ref(EVS_NpcIdle_Oaklie))
    Return
    End
};

NpcData NpcData_Oaklie = {
    .id = NPC_Oaklie,
    .settings = &N(NpcSettings_Oaklie),
    .pos = { NPC_DISPOSE_LOCATION },
    .flags = ENEMY_FLAG_IGNORE_WORLD_COLLISION | ENEMY_FLAG_IGNORE_PLAYER_COLLISION | ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING | ENEMY_FLAG_SKIP_BATTLE | ENEMY_FLAG_ACTIVE_WHILE_OFFSCREEN | ENEMY_FLAG_IGNORE_TOUCH | ENEMY_FLAG_IGNORE_PARTNER,
    .init = &EVS_NpcInit_Oaklie,
    .yaw = GEN_CHEST_OAKLIE_DIR,
    .drops = NO_DROPS,
    .animations = OAKLIE_ANIMS,
    .tattle = MSG_NpcTattle_Oaklie,
};

NpcGroupList DefaultNPCs = {
    NPC_GROUP(NpcData_FreezyFuzzy, BTL_SFF_FORMATION_02, BTL_SFF_STAGE_00),
    NPC_GROUP(NpcData_PiranhaPlant, BTL_SFF_FORMATION_03, BTL_SFF_STAGE_00),
    NPC_GROUP(NpcData_Beehive, BTL_SFF_FORMATION_07, BTL_SFF_STAGE_01),
    NPC_GROUP(NpcData_BooBully_1),
    NPC_GROUP(NpcData_BooBully_2),
    NPC_GROUP(NpcData_Imposter, BTL_SFF_FORMATION_08, BTL_SFF_STAGE_00),
    NPC_GROUP(NpcData_Oaklie),
    {}
};

}; // namespace mim_08
