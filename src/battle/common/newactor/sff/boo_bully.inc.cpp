#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/BooBully.h"
#include "sff_actors.hpp"

namespace battle::actor {

namespace boo_bully {

extern EvtScript EVS_Init;
extern s32 DefaultAnims[];
extern EvtScript EVS_Idle;
extern EvtScript EVS_TakeTurn;
extern EvtScript EVS_HandleEvent;
extern EvtScript EVS_ReturnHome;
extern EvtScript EVS_Attack_Scare;
extern EvtScript EVS_Move_PowerTransfer;

enum ActorPartIDs {
    PRT_MAIN        = 1,
};

// Actor Stats
constexpr s32 hp = 10;
constexpr s32 dmgScare = 2;

s32 DefaultAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_BooBully_Idle,
    STATUS_KEY_STONE,     ANIM_BooBully_Still,
    STATUS_KEY_SLEEP,     ANIM_BooBully_Idle,
    STATUS_KEY_POISON,    ANIM_BooBully_Idle,
    STATUS_KEY_STOP,      ANIM_BooBully_Still,
    STATUS_KEY_STATIC,    ANIM_BooBully_Idle,
    STATUS_KEY_PARALYZE,  ANIM_BooBully_Still,
    STATUS_KEY_DIZZY,     ANIM_BooBully_Idle,
    STATUS_KEY_FEAR,      ANIM_BooBully_Idle,
    STATUS_END,
};

s32 DefenseTable[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 StatusTable[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,               0,
    STATUS_KEY_POISON,              0,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,               0,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,              0,
    STATUS_KEY_PARALYZE,            0,
    STATUS_KEY_SHRINK,              0,
    STATUS_KEY_STOP,                0,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          0,
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
        .targetOffset = { 0, 24 },
        .opacity = 255,
        .idleAnimations = DefaultAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { -1, -10 },
    },
};

EvtScript EVS_Init = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(EVS_TakeTurn))
    Call(BindIdle, ACTOR_SELF, Ref(EVS_Idle))
    Call(BindHandleEvent, ACTOR_SELF, Ref(EVS_HandleEvent))
    Return
    End
};

EvtScript EVS_Idle = {
    Return
    End
};

EvtScript EVS_ReturnHome = {
    Call(ResetAllActorSounds, ACTOR_SELF)
    SetConst(LVar0, PRT_MAIN)
    SetConst(LVar1, ANIM_BooBully_Idle)
    ExecWait(EVS_Enemy_ReturnHome)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BooBully_Idle)
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
            SetConst(LVar1, ANIM_BooBully_Flail)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail) //BurnHurt
            SetConst(LVar2, ANIM_BooBully_Flail) //BurnStill
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail) //BurnHurt
            SetConst(LVar2, ANIM_BooBully_Flail) //BurnStill
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail) //BurnStill
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail) //Death
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail)
            ExecWait(EVS_Enemy_Knockback)
            ExecWait(EVS_ReturnHome)
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail)//Death
            ExecWait(EVS_Enemy_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Idle)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail)//Death
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_BEGIN_FIRST_STRIKE)
            Call(SetActorPos, ACTOR_SELF, 20, 0, 0)
            Call(HPBarToCurrent, ACTOR_SELF)
        CaseEq(EVENT_END_FIRST_STRIKE)
            ExecWait(EVS_ReturnHome)
            Call(HPBarToHome, ACTOR_SELF)
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Idle)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLYING, false)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Idle)//Move?
            SetConst(LVar2, ANIM_BooBully_Flail)
            ExecWait(EVS_Enemy_ScareAway)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Idle)
            ExecWait(EVS_Enemy_AirLift)
        CaseEq(EVENT_BLOW_AWAY)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                IfFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(SetActorDispOffset, ACTOR_SELF, 0, 3, 0)
                Else
                    Call(SetActorDispOffset, ACTOR_SELF, 0, 9, 0)
                EndIf
            EndIf
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BooBully_Flail)
            ExecWait(EVS_Enemy_BlowAway)
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_TakeTurn = {
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(CountTargets, ACTOR_SELF, TARGET_FLAG_2 | TARGET_FLAG_PRIMARY_ONLY, LVar0)
    IfEq(LVar0, 1)
        ExecWait(EVS_Attack_Scare)
        Return
    Else
        Call(RandInt, 100, LVar4)
        Switch(LVar4)
            CaseLt(20)
                ExecWait(EVS_Move_PowerTransfer)
            CaseDefault
                ExecWait(EVS_Attack_Scare)
        EndSwitch
    EndIf
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_Scare = {
    #define LBL_SKIP_DAMAGE 0
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOO_VANISH_A)
	SetF(LVar0, 256)
	Loop(16)
		Sub(LVar0, 16)
		Call(SetPartAlpha, ACTOR_SELF, PRT_MAIN, LVar0)
		Wait(1)
	EndLoop
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BooBully_Conceal) //Hide
	Wait(20)
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
	Add(LVar0, 25)
	Set(LVar1, 10)
	Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
	Call(SetActorSpeed, ACTOR_SELF, Float(10.0))
	Call(FlyToGoal, ACTOR_SELF, 0, 0, EASING_COS_IN_OUT)
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOO_APPEAR_A)
	SetF(LVar0, 0)
	Loop(16)
		Add(LVar0, 16)
		Call(SetPartAlpha, ACTOR_SELF, PRT_MAIN, LVar0)
		Wait(1)
	EndLoop
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BooBully_Scare)
    Wait(28)
	Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_NO_CONTACT, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
	Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
			IfEq(LVar0, HIT_RESULT_LUCKY)
                Call(SetGoalToTarget, ACTOR_SELF)
				Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
			EndIf
			Goto(LBL_SKIP_DAMAGE)
		EndCaseGroup
	EndSwitch
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOO_SPOOK)
	Wait(2)
    Call(SetGoalToTarget, ACTOR_SELF)
	Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_NO_CONTACT, 0, 0, dmgScare, BS_FLAGS1_TRIGGER_EVENTS) // 3 Damage
	Label(LBL_SKIP_DAMAGE)
	Wait(30)
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOO_VANISH_A)
	SetF(LVar0, 256)
	Loop(16)
		Sub(LVar0, 16)
		Call(SetPartAlpha, ACTOR_SELF, PRT_MAIN, LVar0)
		Wait(1)
	EndLoop
	Wait(10)
	Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
	Call(MoveBattleCamOver, 20)
	Call(SetGoalToHome, ACTOR_SELF)
	Call(SetActorSpeed, ACTOR_SELF, Float(10.0))
	Call(FlyToGoal, ACTOR_SELF, 30, 0, EASING_COS_IN_OUT)
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BooBully_Idle)
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOO_APPEAR_A)
	SetF(LVar0, 0)
	Loop(16)
		Add(LVar0, 16)
		Call(SetPartAlpha, ACTOR_SELF, PRT_MAIN, LVar0)
		Wait(1)
	EndLoop
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
    #undef LBL_SKIP_DAMAGE
};

#include "common/battle/CheckMagikoopaCastTarget.inc.c"

EvtScript EVS_Move_PowerTransfer = {
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Set(LFlag0, false)
    Label(10)
        Call(CreateHomeTargetList, TARGET_FLAG_2 | TARGET_FLAG_PRIMARY_ONLY)
        Call(InitTargetIterator)
        Label(0)
            Call(GetOwnerTarget, LVar0, LVar1)
            Call(GetIndexFromHome, LVar0, LVar1)
            Call(GetBattleVar, BTL_VAR_Magikoopa_LastIndexBoosted, LVar2)
            IfGt(LVar1, LVar2)
                Call(N(CheckMagikoopaCastTarget), LVar0, LVar3)
                IfEq(LVar3, 0)
                    Call(GetStatusFlags, LVar0, LVar4)
                    IfNotFlag(LVar4, STATUS_FLAG_STATIC | STATUS_FLAG_TRANSPARENT)
                        Set(LVar8, LVar0)
                        Call(SetBattleVar, BTL_VAR_Magikoopa_LastIndexBoosted, LVar1)
                        Goto(100)
                    EndIf
                EndIf
            EndIf
            Call(ChooseNextTarget, ITER_NEXT, LVar0)
            IfNe(LVar0, ITER_NO_MORE)
                Goto(0)
            EndIf
        IfEq(LFlag0, false)
            Set(LFlag0, true)
            Call(SetBattleVar, BTL_VAR_Magikoopa_LastIndexBoosted, -1)
            Goto(10)
        EndIf
    Return
    Label(100) // make ally invisible
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(UseBattleCamPreset, BTL_CAM_ACTOR)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(MoveBattleCamOver, 15)
    Wait(15)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BooBully_Laugh)
    Wait(5)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SPELL_CAST1)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(GetStatusFlags, ACTOR_SELF, LVarA)
    IfFlag(LVarA, STATUS_FLAG_SHRINK)
        Add(LVar1, 10)
        Sub(LVar2, 2)
        PlayEffect(EFFECT_RADIAL_SHIMMER, 6, LVar0, LVar1, LVar2, Float(0.12), 30, 0)
    Else
        Add(LVar1, 20)
        Sub(LVar2, 2)
        PlayEffect(EFFECT_RADIAL_SHIMMER, 6, LVar0, LVar1, LVar2, Float(0.3), 30, 0)
    EndIf
    Wait(30)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BooBully_Idle)
    Wait(5)
    Thread
        Wait(10)
        Call(PlaySoundAtActor, LVar8, SOUND_MAGIKOOPA_VANISH)
    EndThread
    Thread
        Call(FreezeBattleState, true)
        Call(VanishActor, LVar8, 1)
        Call(FreezeBattleState, false)
    EndThread
    Call(WaitForBuffDone)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Wait(10)
    Call(YieldTurn)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

}; // namespace boo_bully

ActorBlueprint BooBully = {
    .flags = 0,
    .maxHP = boo_bully::hp,
    .type = ACTOR_TYPE_BOO_BULLY,
    .level = ACTOR_LEVEL_BOO_BULLY,
    .partCount = ARRAY_COUNT(boo_bully::ActorParts),
    .partsData = boo_bully::ActorParts,
    .initScript = &boo_bully::EVS_Init,
    .statusTable = boo_bully::StatusTable,
    .escapeChance = 95,
    .airLiftChance = 0,
    .hurricaneChance = 0,
    .spookChance = 100,
    .upAndAwayChance = 95,
    .spinSmashReq = 0,
    .powerBounceChance = 100,
    .coinReward = 1,
    .size = { 28, 24 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

}; // namespace battle::actor
