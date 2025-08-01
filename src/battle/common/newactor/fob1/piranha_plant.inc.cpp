#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/SmallPiranha.h"
#include "fob1_actors.hpp"

namespace battle::actor {

namespace piranha_plant {

extern EvtScript EVS_Init;
extern EvtScript EVS_Idle;
extern EvtScript EVS_TakeTurn;
extern EvtScript EVS_HandleEvent;
extern EvtScript EVS_ReturnHome;
extern EvtScript EVS_Attack_Bite;
// extern EvtScript N(EVS_Attack_DazeBreath);
// extern EvtScript EVS_RiseFromPipe;
// extern EvtScript N(EVS_Move_BoostAttack);

enum ActorPartIDs {
    PRT_MAIN        = 1,
};

// Actor Stats
constexpr s32 hp = 7;
constexpr s32 dmgBite = 3;

s32 DefaultAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_SmallPiranha_Anim01,
    STATUS_KEY_STONE,     ANIM_SmallPiranha_Anim00,
    STATUS_KEY_SLEEP,     ANIM_SmallPiranha_Anim00,
    STATUS_KEY_POISON,    ANIM_SmallPiranha_Anim01,
    STATUS_KEY_STOP,      ANIM_SmallPiranha_Anim00,
    STATUS_KEY_STATIC,    ANIM_SmallPiranha_Anim01,
    STATUS_KEY_PARALYZE,  ANIM_SmallPiranha_Anim00,
    STATUS_KEY_DIZZY,     ANIM_SmallPiranha_Anim01,
    STATUS_KEY_FEAR,      ANIM_SmallPiranha_Anim01,
    STATUS_END,
};

s32 UnusedAnims1[] = {
    STATUS_KEY_NORMAL,    ANIM_SmallPiranha_Anim01,
    STATUS_END,
};

s32 UnusedAnims2[] = {
    STATUS_KEY_NORMAL,    ANIM_SmallPiranha_Anim06,
    STATUS_END,
};

s32 DefenseTable[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 StatusTable[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,              95,
    STATUS_KEY_POISON,              0,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,              50,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,              0,
    STATUS_KEY_PARALYZE,          100,
    STATUS_KEY_SHRINK,             80,
    STATUS_KEY_STOP,               90,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          1,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,          0,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,       0,
    STATUS_TURN_MOD_SHRINK,         0,
    STATUS_TURN_MOD_STOP,           0,
    STATUS_END,
};

ActorPartBlueprint ActorParts[] = {
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 35 },
        .opacity = 255,
        .idleAnimations = DefaultAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAG_SPIKY_TOP,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
};

EvtScript EVS_Init = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(EVS_TakeTurn))
    Call(BindIdle, ACTOR_SELF, Ref(EVS_Idle))
    Call(BindHandleEvent, ACTOR_SELF, Ref(EVS_HandleEvent))
    // Exec(EVS_RiseFromPipe)
    Return
    End
};

EvtScript EVS_RiseFromPipe = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetActorPos, ACTOR_SELF, 42, -45, 0)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_HEALTH_BAR, true)
    Wait(60)
    Call(MakeLerp, -45, 25, 35, EASING_LINEAR)
    Loop(0)
        Call(UpdateLerp)
        IfEq(LVarE, 25)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim01)
        EndIf
        Call(SetActorPos, ACTOR_SELF, 42, LVar0, 0)
        Wait(1)
        IfEq(LVar1, 0)
            BreakLoop
        EndIf
    EndLoop
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_HEALTH_BAR, false)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Return
    End
};


EvtScript EVS_Idle = {
    Return
    End
};

EvtScript EVS_ReturnHome = {
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 20)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, true)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_DIG)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim04)
    Wait(10)
    Call(SetGoalToHome, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_SURFACE)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim03)
    Wait(10)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, false)
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
            SetConst(LVar1, ANIM_SmallPiranha_Anim09)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim0A)
            SetConst(LVar2, ANIM_SmallPiranha_Anim0B)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim0A)
            SetConst(LVar2, ANIM_SmallPiranha_Anim0B)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim0B)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim09)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim09)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim09)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim09)
            ExecWait(EVS_Enemy_ShockHit)
            Call(SetActorSpeed, ACTOR_SELF, Float(4.0))
            ExecWait(EVS_ReturnHome)
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim09)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim09)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim01)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_SPIKE_TAUNT)
            Thread
                Wait(10)
                Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                Call(GetStatusFlags, ACTOR_SELF, LVar3)
                IfFlag(LVar3, STATUS_FLAG_SHRINK)
                    Add(LVar0, 0)
                    Add(LVar1, 14)
                Else
                    Add(LVar0, 0)
                    Add(LVar1, 35)
                EndIf
                PlayEffect(EFFECT_LENS_FLARE, 0, LVar0, LVar1, LVar2, 20, 0)
                Wait(20)
            EndThread
            Loop(40)
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim01)
                Wait(1)
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim06)
                Wait(1)
            EndLoop
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim01)
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim09)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim09)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim01)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim09)
            ExecWait(EVS_Enemy_Hit)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_DIG)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim04)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, true)
            Wait(8)
            Call(HideHealthBar, ACTOR_SELF)
            Call(UseIdleAnimation, ACTOR_SELF, false)
            Call(RemoveActor, ACTOR_SELF)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim09)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_SmallPiranha_Anim09)
            ExecWait(EVS_Enemy_BlowAway)
            Return
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_TakeTurn = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    /*
    Call(RandInt, 100, LVar0)
    Switch(LVar0)
        CaseLt(50)
            Set(LVar1, 0)
        CaseDefault
            Set(LVar1, 1)
    EndSwitch
    Set(LVar1, 0)
    Switch(LVar1)
        CaseEq(0)
            ExecWait(EVS_Attack_Bite)
        CaseEq(1)
            ExecWait(N(EVS_Attack_DazeBreath))
    EndSwitch
    */
    // ExecWait(N(EVS_Move_BoostAttack))
    ExecWait(EVS_Attack_Bite)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_Bite = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(GetBattlePhase, LVar0)
    IfEq(LVar0, PHASE_FIRST_STRIKE)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim04)
        Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, true)
        Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
        Call(BattleCamTargetActor, ACTOR_SELF)
        Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
        Call(MoveBattleCamOver, 1)
        Call(SetGoalToTarget, ACTOR_SELF)
        Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar0, 20)
        Set(LVar1, 0)
        Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Wait(15)
        Goto(123)
    EndIf
    Wait(30)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_DIG)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim04)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, true)
    Wait(8)
    Call(UseBattleCamPreset, BTL_CAM_REPOSITION)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 20)
    Set(LVar1, 0)
    Call(SetBattleCamTarget, LVar0, LVar1, LVar2)
    Call(SetBattleCamDist, 350)
    Call(SetBattleCamOffsetY, 45)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
    Call(MoveBattleCamOver, 20)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(GetStatusFlags, ACTOR_SELF, LVar5)
    IfFlag(LVar5, STATUS_FLAG_SHRINK)
        Add(LVar0, 8)
        Add(LVar0, -3)
    Else
        Add(LVar0, 20)
    EndIf
    Set(LVar1, 0)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.0))
    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP, SOUND_NONE, 0)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 20, false, false, false)
    Label(123)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_SURFACE)
    Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, false)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim03)
    Wait(8)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim05)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(GetStatusFlags, ACTOR_SELF, LVar5)
            IfNotFlag(LVar5, STATUS_FLAG_SHRINK)
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim06)
                Wait(2)
            EndIf
            Thread
                Wait(6)
                Call(PlaySoundAtActor, ACTOR_SELF, SOUND_PIRANHA_BITE)
            EndThread
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim07)
            Wait(10)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim08)
            Wait(2)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(15)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(YieldTurn)
            Call(SetActorSpeed, ACTOR_SELF, Float(4.0))
            ExecWait(EVS_ReturnHome)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
    EndSwitch
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim06)
    Wait(2)
    Thread
        Wait(6)
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_PIRANHA_BITE)
    EndThread
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim07)
    Wait(10)
    Wait(2)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, 0, 0, dmgBite, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Wait(2)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim08)
            Wait(15)
            Call(YieldTurn)
            Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
            ExecWait(EVS_ReturnHome)
            Wait(30)
        EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

/*
#include "common/SetBowserFireBreathScales.inc.c"

EvtScript N(EVS_Attack_DazeBreath) = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Wait(15)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_DIG)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim04)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, true)
    Wait(8)
    Call(UseBattleCamPreset, BTL_CAM_REPOSITION)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 20)
    Set(LVar1, 0)
    Call(SetBattleCamTarget, LVar0, LVar1, LVar2)
    Call(SetBattleCamDist, 350)
    Call(SetBattleCamOffsetY, 45)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
    Call(MoveBattleCamOver, 20)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(GetStatusFlags, ACTOR_SELF, LVar5)
    IfFlag(LVar5, STATUS_FLAG_SHRINK)
        Add(LVar0, 8)
        Add(LVar0, -3)
    Else
        Add(LVar0, 20)
    EndIf
    Set(LVar1, 0)
    Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.0))
    Call(SetActorSounds, ACTOR_SELF, ACTOR_SOUND_JUMP, SOUND_NONE, 0)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 100)
    Set(LVar1, 0)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 20, false, false, false)
    Label(123)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BURROW_SURFACE)
    Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, false)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim03)
    Wait(8)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim05)
    Wait(2)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim07)
    Wait(10)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim06) // breath start
    Wait(27)
    Call(SetBattleCamTarget, LVar0, LVar1, LVar2)
    Call(SetBattleCamDist, 400)
    Call(MoveBattleCamOver, 40)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim07) // breath
    Call(UseBattleCamPreset, BTL_CAM_MIDPOINT_FAR)
    Call(MoveBattleCamOver, 55)
    Add(LVar2, 20)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar3, LVar4, -100)
    Wait(5)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar1, 15)
    Sub(LVar3, 40)
    Sub(LVar4, 0)
    Sub(LVar5, 5)
    PlayEffect(EFFECT_FIRE_BREATH, FIRE_BREATH_TINY, LVar0, LVar1, LVar2, LVar3, LVar4, LVar5, 25, 1, 8, DAZE_BREATH)
    Call(N(SetBowserFireBreathScales), LVarF)
    Thread
        // Call(N(StartRumbleWithParams), 50, 148)
        Call(PlaySound, SOUND_BOWSER_FIRE_BREATH_LOOP)
        Wait(70)
        Call(PlaySound, SOUND_BOWSER_FIRE_BREATH_LOOP | SOUND_ID_TRIGGER_CHANGE_SOUND)
    EndThread
    Wait(10)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(GetStatusFlags, ACTOR_SELF, LVar5)
            IfNotFlag(LVar5, STATUS_FLAG_SHRINK)
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim06)
                Wait(2)
            EndIf
            Thread
                Wait(6)
                Call(PlaySoundAtActor, ACTOR_SELF, SOUND_PIRANHA_BITE)
            EndThread
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim08) // breath end
            Wait(40)
            Wait(2)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(15)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(YieldTurn)
            Call(SetActorSpeed, ACTOR_SELF, Float(4.0))
            ExecWait(EVS_ReturnHome)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
    EndSwitch
    Wait(2)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_NO_CONTACT, SUPPRESS_EVENT_ALL, DMG_STATUS_KEY(STATUS_FLAG_DIZZY, 2, 100), DMG_DAZE_BREATH, BS_FLAGS1_TRIGGER_EVENTS)
        Switch(LVarF)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Wait(2)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_SmallPiranha_Anim08)
            Wait(15)
            Call(YieldTurn)
            Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
            ExecWait(EVS_ReturnHome)
        EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};
*/

// EvtScript N(EVS_Move_BoostAttack) = {
//     Call(UseIdleAnimation, ACTOR_SELF, false)
//     Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
//     Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
//     Call(UseBattleCamPreset, BTL_CAM_ACTOR)
//     Call(BattleCamTargetActor, ACTOR_SELF)
//     Call(MoveBattleCamOver, 15)
//     Wait(15)
//     Thread
//         Wait(10)
//         Call(PlaySoundAtActor, LVar8, SOUND_MAGIKOOPA_POWER_UP)
//     EndThread
//     Thread
//         Call(FreezeBattleState, true)
//         Call(BoostDefense, ACTOR_SELF, 1)
//         Call(FreezeBattleState, false)
//     EndThread
//     Call(WaitForBuffDone)
//     Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
//     Wait(10)
//     Call(YieldTurn)
//     Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
//     Call(UseIdleAnimation, ACTOR_SELF, true)
//     Return
//     End
// };

}; // namespace piranha_plant

ActorBlueprint PiranhaPlant = {
    .flags = 0, // ACTOR_FLAG_NO_SHADOW,
    .maxHP = piranha_plant::hp,
    .type = ACTOR_TYPE_PIRANHA_PLANT,
    .level = ACTOR_LEVEL_PIRANHA_PLANT,
    .partCount = ARRAY_COUNT(piranha_plant::ActorParts),
    .partsData = piranha_plant::ActorParts,
    .initScript = &piranha_plant::EVS_Init,
    .statusTable = piranha_plant::StatusTable,
    .escapeChance = 70,
    .airLiftChance = 20,
    .hurricaneChance = 20,
    .spookChance = 40,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 90,
    .coinReward = 1,
    .size = { 38, 38 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -7, 33 },
    .statusTextOffset = { 10, 33 },
};

}; // namespace battle::actor
