#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "model.h"
#include "sprite/npc/Duplighost.h"
#include "fob1_actors.hpp"

namespace battle::actor {

namespace watt_clone {

extern API_CALLABLE(UnkBackgroundFunc3);
extern API_CALLABLE(SetBackgroundAlpha);

API_CALLABLE(UnkBackgroundFunc3) {
    mdl_set_all_tint_type(ENV_TINT_SHROUD);
    *gBackgroundTintModePtr = ENV_TINT_SHROUD;
    mdl_set_shroud_tint_params(0, 0, 0, 0);
    return ApiStatus_DONE2;
}

API_CALLABLE(SetBackgroundAlpha) {
    Bytecode* args = script->ptrReadPos;

    mdl_set_shroud_tint_params(0, 0, 0, evt_get_variable(script, *args++));
    return ApiStatus_DONE2;
}

}; // namespace watt_clone

namespace duplighost {

extern EvtScript EVS_Init;
extern EvtScript EVS_Idle;
extern EvtScript EVS_TakeTurn;
extern EvtScript EVS_HandleEvent;
extern EvtScript EVS_HandlePhase;
extern EvtScript EVS_Duplighost_OnHitElectric;
extern EvtScript EVS_Duplighost_OnShockHit;
extern EvtScript EVS_Duplighost_OnShockDeath;
extern EvtScript EVS_Duplighost_OnDeath;
extern Vec3i DuplighostSummonPos;
Vec3i DuplighostSummonPos = { NPC_DISPOSE_LOCATION };

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
    PRT_SPINY       = 3,
    PRT_ZERO        = 0,
};

enum ActorVars {
    AVAR_Copy_ParentActorID     = 0,
    AVAR_Copy_PartnerLevel      = 1,
    AVAR_Kooper_Toppled         = 3,
    AVAR_Kooper_ToppleTurns     = 4,
    AVAR_State                  = 8,
    AVAL_State_ReadyToCopy      = 0, // will copy partner next turn
    AVAL_State_CopiedPartner    = 1, // currently transformed
    AVAL_State_ReadyToTackle    = 2, // will use flying tackle next turn
    AVAL_State_WaitToTackle     = 3, // do nothing this turn and tackle next turn (unused)
};

// Actor Stats
constexpr s32 hp = 12;
constexpr s32 dmgFlyingLeap = 4;

s32 DefaultAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_Duplighost_Anim02,
    STATUS_KEY_STONE,     ANIM_Duplighost_Anim00,
    STATUS_KEY_SLEEP,     ANIM_Duplighost_Anim0D,
    STATUS_KEY_POISON,    ANIM_Duplighost_Anim02,
    STATUS_KEY_STOP,      ANIM_Duplighost_Anim00,
    STATUS_KEY_STATIC,    ANIM_Duplighost_Anim02,
    STATUS_KEY_PARALYZE,  ANIM_Duplighost_Anim00,
    STATUS_KEY_DIZZY,     ANIM_Duplighost_Anim0E,
    STATUS_KEY_FEAR,      ANIM_Duplighost_Anim0E,
    STATUS_END,
};

s32 FlailingAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_Duplighost_Anim0A,
    STATUS_END,
};

s32 RunAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_Duplighost_Anim04,
    STATUS_END,
};

s32 DefenseTable[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 StatusTable[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,              60,
    STATUS_KEY_POISON,              0,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,              75,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,             75,
    STATUS_KEY_PARALYZE,           75,
    STATUS_KEY_SHRINK,             75,
    STATUS_KEY_STOP,               80,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,         -1,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,         -1,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,       1,
    STATUS_TURN_MOD_SHRINK,         0,
    STATUS_TURN_MOD_STOP,          -1,
    STATUS_END,
};

ActorPartBlueprint ActorParts[] = {
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { -5, 25 },
        .opacity = 255,
        .idleAnimations = DefaultAnims,
        .defenseTable = DefenseTable,
        .eventFlags = 0,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { -2, -10 },
    },
};

EvtScript EVS_Init = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(EVS_TakeTurn))
    Call(BindIdle, ACTOR_SELF, Ref(EVS_Idle))
    Call(BindHandleEvent, ACTOR_SELF, Ref(EVS_HandleEvent))
    Call(BindHandlePhase, ACTOR_SELF, Ref(EVS_HandlePhase))
    Call(SetActorVar, ACTOR_SELF, AVAR_State, AVAL_State_ReadyToCopy)
    Return
    End
};

EvtScript EVS_HandlePhase = {
    Call(GetBattlePhase, LVar0)
    Switch(LVar0)
        CaseEq(PHASE_PLAYER_BEGIN)
        CaseEq(PHASE_ENEMY_BEGIN)
        CaseEq(PHASE_ENEMY_END)
            Call(GetActorVar, ACTOR_SELF, AVAR_State, LVar0)
            IfEq(LVar0, AVAL_State_WaitToTackle)
                Call(SetActorVar, ACTOR_SELF, AVAR_State, AVAL_State_ReadyToTackle)
            EndIf
    EndSwitch
    Return
    End
};

EvtScript EVS_Idle = {
    Return
    End
};

EvtScript EVS_ReturnHome = {
    SetConst(LVar0, PRT_MAIN)
    SetConst(LVar1, ANIM_Duplighost_Anim04)
    ExecWait(EVS_Enemy_ReturnHome)
    Return
    End
};

EvtScript EVS_HandleEvent = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseOrEq(EVENT_HIT_COMBO)
        CaseOrEq(EVENT_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0A)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0C)
            SetConst(LVar2, -1)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0C)
            SetConst(LVar2, -1)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0C)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0A)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0A)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0A)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SHOCK_HIT)
            Call(ResetAllActorSounds, ACTOR_SELF)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0A)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0A)
            ExecWait(EVS_Enemy_Knockback)
            ExecWait(EVS_ReturnHome)
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0A)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0A)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim02)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0A)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0A)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim02)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim04)
            SetConst(LVar2, ANIM_Duplighost_Anim0A)
            ExecWait(EVS_Enemy_ScareAway)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim04)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Duplighost_Anim0A)
            ExecWait(EVS_Enemy_BlowAway)
            Return
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_FlyingTackle = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Duplighost_Anim07)
    Wait(20)
    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP, SOUND_DUPLIGHOST_LEAP, 0)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Duplighost_Anim08)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(AddGoalPos, ACTOR_SELF, -100, 0, 0)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(0.3))
            Call(JumpToGoal, ACTOR_SELF, 17, false, true, false)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(10)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorPos, ACTOR_SELF, LVar0, 0, LVar2)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Duplighost_Anim02)
            Call(ResetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP)
            Wait(15)
            Call(YieldTurn)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
            ExecWait(EVS_ReturnHome)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
            Call(SetActorYaw, ACTOR_SELF, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
    EndSwitch
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Duplighost_Anim08)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetStatusFlags, ACTOR_SELF, LVarA)
    IfFlag(LVarA, STATUS_FLAG_SHRINK)
        Call(AddGoalPos, ACTOR_SELF, Float(4.0), Float(-6.0), 0)
    Else
        Call(AddGoalPos, ACTOR_SELF, 10, -15, 0)
    EndIf
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.3))
    Call(JumpToGoal, ACTOR_SELF, 12, false, true, false)
    Wait(2)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, 0, 0, dmgFlyingLeap, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(ResetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Duplighost_Anim09)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(0.5))
            Add(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, 0, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 20, false, true, false)
            Add(LVar0, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, 0, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 10, false, true, false)
            Wait(10)
            Call(YieldTurn)
            ExecWait(EVS_ReturnHome)
        EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Duplighost_OnDeath = {
    Call(GetActorVar, ACTOR_SELF, AVAR_Copy_ParentActorID, LVar0)
    Call(RemoveActor, LVar0)
    Return
    End
};

EvtScript EVS_Duplighost_OnHitElectric = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(HideHealthBar, ACTOR_SELF)
    Call(SetAnimation, ACTOR_SELF, LVar0, LVar1)
    Wait(30)
    Call(GetActorVar, ACTOR_SELF, AVAR_Copy_ParentActorID, LVarA)
    Call(UseIdleAnimation, LVarA, false)
    Call(HideHealthBar, LVarA)
    Call(CopyStatusEffects, ACTOR_SELF, LVarA)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SMOKE_BURST)
    Thread
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, 3)
        Add(LVar2, 5)
        Loop(3)
            PlayEffect(EFFECT_BIG_SMOKE_PUFF, LVar0, LVar1, LVar2, 0, 0, 0, 0, 0)
            Wait(3)
        EndLoop
    EndThread
    Wait(5)
    Call(GetActorHP, ACTOR_SELF, LVarB)
    Call(SetEnemyHP, LVarA, LVarB)
    Call(CopyBuffs, ACTOR_SELF, LVarA)
    Call(GetActorPos, ACTOR_SELF, LVarB, LVarC, LVarD)
    Call(SetActorPos, LVarA, LVarB, LVarC, LVarD)
    Call(SetPartFlagBits, LVarA, 1, ACTOR_PART_FLAG_PRIMARY_TARGET, true)
    Call(SetPartFlagBits, LVarA, 1, ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_TARGET, false)
    Call(SetActorFlagBits, LVarA, ACTOR_FLAG_NO_SHADOW | ACTOR_FLAG_NO_DMG_APPLY, false)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, true)
    Call(SetActorVar, LVarA, AVAR_State, AVAL_State_ReadyToTackle)
    Call(SetPartFlagBits, ACTOR_SELF, LVar0, ACTOR_PART_FLAG_INVISIBLE, true)
    Call(SetIdleAnimations, LVarA, PRT_MAIN, Ref(FlailingAnims))
    Call(SetAnimation, LVarA, 1, ANIM_Duplighost_Anim0A)
    Wait(30)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(GetActorPos, LVarA, LVarB, LVarC, LVarD)
    IfGt(LVarC, 0)
        Set(LVarC, 0)
        Call(SetActorJumpGravity, LVarA, Float(0.5))
        Call(SetActorSounds, LVarA, ACTOR_SOUND_JUMP, SOUND_FALL_QUICK, 0)
        Call(SetGoalPos, LVarA, LVarB, LVarC, LVarD)
        Call(JumpToGoal, LVarA, 15, false, true, false)
        Call(ResetActorSounds, LVarA, ACTOR_SOUND_JUMP)
        Call(SetGoalPos, LVarA, LVarB, LVarC, LVarD)
        Call(JumpToGoal, LVarA, 10, false, true, false)
        Call(SetGoalPos, LVarA, LVarB, LVarC, LVarD)
        Call(JumpToGoal, LVarA, 5, false, true, false)
    EndIf
    Call(ForceHomePos, LVarA, LVarB, LVarC, LVarD)
    Call(HPBarToHome, LVarA)
    Call(SetIdleAnimations, LVarA, PRT_MAIN, Ref(DefaultAnims))
    Call(SetAnimation, LVarA, 1, ANIM_Duplighost_Anim02)
    Call(SetActorPos, ACTOR_SELF, NPC_DISPOSE_LOCATION)
    Call(ForceHomePos, ACTOR_SELF, NPC_DISPOSE_LOCATION)
    Call(HPBarToHome, ACTOR_SELF)
    Call(RemoveActor, ACTOR_SELF)
    Return
    End
};

EvtScript EVS_Duplighost_OnShockHit = {
    Set(LVar9, LVar0)
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(HideHealthBar, ACTOR_SELF)
    Call(SetAnimation, ACTOR_SELF, LVar0, LVar1)
    Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
    Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
    Call(GetActorPos, ACTOR_SELF, LVar2, LVar3, LVar4)
    Add(LVar2, 10)
    Add(LVar3, 10)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.0))
    Call(SetGoalPos, ACTOR_SELF, LVar2, LVar3, LVar4)
    Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
    ExecWait(EVS_Enemy_ShockHit)
    Call(GetActorVar, ACTOR_SELF, AVAR_Copy_ParentActorID, LVarA)
    Call(UseIdleAnimation, LVarA, false)
    Call(HideHealthBar, LVarA)
    Call(CopyStatusEffects, ACTOR_SELF, LVarA)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SMOKE_BURST)
    Thread
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, 3)
        Add(LVar2, 5)
        Loop(3)
            PlayEffect(EFFECT_BIG_SMOKE_PUFF, LVar0, LVar1, LVar2, 0, 0, 0, 0, 0)
            Wait(3)
        EndLoop
    EndThread
    Wait(5)
    Call(GetActorHP, ACTOR_SELF, LVarB)
    Call(SetEnemyHP, LVarA, LVarB)
    Call(CopyBuffs, ACTOR_SELF, LVarA)
    Call(GetActorPos, ACTOR_SELF, LVarB, LVarC, LVarD)
    Call(SetActorPos, LVarA, LVarB, LVarC, LVarD)
    Call(SetPartFlagBits, LVarA, 1, ACTOR_PART_FLAG_PRIMARY_TARGET, true)
    Call(SetPartFlagBits, LVarA, 1, ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_TARGET, false)
    Call(SetActorFlagBits, LVarA, ACTOR_FLAG_NO_SHADOW | ACTOR_FLAG_NO_DMG_APPLY, false)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, true)
    Call(SetActorVar, LVarA, AVAR_State, AVAL_State_WaitToTackle)
    Call(SetPartFlagBits, ACTOR_SELF, LVar9, ACTOR_PART_FLAG_INVISIBLE, true)
    Call(SetIdleAnimations, LVarA, PRT_MAIN, Ref(FlailingAnims))
    Call(SetAnimation, LVarA, 1, ANIM_Duplighost_Anim0A)
    Wait(15)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(SetActorJumpGravity, LVarA, Float(0.5))
    Call(GetActorPos, LVarA, LVarB, LVarC, LVarD)
    Add(LVarB, 30)
    Set(LVarC, 0)
    Call(SetGoalPos, LVarA, LVarB, LVarC, LVarD)
    Call(JumpToGoal, LVarA, 15, false, true, false)
    Add(LVarB, 20)
    Call(SetGoalPos, LVarA, LVarB, LVarC, LVarD)
    Call(JumpToGoal, LVarA, 10, false, true, false)
    Add(LVarB, 10)
    Call(SetGoalPos, LVarA, LVarB, LVarC, LVarD)
    Call(JumpToGoal, LVarA, 5, false, true, false)
    Wait(20)
    Call(AddActorDecoration, LVarA, 1, 0, ACTOR_DECORATION_SWEAT)
    Call(SetActorYaw, LVarA, 180)
    Call(SetIdleAnimations, LVarA, PRT_MAIN, Ref(RunAnims))
    Call(SetAnimation, LVarA, 1, ANIM_Duplighost_Anim04)
    Call(SetActorSpeed, LVarA, Float(8.0))
    Call(SetGoalToHome, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVarB, LVarC, LVarD)
    Call(SetGoalPos, LVarA, LVarB, 0, LVarD)
    Call(RunToGoal, LVarA, 0, false)
    Call(SetAnimation, LVarA, 1, ANIM_Duplighost_Anim02)
    Call(SetActorYaw, LVarA, 0)
    Call(RemoveActorDecoration, LVarA, 1, 0)
    Call(SetIdleAnimations, LVarA, PRT_MAIN, Ref(DefaultAnims))
    Call(ForceHomePos, LVarA, LVarB, 0, LVarD)
    Call(HPBarToHome, LVarA)
    Call(SetActorPos, ACTOR_SELF, NPC_DISPOSE_LOCATION)
    Call(ForceHomePos, ACTOR_SELF, NPC_DISPOSE_LOCATION)
    Call(HPBarToHome, ACTOR_SELF)
    Call(RemoveActor, ACTOR_SELF)
    Return
    End
};

EvtScript EVS_Duplighost_OnShockDeath = {
    Call(HideHealthBar, ACTOR_SELF)
    Set(LVarA, LVar0)
    Set(LVarB, LVar1)
    Set(LVarC, LVar2)
    Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
    Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
    Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
    Call(SetAnimation, ACTOR_SELF, LVarA, LVarB)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 15)
    Add(LVar1, 10)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.1))
    Call(SetAnimation, ACTOR_SELF, LVarA, LVarB)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 5, false, false, false)
    Set(LVar0, LVarA)
    Set(LVar1, LVarB)
    ExecWait(EVS_Enemy_ShockHit)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Thread
        Call(SetActorRotationOffset, ACTOR_SELF, 0, LVarC, 0)
        Set(LVar0, 0)
        Loop(15)
            Add(LVar0, -48)
            Call(SetActorRotation, ACTOR_SELF, 0, 0, LVar0)
            Wait(1)
        EndLoop
        Call(SetActorRotationOffset, ACTOR_SELF, 0, 0, 0)
    EndThread
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 60)
    Set(LVar1, 0)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.4))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 15, false, true, false)
    Add(LVar0, 20)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 10, false, true, false)
    Add(LVar0, 10)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 4, false, true, false)
    Return
    End
};

extern Formation GoombarioCloneFormation;

Formation GoombarioCloneFormation = {
    ACTOR_BY_POS(GoombarioClone, DuplighostSummonPos, 0),
};

extern Formation KooperCloneFormation;

Formation KooperCloneFormation = {
    ACTOR_BY_POS(KooperClone,DuplighostSummonPos, 0),
};

extern Formation BombetteCloneFormation;

Formation BombetteCloneFormation = {
    ACTOR_BY_POS(BombetteClone, DuplighostSummonPos, 0),
};

extern Formation ParakarryCloneFormation;

Formation ParakarryCloneFormation = {
    ACTOR_BY_POS(ParakarryClone, DuplighostSummonPos, 0),
};

extern Formation BowCloneFormation;

Formation BowCloneFormation = {
    ACTOR_BY_POS(BowClone, DuplighostSummonPos, 0),
};

extern Formation WattCloneFormation;

Formation WattCloneFormation = {
    ACTOR_BY_POS(WattClone, DuplighostSummonPos, 0),
};

extern Formation SushieCloneFormation;

Formation SushieCloneFormation = {
    ACTOR_BY_POS(SushieClone, DuplighostSummonPos, 0),
};

extern Formation LakilesterCloneFormation;

Formation LakilesterCloneFormation = {
    ACTOR_BY_POS(LakilesterClone, DuplighostSummonPos, 0),
};

API_CALLABLE(GetPartnerAndLevel) {
    Bytecode* args = script->ptrReadPos;

    evt_set_variable(script, *args++, gPlayerData.curPartner);
    evt_set_variable(script, *args++, gPlayerData.partners[gPlayerData.curPartner].level);
    return ApiStatus_DONE2;
}

API_CALLABLE(AdjustFormationPriority) {
    s32 partnerID = evt_get_variable(script, *script->ptrReadPos);
    Actor* actor = get_actor(script->owner1.actorID);
    FormationRow* formation = NULL;

    switch (partnerID) {
        case PARTNER_GOOMBARIO:
            formation = GoombarioCloneFormation;
            break;
        case PARTNER_KOOPER:
            formation = KooperCloneFormation;
            break;
        case PARTNER_BOMBETTE:
            formation = BombetteCloneFormation;
            break;
        case PARTNER_PARAKARRY:
            formation = ParakarryCloneFormation;
            break;
        case PARTNER_BOW:
            formation = BowCloneFormation;
            break;
        case PARTNER_WATT:
            formation = WattCloneFormation;
            break;
        case PARTNER_SUSHIE:
            formation = SushieCloneFormation;
            break;
        case PARTNER_LAKILESTER:
            formation = LakilesterCloneFormation;
            break;
    }

    formation->priority = actor->turnPriority;

    return ApiStatus_DONE2;
}

EvtScript EVS_CopyPartner = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(UseBattleCamPreset, BTL_CAM_ACTOR)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Wait(15)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_GHOST_TRANSFORM)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Duplighost_Anim06)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(GetStatusFlags, ACTOR_SELF, LVarA)
    IfFlag(LVarA, STATUS_FLAG_SHRINK)
        Add(LVar1, 8)
        SetF(LVar3, Float(0.4))
    Else
        Add(LVar1, 20)
        SetF(LVar3, Float(1.0))
    EndIf
    PlayEffect(EFFECT_GATHER_ENERGY_PINK, 1, LVar0, LVar1, LVar2, LVar3, 40, 0)
    Call(watt_clone::UnkBackgroundFunc3)
    Call(MakeLerp, 0, 200, 20, EASING_LINEAR)
    Label(0)
    Call(UpdateLerp)
    Call(watt_clone::SetBackgroundAlpha, LVar0)
    Wait(1)
    IfEq(LVar1, 1)
        Goto(0)
    EndIf
    Wait(10)
    Call(GetPartnerAndLevel, LVar5, LVar6)
    Call(AdjustFormationPriority, LVar5)
    Switch(LVar5)
        CaseEq(PARTNER_GOOMBARIO)
            Call(SummonEnemy, Ref(GoombarioCloneFormation), false)
        CaseEq(PARTNER_KOOPER)
            Call(SummonEnemy, Ref(KooperCloneFormation), false)
        CaseEq(PARTNER_BOMBETTE)
            Call(SummonEnemy, Ref(BombetteCloneFormation), false)
        CaseEq(PARTNER_PARAKARRY)
            Call(SummonEnemy, Ref(ParakarryCloneFormation), false)
        CaseEq(PARTNER_BOW)
            Call(SummonEnemy, Ref(BowCloneFormation), false)
        CaseEq(PARTNER_WATT)
            Call(SummonEnemy, Ref(WattCloneFormation), false)
        CaseEq(PARTNER_SUSHIE)
            Call(SummonEnemy, Ref(SushieCloneFormation), false)
        CaseEq(PARTNER_LAKILESTER)
            Call(SummonEnemy, Ref(LakilesterCloneFormation), false)
    EndSwitch
    Set(LVarA, LVar0)
    Call(CopyStatusEffects, ACTOR_SELF, LVarA)
    Call(SetBattleVar, BTL_VAR_LastCopiedPartner, LVar5)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SMOKE_BURST)
    Thread
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, 3)
        Add(LVar2, 5)
        Loop(3)
            PlayEffect(EFFECT_BIG_SMOKE_PUFF, LVar0, LVar1, LVar2, 0, 0, 0, 0, 0)
            Wait(3)
        EndLoop
    EndThread
    Wait(5)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetActorPos, LVarA, LVar0, LVar1, LVar2)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_MAIN, ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_TARGET, true)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_MAIN, ACTOR_PART_FLAG_PRIMARY_TARGET, false)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW | ACTOR_FLAG_NO_DMG_APPLY, true)
    Call(GetActorHP, ACTOR_SELF, LVar0)
    Call(SetEnemyHP, LVarA, LVar0)
    Call(CopyBuffs, ACTOR_SELF, LVarA)
    Call(GetOwnerID, LVar0)
    Call(SetActorVar, LVarA, AVAR_Copy_ParentActorID, LVar0)
    Call(SetActorVar, LVarA, AVAR_Copy_PartnerLevel, LVar6)
    Wait(20)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 20)
    Thread
        Call(MakeLerp, 200, 0, 20, EASING_LINEAR)
        Label(1)
        Call(UpdateLerp)
        Call(watt_clone::SetBackgroundAlpha, LVar0)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(1)
        EndIf
    EndThread
    Call(SetActorSounds, LVarA, ACTOR_SOUND_JUMP, SOUND_NONE, 0)
    Switch(LVar5)
        CaseEq(8)
            Call(GetActorPos, LVarA, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, LVarA, Float(0.01))
            Call(SetGoalPos, LVarA, LVar0, 10, LVar2)
            Call(JumpToGoal, LVarA, 10, false, false, false)
            Wait(10)
        CaseOrEq(6)
        CaseOrEq(9)
        CaseOrEq(4)
            Call(GetActorPos, LVarA, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, LVarA, Float(0.01))
            Call(SetGoalPos, LVarA, LVar0, 30, LVar2)
            Call(JumpToGoal, LVarA, 20, false, false, false)
        EndCaseGroup
    EndSwitch
    Call(GetActorPos, LVarA, LVar0, LVar1, LVar2)
    Call(ForceHomePos, LVarA, LVar0, LVar1, LVar2)
    Call(HPBarToHome, LVarA)
    Call(ResetActorSounds, LVarA, ACTOR_SOUND_JUMP)
    Wait(20)
    Call(SetActorPos, ACTOR_SELF, NPC_DISPOSE_LOCATION)
    Call(ForceHomePos, ACTOR_SELF, NPC_DISPOSE_LOCATION)
    Call(HPBarToHome, ACTOR_SELF)
    Call(SetActorVar, ACTOR_SELF, AVAR_State, AVAL_State_CopiedPartner)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_TakeTurn = {
    Call(GetActorVar, ACTOR_SELF, AVAR_State, LVar0)
    Switch(LVar0)
        CaseEq(AVAL_State_ReadyToCopy)
            Call(RandInt, 1000, LVar0)
            IfLt(LVar0, 600)
                Call(GetBattleVar, BTL_VAR_DuplighostCopyFlags, LVar0)
                IfNotFlag(LVar0, BTL_VAL_Duplighost_HasCopied)
                    // first time partner is copied, set battle flag and proceed
                    Call(GetBattleVar, BTL_VAR_DuplighostCopyFlags, LVar0)
                    BitwiseOrConst(LVar0, BTL_VAL_Duplighost_HasCopied)
                    Call(SetBattleVar, BTL_VAR_DuplighostCopyFlags, LVar0)
                    ExecWait(EVS_CopyPartner)
                Else
                    // partner has been copied before, try to avoid copying the same one
                    Call(GetBattleVar, BTL_VAR_LastCopiedPartner, LVar0)
                    Call(GetPartnerAndLevel, LVar1, LVar2)
                    IfEq(LVar0, LVar1)
                        ExecWait(EVS_CopyPartner)
                    Else
                        ExecWait(EVS_Attack_FlyingTackle)
                    EndIf
                EndIf
            Else
                ExecWait(EVS_Attack_FlyingTackle)
            EndIf
        CaseEq(AVAL_State_CopiedPartner)
            // do nothing, currently in disguise
        CaseEq(AVAL_State_ReadyToTackle)
            ExecWait(EVS_Attack_FlyingTackle)
        CaseEq(AVAL_State_WaitToTackle)
    EndSwitch
    Return
    End
};

}; // namespace duplighost

namespace goombario_clone {

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
    PRT_SPINY       = 3,
    PRT_ZERO        = 0,
};

enum ActorVars {
    AVAR_Copy_ParentActorID     = 0,
    AVAR_Copy_PartnerLevel      = 1,
    AVAR_Kooper_Toppled         = 3,
    AVAR_Kooper_ToppleTurns     = 4,
    AVAR_State                  = 8,
    AVAL_State_ReadyToCopy      = 0, // will copy partner next turn
    AVAL_State_CopiedPartner    = 1, // currently transformed
    AVAL_State_ReadyToTackle    = 2, // will use flying tackle next turn
    AVAL_State_WaitToTackle     = 3, // do nothing this turn and tackle next turn (unused)
};

#include "duplighost/ghost_goombario.inc.cpp"

}; // namespace goombario_clone

namespace kooper_clone {

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
    PRT_SPINY       = 3,
    PRT_ZERO        = 0,
};

enum ActorVars {
    AVAR_Copy_ParentActorID     = 0,
    AVAR_Copy_PartnerLevel      = 1,
    AVAR_Kooper_Toppled         = 3,
    AVAR_Kooper_ToppleTurns     = 4,
    AVAR_State                  = 8,
    AVAL_State_ReadyToCopy      = 0, // will copy partner next turn
    AVAL_State_CopiedPartner    = 1, // currently transformed
    AVAL_State_ReadyToTackle    = 2, // will use flying tackle next turn
    AVAL_State_WaitToTackle     = 3, // do nothing this turn and tackle next turn (unused)
};

#include "duplighost/ghost_kooper.inc.cpp"

}; // namespace kooper_clone

namespace bombette_clone {

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
    PRT_SPINY       = 3,
    PRT_ZERO        = 0,
};

enum ActorVars {
    AVAR_Copy_ParentActorID     = 0,
    AVAR_Copy_PartnerLevel      = 1,
    AVAR_Kooper_Toppled         = 3,
    AVAR_Kooper_ToppleTurns     = 4,
    AVAR_State                  = 8,
    AVAL_State_ReadyToCopy      = 0, // will copy partner next turn
    AVAL_State_CopiedPartner    = 1, // currently transformed
    AVAL_State_ReadyToTackle    = 2, // will use flying tackle next turn
    AVAL_State_WaitToTackle     = 3, // do nothing this turn and tackle next turn (unused)
};

#include "duplighost/ghost_bombette.inc.cpp"

}; // namespace bombette_clone

namespace parakarry_clone {

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
    PRT_SPINY       = 3,
    PRT_ZERO        = 0,
};

enum ActorVars {
    AVAR_Copy_ParentActorID     = 0,
    AVAR_Copy_PartnerLevel      = 1,
    AVAR_Kooper_Toppled         = 3,
    AVAR_Kooper_ToppleTurns     = 4,
    AVAR_State                  = 8,
    AVAL_State_ReadyToCopy      = 0, // will copy partner next turn
    AVAL_State_CopiedPartner    = 1, // currently transformed
    AVAL_State_ReadyToTackle    = 2, // will use flying tackle next turn
    AVAL_State_WaitToTackle     = 3, // do nothing this turn and tackle next turn (unused)
};

#include "duplighost/ghost_parakarry.inc.cpp"

}; // namespace parakarry_clone

namespace bow_clone {

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
    PRT_SPINY       = 3,
    PRT_ZERO        = 0,
};

enum ActorVars {
    AVAR_Copy_ParentActorID     = 0,
    AVAR_Copy_PartnerLevel      = 1,
    AVAR_Kooper_Toppled         = 3,
    AVAR_Kooper_ToppleTurns     = 4,
    AVAR_State                  = 8,
    AVAL_State_ReadyToCopy      = 0, // will copy partner next turn
    AVAL_State_CopiedPartner    = 1, // currently transformed
    AVAL_State_ReadyToTackle    = 2, // will use flying tackle next turn
    AVAL_State_WaitToTackle     = 3, // do nothing this turn and tackle next turn (unused)
};

#include "duplighost/ghost_bow.inc.cpp"

}; // namespace bow_cline

namespace watt_clone {

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
    PRT_SPINY       = 3,
    PRT_ZERO        = 0,
};

enum ActorVars {
    AVAR_Copy_ParentActorID     = 0,
    AVAR_Copy_PartnerLevel      = 1,
    AVAR_Kooper_Toppled         = 3,
    AVAR_Kooper_ToppleTurns     = 4,
    AVAR_State                  = 8,
    AVAL_State_ReadyToCopy      = 0, // will copy partner next turn
    AVAL_State_CopiedPartner    = 1, // currently transformed
    AVAL_State_ReadyToTackle    = 2, // will use flying tackle next turn
    AVAL_State_WaitToTackle     = 3, // do nothing this turn and tackle next turn (unused)
};

#include "duplighost/ghost_watt.inc.cpp"

}; // namespace watt_clone

namespace sushie_clone {

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
    PRT_SPINY       = 3,
    PRT_ZERO        = 0,
};

enum ActorVars {
    AVAR_Copy_ParentActorID     = 0,
    AVAR_Copy_PartnerLevel      = 1,
    AVAR_Kooper_Toppled         = 3,
    AVAR_Kooper_ToppleTurns     = 4,
    AVAR_State                  = 8,
    AVAL_State_ReadyToCopy      = 0, // will copy partner next turn
    AVAL_State_CopiedPartner    = 1, // currently transformed
    AVAL_State_ReadyToTackle    = 2, // will use flying tackle next turn
    AVAL_State_WaitToTackle     = 3, // do nothing this turn and tackle next turn (unused)
};

#include "duplighost/ghost_sushie.inc.cpp"

}; // namespace sushie_clone

namespace lakilester_clone {

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
    PRT_SPINY       = 3,
    PRT_ZERO        = 0,
};

enum ActorVars {
    AVAR_Copy_ParentActorID     = 0,
    AVAR_Copy_PartnerLevel      = 1,
    AVAR_Kooper_Toppled         = 3,
    AVAR_Kooper_ToppleTurns     = 4,
    AVAR_State                  = 8,
    AVAL_State_ReadyToCopy      = 0, // will copy partner next turn
    AVAL_State_CopiedPartner    = 1, // currently transformed
    AVAL_State_ReadyToTackle    = 2, // will use flying tackle next turn
    AVAL_State_WaitToTackle     = 3, // do nothing this turn and tackle next turn (unused)
};

#include "duplighost/ghost_lakilester.inc.cpp"

}; // namespace lakilester_clone

ActorBlueprint GoombarioClone = {
    .flags = 0,
    .maxHP = goombario_clone::hp,
    .type = ACTOR_TYPE_GHOST_GOOMBARIO,
    .level = ACTOR_LEVEL_GHOST_GOOMBARIO,
    .partCount = ARRAY_COUNT(goombario_clone::ActorParts),
    .partsData = goombario_clone::ActorParts,
    .initScript = &goombario_clone::EVS_Init,
    .statusTable = goombario_clone::StatusTable,
    .escapeChance = 50,
    .airLiftChance = 80,
    .hurricaneChance = 70,
    .spookChance = 50,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 90,
    .coinReward = 2,
    .size = { 29, 26 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

ActorBlueprint KooperClone = {
    .flags = 0,
    .maxHP = kooper_clone::hp,
    .type = ACTOR_TYPE_GHOST_KOOPER,
    .level = ACTOR_LEVEL_GHOST_KOOPER,
    .partCount = ARRAY_COUNT(kooper_clone::ActorParts),
    .partsData = kooper_clone::ActorParts,
    .initScript = &kooper_clone::EVS_Init,
    .statusTable = kooper_clone::StatusTable,
    .escapeChance = 50,
    .airLiftChance = 80,
    .hurricaneChance = 70,
    .spookChance = 50,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 90,
    .coinReward = 2,
    .size = { 32, 38 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

ActorBlueprint BombetteClone = {
    .flags = 0,
    .maxHP = bombette_clone::hp,
    .type = ACTOR_TYPE_GHOST_BOMBETTE,
    .level = ACTOR_LEVEL_GHOST_BOMBETTE,
    .partCount = ARRAY_COUNT(bombette_clone::ActorParts),
    .partsData = bombette_clone::ActorParts,
    .initScript = &bombette_clone::EVS_Init,
    .statusTable = bombette_clone::StatusTable,
    .escapeChance = 50,
    .airLiftChance = 80,
    .hurricaneChance = 70,
    .spookChance = 50,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 90,
    .coinReward = 2,
    .size = { 30, 28 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

ActorBlueprint ParakarryClone = {
    .flags = ACTOR_FLAG_FLYING,
    .maxHP = parakarry_clone::hp,
    .type = ACTOR_TYPE_GHOST_PARAKARRY,
    .level = ACTOR_LEVEL_GHOST_PARAKARRY,
    .partCount = ARRAY_COUNT(parakarry_clone::ActorParts),
    .partsData = parakarry_clone::ActorParts,
    .initScript = &parakarry_clone::EVS_Init,
    .statusTable = parakarry_clone::StatusTable,
    .escapeChance = 50,
    .airLiftChance = 80,
    .hurricaneChance = 70,
    .spookChance = 50,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 90,
    .coinReward = 2,
    .size = { 38, 38 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

ActorBlueprint BowClone = {
    .flags = ACTOR_FLAG_FLYING,
    .maxHP = bow_clone::hp,
    .type = ACTOR_TYPE_GHOST_BOW,
    .level = ACTOR_LEVEL_GHOST_BOW,
    .partCount = ARRAY_COUNT(bow_clone::ActorParts),
    .partsData = bow_clone::ActorParts,
    .initScript = &bow_clone::EVS_Init,
    .statusTable = bow_clone::StatusTable,
    .escapeChance = 50,
    .airLiftChance = 80,
    .hurricaneChance = 70,
    .spookChance = 50,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 90,
    .coinReward = 2,
    .size = { 36, 29 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

ActorBlueprint WattClone = {
    .flags = ACTOR_FLAG_FLYING,
    .maxHP = watt_clone::hp,
    .type = ACTOR_TYPE_GHOST_WATT,
    .level = ACTOR_LEVEL_GHOST_WATT,
    .partCount = ARRAY_COUNT(watt_clone::ActorParts),
    .partsData = watt_clone::ActorParts,
    .initScript = &watt_clone::EVS_Init,
    .statusTable = watt_clone::StatusTable,
    .escapeChance = 50,
    .airLiftChance = 80,
    .hurricaneChance = 70,
    .spookChance = 50,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 90,
    .coinReward = 2,
    .size = { 34, 28 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

ActorBlueprint SushieClone = {
    .flags = 0,
    .maxHP = sushie_clone::hp,
    .type = ACTOR_TYPE_GHOST_SUSHIE,
    .level = ACTOR_LEVEL_GHOST_SUSHIE,
    .partCount = ARRAY_COUNT(sushie_clone::ActorParts),
    .partsData = sushie_clone::ActorParts,
    .initScript = &sushie_clone::EVS_Init,
    .statusTable = sushie_clone::StatusTable,
    .escapeChance = 50,
    .airLiftChance = 80,
    .hurricaneChance = 70,
    .spookChance = 50,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 90,
    .coinReward = 2,
    .size = { 37, 26 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

ActorBlueprint LakilesterClone = {
    .flags = ACTOR_FLAG_FLYING,
    .maxHP = lakilester_clone::hp,
    .type = ACTOR_TYPE_GHOST_LAKILESTER,
    .level = ACTOR_LEVEL_GHOST_LAKILESTER,
    .partCount = ARRAY_COUNT(lakilester_clone::ActorParts),
    .partsData = lakilester_clone::ActorParts,
    .initScript = &lakilester_clone::EVS_Init,
    .statusTable = lakilester_clone::StatusTable,
    .escapeChance = 50,
    .airLiftChance = 80,
    .hurricaneChance = 70,
    .spookChance = 50,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 90,
    .coinReward = 2,
    .size = { 44, 40 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

ActorBlueprint Duplighost = {
    .flags = 0,
    .maxHP = duplighost::hp,
    .type = ACTOR_TYPE_DUPLIGHOST,
    .level = ACTOR_LEVEL_DUPLIGHOST,
    .partCount = ARRAY_COUNT(duplighost::ActorParts),
    .partsData = duplighost::ActorParts,
    .initScript = &duplighost::EVS_Init,
    .statusTable = duplighost::StatusTable,
    .escapeChance = 50,
    .airLiftChance = 80,
    .hurricaneChance = 70,
    .spookChance = 50,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 90,
    .coinReward = 2,
    .size = { 36, 36 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

}; // namespace battle::actor
