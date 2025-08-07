#include "mim_07.h"

namespace mim_07 {

#include "world/common/enemy/BooBully.inc.c"

EvtScript EVS_BooBully_SpookPlayer = {
    Call(SetPlayerAnimation, ANIM_MarioW2_Panic)
    Loop(5)
        Call(ShakeCam, CAM_DEFAULT, 0, 10, Float(0.5))
        Wait(1)
    EndLoop
    Return
    End
};

EvtScript EVS_BooBullyChestScene = {
    WaitSecs(2)
    SetF(LVar0, Float(0.0))
    Call(SetNpcImgFXParams, NPC_BooBully, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
    Call(DisablePlayerInput, true)
    Call(DisablePartnerAI, 0)
    Call(PlayerFaceNpc, NPC_BooBully, 0)
    Call(GetPlayerPos, LVar0, LVar1, LVar2)
    Call(UseSettingsFrom, CAM_DEFAULT, LVar0, LVar1, LVar2)
    Call(SetCamSpeed, CAM_DEFAULT, Float(4.0))
    Call(SetCamDistance, CAM_DEFAULT, 250)
    Call(GetNpcPos, NPC_BooBully, LVar0, LVar1, LVar2)
    Add(LVar1, 40)
    Call(SetPanTarget, CAM_DEFAULT, LVar0, LVar1, LVar2)
    Call(PanToTarget, CAM_DEFAULT, 0, true)
    Wait(1)
    Call(SetPlayerAnimation, ANIM_Mario1_Question)
    WaitSecs(2)
    Call(GetNpcPos, NPC_BooBully, LVar0, LVar1, LVar2)
    Add(LVar1, 40)
    Call(SetNpcPos, NPC_BooBully, LVar0, LVar1, LVar2)
    Call(NpcFacePlayer, NPC_BooBully, 0)
    Call(SetNpcFlagBits, NPC_BooBully, NPC_FLAG_INVISIBLE, false)
    Call(PlaySoundAtNpc, NPC_BooBully, SOUND_BOO_APPEAR_A, SOUND_SPACE_DEFAULT)
    SetF(LVar0, Float(0.0))
    Loop(10)
        AddF(LVar0, Float(24.0))
        Call(SetNpcImgFXParams, NPC_BooBully, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        Wait(1)
    EndLoop
    Call(SetNpcAnimation, NPC_BooBully, ANIM_BooBully_Laugh)
    Call(PlaySoundAtNpc, NPC_BooBully, SOUND_BOO_SPOOK, SOUND_SPACE_DEFAULT)
    ExecWait(EVS_BooBully_SpookPlayer)
    Call(SetNpcVar, NPC_BooBully, 0, 1)
    Call(EnablePartnerAI)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_NpcIdle_BooBully = {
    // Exec(EVS_BooBullyChestScene)
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

EvtScript EVS_NpcDefeat_BooBully = {
    Call(ResetCam, CAM_DEFAULT, 3)
    Call(GetBattleOutcome, LVar0)
    Switch(LVar0)
        CaseEq(OUTCOME_PLAYER_WON)
            Set(GF_MIM07_DefeatedBooBully, true)
            Call(DoNpcDefeat)
        CaseEq(OUTCOME_PLAYER_LOST)
        CaseEq(OUTCOME_PLAYER_FLED)
    EndSwitch
    Return
    End
};

EvtScript EVS_NpcInit_BooBully = {
    IfEq(GF_MIM07_DefeatedBooBully, false)
        Call(SetNpcFlagBits, NPC_SELF, NPC_FLAG_INVISIBLE, true)
        Call(BindNpcIdle, NPC_SELF, Ref(EVS_NpcIdle_BooBully))
        Call(BindNpcDefeat, NPC_SELF, Ref(EVS_NpcDefeat_BooBully))
    Else
        Call(RemoveNpc, NPC_SELF)
    EndIf
    Return
    End
};

NpcData NpcData_BooBully = {
    .id = NPC_BooBully,
    .settings = &N(NpcSettings_BooBully),
    .pos = { -284, -10, 201 },
    .flags = COMMON_PASSIVE_FLAGS | ENEMY_FLAG_NO_DELAY_AFTER_FLEE | ENEMY_FLAG_ACTIVE_WHILE_OFFSCREEN | ENEMY_FLAG_DO_NOT_AUTO_FACE_PLAYER | ENEMY_FLAG_NO_DROPS,
    .init = &EVS_NpcInit_BooBully,
    .yaw = GEN_CHEST_FORMATION2_DIR,
    .drops = NO_DROPS,
    .animations = BOO_BULLY_ANIMS,
};

NpcGroupList DefaultNPCs = {
    NPC_GROUP(NpcData_BooBully, BTL_SFF_FORMATION_01, BTL_SFF_STAGE_00),
    {}
};

}; // namespace mim_07
