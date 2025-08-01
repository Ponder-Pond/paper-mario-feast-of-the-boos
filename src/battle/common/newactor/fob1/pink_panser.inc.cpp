#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/Panser.h"
#include "fob1_actors.hpp"

namespace battle::actor {

namespace pink_panser {

extern EvtScript EVS_Init;
extern s32 DefaultAnims[];
extern s32 SeedAnims[];
extern EvtScript EVS_Idle;
extern EvtScript EVS_TakeTurn;
extern EvtScript EVS_HandleEvent;
extern EvtScript EVS_ReturnHome;
extern EvtScript EVS_Attack_Seed;

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_SEED        = 2,
};

// Actor Stats
constexpr s32 hp = 6;
constexpr s32 dmgSeed = 2;

s32 DefaultAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_Panser_Pink_Idle,
    STATUS_KEY_STONE,     ANIM_Panser_Pink_Still,
    STATUS_KEY_SLEEP,     ANIM_Panser_Pink_Idle,
    STATUS_KEY_POISON,    ANIM_Panser_Pink_Idle,
    STATUS_KEY_STOP,      ANIM_Panser_Pink_Still,
    STATUS_KEY_STATIC,    ANIM_Panser_Pink_Idle,
    STATUS_KEY_PARALYZE,  ANIM_Panser_Pink_Still,
    STATUS_KEY_DIZZY,     ANIM_Panser_Pink_Idle,
    STATUS_KEY_FEAR,      ANIM_Panser_Pink_Idle,
    STATUS_END,
};

s32 SeedAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_Panser_Pink_Seed,
    STATUS_END,
};

s32 DefenseTable[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_FIRE,    -1,
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
    {
        .flags = ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_SEED,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = SeedAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
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
    SetConst(LVar1, ANIM_Panser_Pink_Idle)
    ExecWait(EVS_Enemy_ReturnHome)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Panser_Pink_Idle)
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
            SetConst(LVar1, ANIM_Panser_Pink_Hurt)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Hurt) //BurnHurt
            SetConst(LVar2, ANIM_Panser_Pink_Hurt) //BurnStill
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Hurt) //BurnHurt
            SetConst(LVar2, ANIM_Panser_Pink_Hurt) //BurnStill
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Hurt) //BurnStill
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Hurt) //Death
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Hurt)
            ExecWait(EVS_Enemy_Knockback)
            ExecWait(EVS_ReturnHome)
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Hurt)//Death
            ExecWait(EVS_Enemy_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Idle)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Hurt)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Hurt)//Death
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
            SetConst(LVar1, ANIM_Panser_Pink_Idle)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLYING, false)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Idle)//Move?
            SetConst(LVar2, ANIM_Panser_Pink_Hurt)
            ExecWait(EVS_Enemy_ScareAway)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Pink_Idle)
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
            SetConst(LVar1, ANIM_Panser_Pink_Hurt)
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
    ExecWait(EVS_Attack_Seed)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_Seed = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Panser_Pink_Shot)
    Thread
        Wait(10)
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_ACTOR_HURT)
        Wait(14)
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_ACTOR_HURT)
    EndThread
    Wait(40)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_POWER_UP)
    Wait(18)
    Call(SetAnimation, ACTOR_SELF, PRT_SEED, ANIM_Panser_Pink_Seed)
    Wait(15)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar1, 10)
    Call(SetPartPos, ACTOR_SELF, PRT_SEED, LVar0, LVar1, LVar2)
    Call(SetPartJumpGravity, ACTOR_SELF, PRT_SEED, Float(3.0))
    Call(SetPartScale, ACTOR_SELF, PRT_SEED, Float(1.5), Float(1.5), Float(1.0))
    Wait(45)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_SEED, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            // Set(LVarA, LVar0)
            Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
            Add(LVar1, -32) // target mario's feet
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_MOLE_THROW)
            Call(JumpPartTo, ACTOR_SELF, PRT_SEED, LVar0, LVar1, LVar2, 15, true)
            Call(SetPartFlagBits, ACTOR_SELF, PRT_SEED, ACTOR_PART_FLAG_INVISIBLE, true)
            Wait(2)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(SetGoalToTarget, ACTOR_SELF)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
        EndCaseGroup
    EndSwitch
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_MOLE_THROW)
    Call(JumpPartTo, ACTOR_SELF, PRT_SEED, LVar0, LVar1, LVar2, 15, true)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_SEED, ACTOR_PART_FLAG_INVISIBLE, true)
    Wait(2)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_NO_CONTACT, 0, 0, dmgSeed, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Wait(20)
            Call(YieldTurn)
            ExecWait(EVS_ReturnHome)
        EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

}; // namespace pink_panser

ActorBlueprint PinkPanser = {
    .flags = 0,
    .maxHP = pink_panser::hp,
    .type = ACTOR_TYPE_PINK_PANSER,
    .level = ACTOR_LEVEL_PINK_PANSER,
    .partCount = ARRAY_COUNT(pink_panser::ActorParts),
    .partsData = pink_panser::ActorParts,
    .initScript = &pink_panser::EVS_Init,
    .statusTable = pink_panser::StatusTable,
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
