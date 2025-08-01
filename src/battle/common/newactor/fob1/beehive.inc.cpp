#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/player.h"
#include "sprite/npc/InsectsDK3.h"
#include "battle/action_cmd/stop_leech.h"
#include "fob1_actors.hpp"
#include "dx/debug_menu.h"

namespace battle::actor {

namespace beehive {


extern EvtScript EVS_Init;
extern s32 DefaultAnims[];
extern s32 BuzzbeeAnims[];
extern s32 SuperBeeAnims[];
extern s32 ButterflyAnims[];
extern s32 AttackerAnims[];
extern s32 KabutomushiAnims[];
extern s32 BeespyAnims[];
extern s32 CreepyAnims[];
extern s32 VineEaterAnims[];
extern s32 SpearAnims[];
extern EvtScript EVS_Idle;
extern EvtScript EVS_TakeTurn;
extern EvtScript EVS_HandleEvent;
extern EvtScript EVS_Insects_GoBeehive;
extern EvtScript EVS_Buzzbee_ReturnHome;
extern EvtScript EVS_SuperBee_ReturnHome;
extern EvtScript EVS_Butterfly_ReturnHome;
extern EvtScript EVS_Attacker_ReturnHome;
extern EvtScript EVS_Kabutomushi_ReturnHome;
extern EvtScript EVS_Beespy_ReturnHome;
extern EvtScript EVS_SetUp_Buzzbee_Attack;
extern EvtScript EVS_Attack_Buzzbee;
extern EvtScript EVS_Attack_SuperBee;
extern EvtScript EVS_Attack_Butterfly;
extern EvtScript EVS_Attack_Attacker;
extern EvtScript EVS_Attack_Kabutomushi;
extern EvtScript EVS_Buzzbee_BehindFollow_Beespy;
extern EvtScript EVS_Buzzbee_GoBehind_Beespy;
extern EvtScript EVS_Attack_QuadBuzzbee;
extern EvtScript EVS_Attack_Beespy;
extern EvtScript EVS_Attack_Creepy;
extern EvtScript EVS_Attack_VineEater;

// Define enum for actor part IDs
enum ActorPartIDs {
    PRT_MAIN            = 1,
    PRT_BUZZBEE_1       = 2,
    PRT_BUZZBEE_2       = 3,
    PRT_BUZZBEE_3       = 4,
    PRT_BUZZBEE_4       = 5,
    PRT_SUPER_BEE       = 6,
    PRT_BUTTERFLY       = 7,
    PRT_ATTACKER        = 8,
    PRT_KABUTOMUSHI     = 9,
    PRT_BEESPY          = 10,
    PRT_CREEPY          = 11,
    PRT_VINE_EATER      = 12,
    PRT_SPEAR_1         = 13,
    PRT_SPEAR_2         = 14,
    PRT_SPEAR_3         = 15,
    PRT_SPEAR_4         = 16,
};

// Actor Stats
constexpr s32 hp = 8;
constexpr s32 dmgBuzzbee = 1;
constexpr s32 dmgSuperBee = 1;
constexpr s32 dmgButterFly = 2;
constexpr s32 dmgAttacker = 2;
constexpr s32 dmgKabutomushi = 3;
constexpr s32 dmgBeespy = 4;
constexpr s32 dmgCreepy = 1;
constexpr s32 dmgVineEater = 2;

// #define NUM_INSECTS (1 + PRT_VINE_EATER - PRT_BUZZBEE_1)  // There are 8 Insects from PRT_BUZZBEE_1 to PRT_VINE_EATER

// #define LOOP_INSECTS(idx) \
//     Set(idx, PRT_BUZZBEE_1) \
//     Loop(NUM_INSECTS)

s32 DefenseTable[] = {
    ELEMENT_NORMAL,     0,
    ELEMENT_FIRE,      -1,
    ELEMENT_QUAKE,    -20,
    ELEMENT_END,
};

s32 StatusTable[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,              85,
    STATUS_KEY_POISON,              0,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,               0,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,              0,
    STATUS_KEY_PARALYZE,            0,
    STATUS_KEY_SHRINK,              0,
    STATUS_KEY_STOP,               60,
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
        .targetOffset = { 0, -10 },
        .opacity = 255,
        .idleAnimations = DefaultAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_BUZZBEE_1,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = BuzzbeeAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_BUZZBEE_2,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = BuzzbeeAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_BUZZBEE_3,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = BuzzbeeAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_BUZZBEE_4,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = BuzzbeeAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_SUPER_BEE,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = SuperBeeAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_BUTTERFLY,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = ButterflyAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_ATTACKER,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = AttackerAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_KABUTOMUSHI,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = KabutomushiAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_BEESPY,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = BeespyAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_CREEPY,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = CreepyAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_VINE_EATER,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = VineEaterAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_SPEAR_1,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = SpearAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_SPEAR_2,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = SpearAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_SPEAR_3,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = SpearAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
        .index = PRT_SPEAR_4,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = SpearAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
};

s32 DefaultAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_InsectsDK3_Beehive,
    STATUS_END,
};

s32 BuzzbeeAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_InsectsDK3_Buzzbee,
    STATUS_END,
};

s32 SuperBeeAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_InsectsDK3_Superbee,
    STATUS_END,
};

s32 ButterflyAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_InsectsDK3_Butterfly,
    STATUS_END,
};

s32 AttackerAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_InsectsDK3_Attacker,
    STATUS_END,
};

s32 KabutomushiAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_InsectsDK3_Kabutomushi,
    STATUS_END,
};

s32 BeespyAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_InsectsDK3_QueenBeespy,
    STATUS_END,
};

s32 CreepyAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_InsectsDK3_Creepy,
    STATUS_END,
};

s32 VineEaterAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_InsectsDK3_VineEater,
    STATUS_END,
};

s32 SpearAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_InsectsDK3_Spear,
    STATUS_END,
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

EvtScript EVS_HandleEvent = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseOrEq(EVENT_HIT_COMBO)
        CaseOrEq(EVENT_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_InsectsDK3_Beehive) // Hurt
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_STAR_BEAM)
            // do nothing
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseEq(EVENT_DEATH)
            Call(GetLastElement, LVarE)
            IfFlag(LVarE, DAMAGE_TYPE_QUAKE | DAMAGE_TYPE_QUAKE_HAMMER)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_InsectsDK3_Beehive) // Hurt
                ExecWait(EVS_Enemy_Hit)
                Wait(10)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_InsectsDK3_Beehive) // Dead
                ExecWait(EVS_Enemy_Death)
                Return
            Else
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_InsectsDK3_Beehive) // Hurt
                ExecWait(EVS_Enemy_Hit)
                Wait(10)
                SetConst(LVar0, PRT_MAIN)
                SetConst(LVar1, ANIM_InsectsDK3_Beehive) // Dead
                ExecWait(EVS_Enemy_Death)
                Return
            EndIf
        CaseEq(EVENT_END_FIRST_STRIKE)
            // Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(2.0))
            // Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_Beehive)
            // Call(SetGoalToHome, ACTOR_SELF)
            // Call(SetPartMoveSpeed, ACTOR_SELF, Float(4.0))
            // Call(RunToGoal, ACTOR_SELF, 0, false)
            // Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
            // Call(HPBarToHome, ACTOR_SELF)
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_InsectsDK3_Beehive)
            ExecWait(EVS_Enemy_Recover)
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

// #include "common/CalculateArcsinDeg.inc.c"

EvtScript EVS_TakeTurn = {
    // Define macros
    #define BuzzbeeAttack 0
    #define SuperBeeAttack 1
    #define ButterflyAttack 2
    #define AttackerAttack 3
    #define KabutomushiAttack 4
    #define BeespyAttack 5
    #define CreepyAttack 6
    #define VineEaterAttack 7
    #define AttackEventScript LVar3
    #define AttackPriority LVar4
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(RandInt, 100, AttackPriority)
    Switch(AttackPriority)
    CaseLt(20) // 20%
        Set(AttackEventScript, BuzzbeeAttack)
    CaseLt(40) // 20%
        Set(AttackEventScript, SuperBeeAttack)
    CaseLt(55) // 15%
        Set(AttackEventScript, ButterflyAttack)
    CaseLt(70) // 15$
        Set(AttackEventScript, AttackerAttack)
    CaseLt(80) // 10%
        Set(AttackEventScript, KabutomushiAttack)
    CaseLt(90) // 10%
        Set(AttackEventScript, BeespyAttack)
    CaseLt(95) // 5%
        Set(AttackEventScript, CreepyAttack)
    CaseDefault // 5%
        Set(AttackEventScript, VineEaterAttack)
    EndSwitch
    Set(AttackEventScript, BeespyAttack) // For attack testing.
    Switch(AttackEventScript)
    CaseEq(BuzzbeeAttack)
        Set(LVarC, 0)
        ExecWait(EVS_SetUp_Buzzbee_Attack)
    CaseEq(SuperBeeAttack)
        ExecWait(EVS_Attack_SuperBee)
    CaseEq(ButterflyAttack)
        ExecWait(EVS_Attack_Butterfly)
    CaseEq(AttackerAttack)
        ExecWait(EVS_Attack_Attacker)
    CaseEq(KabutomushiAttack)
        ExecWait(EVS_Attack_Kabutomushi)
    CaseEq(BeespyAttack)
        ExecWait(EVS_Attack_Beespy)
    CaseEq(CreepyAttack)
        ExecWait(EVS_Attack_Creepy)
    CaseEq(VineEaterAttack)
        ExecWait(EVS_Attack_VineEater)
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
    // Undefine macros
    #undef BuzzbeeAttack
    #undef SuperBeeAttack
    #undef ButterflyAttack
    #undef AttackerAttack
    #undef KabutomushiAttack
    #undef BeespyAttack
    #undef CreepyAttack
    #undef VineEaterAttack
    #undef AttackEventScript
    #undef AttackPriority
};

s32 PartArray[] = {PRT_BUZZBEE_1, PRT_SUPER_BEE, PRT_BUTTERFLY, PRT_ATTACKER, PRT_KABUTOMUSHI, PRT_BEESPY};

API_CALLABLE(GetBeePartIndex) {
    Bytecode* args = script->ptrReadPos;
    s32 index = evt_get_variable(script, *args++);

    evt_set_variable(script, *args++, PartArray[index]);
    return ApiStatus_DONE2;
}

EvtScript EVS_Insects_GoBeehive = {
    UseArray(PartArray)
    Set(LVar0, 0) //for loop iterator
    Loop(ARRAY_COUNT(PartArray))
        Call(GetBeePartIndex, LVar0, LVar1) //returns part to LVar1
        Switch(LVar1)
            CaseEq(PRT_BUZZBEE_1)
                ExecWait(EVS_Buzzbee_ReturnHome)
            CaseEq(PRT_SUPER_BEE)
                ExecWait(EVS_SuperBee_ReturnHome)
            CaseEq(PRT_BUTTERFLY)
                ExecWait(EVS_Butterfly_ReturnHome)
            CaseEq(PRT_ATTACKER)
                ExecWait(EVS_Attacker_ReturnHome)
            CaseEq(PRT_KABUTOMUSHI)
                ExecWait(EVS_Kabutomushi_ReturnHome)
            CaseEq(PRT_BEESPY)
                ExecWait(EVS_Beespy_ReturnHome)
            CaseDefault
                DebugPrintf("Invalid part ID at index %d, skipping.\n", ArrayVar(0))
        EndSwitch
        Add(LVar0, 1)
    EndLoop
    Return
    End
};

EvtScript EVS_Buzzbee_ReturnHome = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_Buzzbee)
    Call(ResetAllActorSounds, ACTOR_SELF)
    Call(GetActorPos, ACTOR_PLAYER, 0, 0, LVar4)
    Add(LVar4, 2)
    Call(GetActorPos, ACTOR_SELF, LVar2, LVar3, 0)
    Add(LVar2, -10)
    Add(LVar3, -30)
    Call(SetPartMoveSpeed, ACTOR_SELF, LVarA, Float(6.0))
    Call(FlyPartTo, ACTOR_SELF, LVarA, LVar2, LVar3, LVar4, 0, 0, EASING_COS_IN_OUT)
    Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_BuzzbeeExit)
    Wait(16)
    Call(SetPartFlagBits, ACTOR_SELF, LVarA, ACTOR_PART_FLAG_INVISIBLE, true)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Return
    End
};

EvtScript EVS_SuperBee_ReturnHome = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetAnimation, ACTOR_SELF, PRT_SUPER_BEE, ANIM_InsectsDK3_Superbee)
    Call(ResetAllActorSounds, ACTOR_SELF)
    Call(GetActorPos, ACTOR_PLAYER, 0, 0, LVar4)
    Add(LVar4, 2)
    Call(GetActorPos, ACTOR_SELF, LVar2, LVar3, 0)
    Add(LVar2, -10)
    Add(LVar3, -30)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_SUPER_BEE, Float(13.5))
    Call(FlyPartTo, ACTOR_SELF, PRT_SUPER_BEE, LVar2, LVar3, LVar4, 0, -40, EASING_COS_IN_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_SUPER_BEE, ANIM_InsectsDK3_SuperbeeExit)
    Wait(16)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_SUPER_BEE, ACTOR_PART_FLAG_INVISIBLE, true)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Return
    End
};

EvtScript EVS_Butterfly_ReturnHome = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetAnimation, ACTOR_SELF, PRT_BUTTERFLY, ANIM_InsectsDK3_Butterfly)
    Call(ResetAllActorSounds, ACTOR_SELF)
    Call(GetActorPos, ACTOR_PLAYER, 0, 0, LVar4)
    Add(LVar4, 2)
    Call(GetActorPos, ACTOR_SELF, LVar2, LVar3, 0)
    Add(LVar2, -10)
    Add(LVar3, -30)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_BUTTERFLY, Float(8.0))
    Call(FlyPartTo, ACTOR_SELF, PRT_BUTTERFLY, LVar2, LVar3, LVar4, 0, 0, EASING_COS_IN_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_BUTTERFLY, ANIM_InsectsDK3_ButterflyExit)
    Wait(16)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_BUTTERFLY, ACTOR_PART_FLAG_INVISIBLE, true)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Return
    End
};

EvtScript EVS_Attacker_ReturnHome = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetAnimation, ACTOR_SELF, PRT_ATTACKER, ANIM_InsectsDK3_Attacker)
    Call(ResetAllActorSounds, ACTOR_SELF)
    Call(GetActorPos, ACTOR_PLAYER, 0, 0, LVar4)
    Add(LVar4, 2)
    Call(GetActorPos, ACTOR_SELF, LVar2, LVar3, 0)
    Add(LVar2, -10)
    Add(LVar3, -30)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_ATTACKER, Float(11.0))
    Call(FlyPartTo, ACTOR_SELF, PRT_ATTACKER, LVar2, LVar3, LVar4, 0, -40, EASING_COS_IN_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_ATTACKER, ANIM_InsectsDK3_AttackerExit)
    Wait(16)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_ATTACKER, ACTOR_PART_FLAG_INVISIBLE, true)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Return
    End
};

EvtScript EVS_Kabutomushi_ReturnHome = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetAnimation, ACTOR_SELF, PRT_KABUTOMUSHI, ANIM_InsectsDK3_Kabutomushi)
    Call(ResetAllActorSounds, ACTOR_SELF)
    Call(GetActorPos, ACTOR_PLAYER, 0, 0, LVar4)
    Add(LVar4, 2)
    Call(GetActorPos, ACTOR_SELF, LVar2, LVar3, 0)
    Add(LVar2, -10)
    Add(LVar3, -25)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_KABUTOMUSHI, Float(15.0))
    Call(FlyPartTo, ACTOR_SELF, PRT_KABUTOMUSHI, LVar2, LVar3, LVar4, 0, -30, EASING_COS_IN_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_KABUTOMUSHI, ANIM_InsectsDK3_KabutomushiExit)
    Wait(16)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_KABUTOMUSHI, ACTOR_PART_FLAG_INVISIBLE, true)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Return
    End
};

EvtScript EVS_Beespy_ReturnHome = {
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(SetAnimation, ACTOR_SELF, PRT_BEESPY, ANIM_InsectsDK3_QueenBeespy)
    Call(ResetAllActorSounds, ACTOR_SELF)
    Call(GetActorPos, ACTOR_PLAYER, 0, 0, LVar4)
    Add(LVar4, 2)
    Call(GetActorPos, ACTOR_SELF, LVar2, LVar3, 0)
    Add(LVar2, -10)
    Add(LVar3, -30)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_BEESPY, Float(10.0))
    Call(FlyPartTo, ACTOR_SELF, PRT_BEESPY, LVar2, LVar3, LVar4, 0, 0, EASING_COS_IN_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_BEESPY, ANIM_InsectsDK3_QueenBeespyExit)
    Wait(16)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_BEESPY, ACTOR_PART_FLAG_INVISIBLE, true)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_SetUp_Buzzbee_Attack = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Switch(LVarC)
        CaseEq(0)
            Set(LVarA, PRT_BUZZBEE_1)
            Set(LVar9, PRT_SPEAR_1)
            ExecWait(EVS_Attack_Buzzbee)
        CaseEq(1)
            Set(LVarA, PRT_BUZZBEE_1)
            Set(LVar9, PRT_SPEAR_1)
            ExecWait(EVS_Attack_QuadBuzzbee)
        CaseEq(2)
            Set(LVarA, PRT_BUZZBEE_2)
            Set(LVar9, PRT_SPEAR_2)
            ExecWait(EVS_Attack_QuadBuzzbee)
        CaseEq(3)
            Set(LVarA, PRT_BUZZBEE_3)
            Set(LVar9, PRT_SPEAR_3)
            ExecWait(EVS_Attack_QuadBuzzbee)
        CaseEq(4)
            Set(LVarA, PRT_BUZZBEE_4)
            Set(LVar9, PRT_SPEAR_4)
            ExecWait(EVS_Attack_QuadBuzzbee)
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_Buzzbee = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 0)
    Add(LVar1, -30)
    Add(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
    CaseOrEq(HIT_RESULT_MISS)
    CaseOrEq(HIT_RESULT_LUCKY)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_BeehiveSpawn)
        Wait(25)
        Call(SetPartFlagBits, ACTOR_SELF, LVarA, ACTOR_PART_FLAG_INVISIBLE, false)
        Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_BuzzbeeSpawn)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_Beehive)
        Wait(10)
        Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_Buzzbee)
        Call(GetPartPos, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2)
        Add(LVar1, -25)
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetPartMoveSpeed, ACTOR_SELF, LVarA, Float(6.0))
        Call(SetGoalToTarget, ACTOR_SELF)
        Call(FlyPartTo, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
        Wait(5)
        Call(SetPartRotation, ACTOR_SELF, LVar9, 0, 0, 0)
        Thread
            Call(MakeLerp, 0, Float(47.5), 20, EASING_QUADRATIC_OUT)
            Label(0)
            Call(UpdateLerp)
            Call(SetPartRotation, ACTOR_SELF, LVar9, 0, 0, LVar0)
            Wait(1)
            IfEq(LVar1, 1)
                Goto(0)
            EndIf
        EndThread
        Call(GetPartPos, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2)
        Add(LVar0, -10)
        Add(LVar1, 20)
        Call(SetPartPos, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2)
        Call(SetPartFlagBits, ACTOR_SELF, LVar9, ACTOR_PART_FLAG_INVISIBLE, false)
        Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
        Add(LVar1, 20)
        Add(LVar2, 2)
        Call(SetPartJumpGravity, ACTOR_SELF, LVar9, Float(0.7))
        Call(JumpPartTo, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2, 18, true)
        Call(SetPartFlagBits, ACTOR_SELF, LVar9, ACTOR_PART_FLAG_INVISIBLE, true)
        Wait(2)
        Call(SetPartRotation, ACTOR_SELF, LVar9, 0, 0, 0)
        IfEq(LVar0, HIT_RESULT_LUCKY)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
        EndIf
        ExecWait(EVS_Buzzbee_ReturnHome)
        Call(YieldTurn)
        Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
        Call(UseIdleAnimation, ACTOR_SELF, true)
        Return
    EndCaseGroup
    EndSwitch
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_BeehiveSpawn)
    Wait(25)
    Call(SetPartFlagBits, ACTOR_SELF, LVarA, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_BuzzbeeSpawn)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_Beehive)
    Wait(10)
    Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_Buzzbee)
    Call(GetPartPos, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2)
    Add(LVar1, -25)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartMoveSpeed, ACTOR_SELF, LVarA, Float(6.0))
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(FlyPartTo, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
    Wait(5)
    Call(SetPartRotation, ACTOR_SELF, LVar9, 0, 0, 0)
    Thread
        Call(MakeLerp, 0, Float(47.5), 20, EASING_QUADRATIC_OUT)
        Label(0)
        Call(UpdateLerp)
        Call(SetPartRotation, ACTOR_SELF, LVar9, 0, 0, LVar0)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(0)
        EndIf
    EndThread
    Call(GetPartPos, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2)
    Add(LVar0, -10)
    Add(LVar1, 20)
    Call(SetPartPos, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2)
    Call(SetPartFlagBits, ACTOR_SELF, LVar9, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Add(LVar1, 20)
    Add(LVar2, 2)
    Call(SetPartJumpGravity, ACTOR_SELF, LVar9, Float(0.7))
    Call(JumpPartTo, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2, 18, true)
    Call(SetPartFlagBits, ACTOR_SELF, LVar9, ACTOR_PART_FLAG_INVISIBLE, true)
    Wait(2)
    Call(SetPartRotation, ACTOR_SELF, LVar9, 0, 0, 0)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_NO_CONTACT, 0, 0, dmgBuzzbee, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVarF)
    CaseOrEq(HIT_RESULT_HIT)
    CaseOrEq(HIT_RESULT_NO_DAMAGE)
        ExecWait(EVS_Buzzbee_ReturnHome)
        Call(YieldTurn)
    EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_SuperBee = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 0)
    Add(LVar1, -30)
    Add(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, PRT_SUPER_BEE, LVar0, LVar1, LVar2)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
    CaseOrEq(HIT_RESULT_MISS)
    CaseOrEq(HIT_RESULT_LUCKY)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_BeehiveSpawn)
        Wait(25)
        Call(SetPartFlagBits, ACTOR_SELF, PRT_SUPER_BEE, ACTOR_PART_FLAG_INVISIBLE, false)
        Call(SetAnimation, ACTOR_SELF, PRT_SUPER_BEE, ANIM_InsectsDK3_SuperbeeSpawn)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_Beehive)
        Wait(5)
        Call(SetAnimation, ACTOR_SELF, PRT_SUPER_BEE, ANIM_InsectsDK3_Superbee)
        Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
        Add(LVar2, 2)
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetPartMoveSpeed, ACTOR_SELF, PRT_SUPER_BEE, Float(13.5))
        Call(SetGoalToTarget, ACTOR_SELF)
        Wait(15)
        Call(FlyPartTo, ACTOR_SELF, PRT_SUPER_BEE, LVar0, LVar1, LVar2, 0, -40, EASING_COS_IN_OUT)
        Wait(2)
        IfEq(LVar0, HIT_RESULT_LUCKY)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
        EndIf
        ExecWait(EVS_SuperBee_ReturnHome)
        Call(YieldTurn)
        Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
        Call(UseIdleAnimation, ACTOR_SELF, true)
        Return
    EndCaseGroup
    EndSwitch
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_BeehiveSpawn)
    Wait(25)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_SUPER_BEE, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(SetAnimation, ACTOR_SELF, PRT_SUPER_BEE, ANIM_InsectsDK3_SuperbeeSpawn)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_Beehive)
    Wait(5)
    Call(SetAnimation, ACTOR_SELF, PRT_SUPER_BEE, ANIM_InsectsDK3_Superbee)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Add(LVar2, 2)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_SUPER_BEE, Float(13.5))
    Call(SetGoalToTarget, ACTOR_SELF)
    Wait(15)
    Call(FlyPartTo, ACTOR_SELF, PRT_SUPER_BEE, LVar0, LVar1, LVar2, 0, -40, EASING_COS_IN_OUT)
    Wait(2)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_NO_CONTACT, 0, 0, dmgSuperBee, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVarF)
    CaseOrEq(HIT_RESULT_HIT)
    CaseOrEq(HIT_RESULT_NO_DAMAGE)
        ExecWait(EVS_SuperBee_ReturnHome)
        Call(YieldTurn)
    EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_Butterfly = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 0)
    Add(LVar1, -30)
    Add(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, PRT_BUTTERFLY, LVar0, LVar1, LVar2)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
    CaseOrEq(HIT_RESULT_MISS)
    CaseOrEq(HIT_RESULT_LUCKY)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_BeehiveSpawn)
        Wait(25)
        Call(SetPartFlagBits, ACTOR_SELF, PRT_BUTTERFLY, ACTOR_PART_FLAG_INVISIBLE, false)
        Call(SetAnimation, ACTOR_SELF, PRT_BUTTERFLY, ANIM_InsectsDK3_ButterflySpawn)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_Beehive)
        Wait(10)
        Call(SetAnimation, ACTOR_SELF, PRT_BUTTERFLY, ANIM_InsectsDK3_Butterfly)
        Call(GetPartPos, ACTOR_SELF, PRT_BUTTERFLY, LVar0, LVar1, LVar2)
        Add(LVar1, -25)
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetPartMoveSpeed, ACTOR_SELF, PRT_BUTTERFLY, Float(8.0))
        Call(SetGoalToTarget, ACTOR_SELF)
        Call(FlyPartTo, ACTOR_SELF, PRT_BUTTERFLY, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
        Wait(5)
        Call(SetPartRotation, ACTOR_SELF, PRT_SPEAR_1, 0, 0, 0)
        Thread
            Call(MakeLerp, 0, Float(47.5), 20, EASING_QUADRATIC_OUT)
            Label(0)
            Call(UpdateLerp)
            Call(SetPartRotation, ACTOR_SELF, PRT_SPEAR_1, 0, 0, LVar0)
            Wait(1)
            IfEq(LVar1, 1)
                Goto(0)
            EndIf
        EndThread
        Call(GetPartPos, ACTOR_SELF, PRT_BUTTERFLY, LVar0, LVar1, LVar2)
        Add(LVar0, -10)
        Add(LVar1, 20)
        Call(SetPartPos, ACTOR_SELF, PRT_SPEAR_1, LVar0, LVar1, LVar2)
        Call(SetPartFlagBits, ACTOR_SELF, PRT_SPEAR_1, ACTOR_PART_FLAG_INVISIBLE, false)
        Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
        Add(LVar1, 20)
        Add(LVar2, 2)
        Call(SetPartJumpGravity, ACTOR_SELF, PRT_SPEAR_1, Float(0.7))
        Call(JumpPartTo, ACTOR_SELF, PRT_SPEAR_1, LVar0, LVar1, LVar2, 18, true)
        Wait(2)
        Call(SetPartRotation, ACTOR_SELF, PRT_SPEAR_1, 0, 0, 0)
        IfEq(LVar0, HIT_RESULT_LUCKY)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
        EndIf
        ExecWait(EVS_Butterfly_ReturnHome)
        Call(YieldTurn)
        Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
        Call(UseIdleAnimation, ACTOR_SELF, true)
        Return
    EndCaseGroup
    EndSwitch
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_BeehiveSpawn)
    Wait(25)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_BUTTERFLY, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(SetAnimation, ACTOR_SELF, PRT_BUTTERFLY, ANIM_InsectsDK3_ButterflySpawn)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_Beehive)
    Wait(10)
    Call(SetAnimation, ACTOR_SELF, PRT_BUTTERFLY, ANIM_InsectsDK3_Butterfly)
    Call(GetPartPos, ACTOR_SELF, PRT_BUTTERFLY, LVar0, LVar1, LVar2)
    Add(LVar1, -25)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_BUTTERFLY, Float(8.0))
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(FlyPartTo, ACTOR_SELF, PRT_BUTTERFLY, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
    Wait(5)
    Call(SetPartRotation, ACTOR_SELF, PRT_SPEAR_1, 0, 0, 0)
    Thread
        Call(MakeLerp, 0, Float(47.5), 20, EASING_QUADRATIC_OUT)
        Label(0)
        Call(UpdateLerp)
        Call(SetPartRotation, ACTOR_SELF, PRT_SPEAR_1, 0, 0, LVar0)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(0)
        EndIf
    EndThread
    Call(GetPartPos, ACTOR_SELF, PRT_BUTTERFLY, LVar0, LVar1, LVar2)
    Add(LVar0, -10)
    Add(LVar1, 20)
    Call(SetPartPos, ACTOR_SELF, PRT_SPEAR_1, LVar0, LVar1, LVar2)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_SPEAR_1, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Add(LVar1, 20)
    Add(LVar2, 2)
    Call(SetPartJumpGravity, ACTOR_SELF, PRT_SPEAR_1, Float(0.7))
    Call(JumpPartTo, ACTOR_SELF, PRT_SPEAR_1, LVar0, LVar1, LVar2, 18, true)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_SPEAR_1, ACTOR_PART_FLAG_INVISIBLE, true)
    Wait(2)
    Call(SetPartRotation, ACTOR_SELF, PRT_SPEAR_1, 0, 0, 0)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_NO_CONTACT, 0, 0, dmgBuzzbee, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVarF)
    CaseOrEq(HIT_RESULT_HIT)
    CaseOrEq(HIT_RESULT_NO_DAMAGE)
        ExecWait(EVS_Butterfly_ReturnHome)
        Call(YieldTurn)
    EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_Attacker = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 0)
    Add(LVar1, -20)
    Add(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, PRT_ATTACKER, LVar0, LVar1, LVar2)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
    CaseOrEq(HIT_RESULT_MISS)
    CaseOrEq(HIT_RESULT_LUCKY)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_BeehiveSpawn)
        Wait(25)
        Call(SetPartFlagBits, ACTOR_SELF, PRT_ATTACKER, ACTOR_PART_FLAG_INVISIBLE, false)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_Beehive)
        Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
        Add(LVar2, 2)
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetPartMoveSpeed, ACTOR_SELF, PRT_ATTACKER, Float(11.0))
        Call(SetGoalToTarget, ACTOR_SELF)
        Wait(15)
        Call(FlyPartTo, ACTOR_SELF, PRT_ATTACKER, LVar0, LVar1, LVar2, 0, -40, EASING_COS_IN_OUT)
        Wait(2)
        IfEq(LVar0, HIT_RESULT_LUCKY)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
        EndIf
        ExecWait(EVS_Insects_GoBeehive)
        Call(SetPartFlagBits, ACTOR_SELF, PRT_ATTACKER, ACTOR_PART_FLAG_INVISIBLE, true)
        Call(YieldTurn)
        Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
        Call(UseIdleAnimation, ACTOR_SELF, true)
        Return
    EndCaseGroup
    EndSwitch
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_BeehiveSpawn)
    Wait(25)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_ATTACKER, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_Beehive)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Add(LVar2, 2)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_ATTACKER, Float(11.0))
    Call(SetGoalToTarget, ACTOR_SELF)
    Wait(15)
    Call(FlyPartTo, ACTOR_SELF, PRT_ATTACKER, LVar0, LVar1, LVar2, 0, -40, EASING_COS_IN_OUT)
    Wait(2)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_NO_CONTACT, 0, 0, dmgAttacker, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVarF)
    CaseOrEq(HIT_RESULT_HIT)
    CaseOrEq(HIT_RESULT_NO_DAMAGE)
        ExecWait(EVS_Attacker_ReturnHome)
        Call(YieldTurn)
    EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_Kabutomushi = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 0)
    Add(LVar1, -30)
    Add(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, PRT_KABUTOMUSHI, LVar0, LVar1, LVar2)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
    CaseOrEq(HIT_RESULT_MISS)
    CaseOrEq(HIT_RESULT_LUCKY)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_BeehiveSpawn)
        Wait(25)
        Call(SetPartFlagBits, ACTOR_SELF, PRT_KABUTOMUSHI, ACTOR_PART_FLAG_INVISIBLE, false)
        Call(SetAnimation, ACTOR_SELF, PRT_KABUTOMUSHI, ANIM_InsectsDK3_KabutomushiSpawn)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_Beehive)
        Wait(5)
        Call(SetAnimation, ACTOR_SELF, PRT_KABUTOMUSHI, ANIM_InsectsDK3_Kabutomushi)
        Call(GetPartPos, ACTOR_SELF, PRT_KABUTOMUSHI, LVar0, LVar1, LVar2)
        Add(LVar1, -60)
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetPartMoveSpeed, ACTOR_SELF, PRT_KABUTOMUSHI, Float(8.0))
        Call(FlyPartTo, ACTOR_SELF, PRT_KABUTOMUSHI, LVar0, LVar1, LVar2, 0, 0, EASING_LINEAR)
        Wait(15)
        Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
        Add(LVar2, 2)
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetPartMoveSpeed, ACTOR_SELF, PRT_KABUTOMUSHI, Float(15.0))
        Call(SetGoalToTarget, ACTOR_SELF)
        // Wait(5)
        Call(FlyPartTo, ACTOR_SELF, PRT_KABUTOMUSHI, LVar0, LVar1, LVar2, 0, -30, EASING_COS_IN_OUT)
        Wait(2)
        IfEq(LVar0, HIT_RESULT_LUCKY)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
        EndIf
        // ExecWait(EVS_Insects_GoBeehive)
        ExecWait(EVS_Kabutomushi_ReturnHome)
        Call(YieldTurn)
        Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
        Call(UseIdleAnimation, ACTOR_SELF, true)
        Return
    EndCaseGroup
    EndSwitch
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_BeehiveSpawn)
    Wait(25)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_KABUTOMUSHI, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(SetAnimation, ACTOR_SELF, PRT_KABUTOMUSHI, ANIM_InsectsDK3_KabutomushiSpawn)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_Beehive)
    Wait(5)
    Call(SetAnimation, ACTOR_SELF, PRT_KABUTOMUSHI, ANIM_InsectsDK3_Kabutomushi)
    Call(GetPartPos, ACTOR_SELF, PRT_KABUTOMUSHI, LVar0, LVar1, LVar2)
    Add(LVar1, -60)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_KABUTOMUSHI, Float(8.0))
    Call(FlyPartTo, ACTOR_SELF, PRT_KABUTOMUSHI, LVar0, LVar1, LVar2, 0, 0, EASING_LINEAR)
    Wait(15)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Add(LVar2, 2)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_KABUTOMUSHI, Float(15.0))
    Call(SetGoalToTarget, ACTOR_SELF)
    // Wait(5)
    Call(FlyPartTo, ACTOR_SELF, PRT_KABUTOMUSHI, LVar0, LVar1, LVar2, 0, -30, EASING_COS_IN_OUT)
    Wait(2)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_NO_CONTACT, 0, 0, dmgKabutomushi, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVarF)
    CaseOrEq(HIT_RESULT_HIT)
    CaseOrEq(HIT_RESULT_NO_DAMAGE)
        ExecWait(EVS_Kabutomushi_ReturnHome)
        Call(YieldTurn)
    EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Buzzbee_BehindFollow_Beespy = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Switch(LVarA)
        CaseEq(PRT_BUZZBEE_1)
            Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
            Sub(LVar2, 2)
            Call(SetPartPos, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2)
            Call(SetPartFlagBits, ACTOR_SELF, LVarA, ACTOR_PART_FLAG_INVISIBLE, false)
            Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_Buzzbee)
            Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
            Add(LVar0, 25)
            Add(LVar1, 25)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetPartMoveSpeed, ACTOR_SELF, LVarA, Float(6.0))
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(FlyPartTo, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
            Wait(5)
        CaseEq(PRT_BUZZBEE_2)
            Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
            Sub(LVar2, 2)
            Call(SetPartPos, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2)
            Call(SetPartFlagBits, ACTOR_SELF, LVarA, ACTOR_PART_FLAG_INVISIBLE, false)
            Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_Buzzbee)
            Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
            Sub(LVar0, 25)
            Add(LVar1, 25)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetPartMoveSpeed, ACTOR_SELF, LVarA, Float(6.0))
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(FlyPartTo, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
            Wait(5)
        CaseEq(PRT_BUZZBEE_3)
            Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
            Sub(LVar2, 2)
            Call(SetPartPos, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2)
            Call(SetPartFlagBits, ACTOR_SELF, LVarA, ACTOR_PART_FLAG_INVISIBLE, false)
            Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_Buzzbee)
            Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
            Add(LVar0, 25)
            Sub(LVar1, 25)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetPartMoveSpeed, ACTOR_SELF, LVarA, Float(6.0))
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(FlyPartTo, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
            Wait(5)
        CaseEq(PRT_BUZZBEE_4)
            Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
            Sub(LVar2, 2)
            Call(SetPartPos, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2)
            Call(SetPartFlagBits, ACTOR_SELF, LVarA, ACTOR_PART_FLAG_INVISIBLE, false)
            Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_Buzzbee)
            Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
            Sub(LVar0, 25)
            Sub(LVar1, 25)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetPartMoveSpeed, ACTOR_SELF, LVarA, Float(6.0))
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(FlyPartTo, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
            Wait(5)
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Buzzbee_GoBehind_Beespy = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Switch(LVarA)
        CaseEq(PRT_BUZZBEE_1)
            Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
            Sub(LVar2, 2)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetPartMoveSpeed, ACTOR_SELF, LVarA, Float(6.0))
            Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_Buzzbee)
            Call(FlyPartTo, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
            Wait(5)
            Call(SetPartFlagBits, ACTOR_SELF, LVarA, ACTOR_PART_FLAG_INVISIBLE, true)
        CaseEq(PRT_BUZZBEE_2)
            Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
            Sub(LVar2, 2)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetPartMoveSpeed, ACTOR_SELF, LVarA, Float(6.0))
            Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_Buzzbee)
            Call(FlyPartTo, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
            Wait(5)
            Call(SetPartFlagBits, ACTOR_SELF, LVarA, ACTOR_PART_FLAG_INVISIBLE, true)
        CaseEq(PRT_BUZZBEE_3)
            Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
            Sub(LVar2, 2)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetPartMoveSpeed, ACTOR_SELF, LVarA, Float(6.0))
            Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_Buzzbee)
            Call(FlyPartTo, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
            Wait(5)
            Call(SetPartFlagBits, ACTOR_SELF, LVarA, ACTOR_PART_FLAG_INVISIBLE, true)
        CaseEq(PRT_BUZZBEE_4)
            Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
            Sub(LVar2, 2)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetPartMoveSpeed, ACTOR_SELF, LVarA, Float(6.0))
            Call(SetAnimation, ACTOR_SELF, LVarA, ANIM_InsectsDK3_Buzzbee)
            Call(FlyPartTo, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
            Wait(5)
            Call(SetPartFlagBits, ACTOR_SELF, LVarA, ACTOR_PART_FLAG_INVISIBLE, true)
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_QuadBuzzbee = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
    CaseOrEq(HIT_RESULT_MISS)
    CaseOrEq(HIT_RESULT_LUCKY)
        Wait(5)
        Call(SetPartRotation, ACTOR_SELF, LVar9, 0, 0, 0)
        Thread
            Call(MakeLerp, 0, Float(47.5), 20, EASING_QUADRATIC_OUT)
            Label(0)
            Call(UpdateLerp)
            Call(SetPartRotation, ACTOR_SELF, LVar9, 0, 0, LVar0)
            Wait(1)
            IfEq(LVar1, 1)
                Goto(0)
            EndIf
        EndThread
        Call(GetPartPos, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2)
        Add(LVar0, -10)
        Add(LVar1, 20)
        Call(SetPartPos, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2)
        Call(SetPartFlagBits, ACTOR_SELF, LVar9, ACTOR_PART_FLAG_INVISIBLE, false)
        Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
        Add(LVar1, 20)
        Add(LVar2, 2)
        Call(SetPartJumpGravity, ACTOR_SELF, LVar9, Float(0.7))
        Call(JumpPartTo, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2, 18, true)
        Call(SetPartFlagBits, ACTOR_SELF, LVar9, ACTOR_PART_FLAG_INVISIBLE, true)
        Wait(2)
        Call(SetPartRotation, ACTOR_SELF, LVar9, 0, 0, 0)
        IfEq(LVar0, HIT_RESULT_LUCKY)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
        EndIf
        Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
        Call(UseIdleAnimation, ACTOR_SELF, true)
        Return
    EndCaseGroup
    EndSwitch
    Wait(5)
    Call(SetPartRotation, ACTOR_SELF, LVar9, 0, 0, 0)
    Thread
        Call(MakeLerp, 0, Float(47.5), 20, EASING_QUADRATIC_OUT)
        Label(0)
        Call(UpdateLerp)
        Call(SetPartRotation, ACTOR_SELF, LVar9, 0, 0, LVar0)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(0)
        EndIf
    EndThread
    Call(GetPartPos, ACTOR_SELF, LVarA, LVar0, LVar1, LVar2)
    Add(LVar0, -10)
    Add(LVar1, 20)
    Call(SetPartPos, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2)
    Call(SetPartFlagBits, ACTOR_SELF, LVar9, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Add(LVar1, 20)
    Add(LVar2, 2)
    Call(SetPartJumpGravity, ACTOR_SELF, LVar9, Float(0.7))
    Call(JumpPartTo, ACTOR_SELF, LVar9, LVar0, LVar1, LVar2, 18, true)
    Call(SetPartFlagBits, ACTOR_SELF, LVar9, ACTOR_PART_FLAG_INVISIBLE, true)
    Wait(2)
    Call(SetPartRotation, ACTOR_SELF, LVar9, 0, 0, 0)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_NO_CONTACT, 0, 0, dmgBuzzbee, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVarF)
    CaseOrEq(HIT_RESULT_HIT)
    CaseOrEq(HIT_RESULT_NO_DAMAGE)
    EndCaseGroup
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_Beespy = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 0)
    Add(LVar1, -30)
    Add(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_BeehiveSpawn)
    Wait(25)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_BEESPY, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(SetAnimation, ACTOR_SELF, PRT_BEESPY, ANIM_InsectsDK3_QueenBeespySpawn)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_InsectsDK3_Beehive)
    Wait(10)
    Call(SetAnimation, ACTOR_SELF, PRT_BEESPY, ANIM_InsectsDK3_QueenBeespy)
    Call(GetPartPos, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2)
    Set(LVar0, 55)
    Set(LVar1, 50)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_BEESPY, Float(6.0))
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(FlyPartTo, ACTOR_SELF, PRT_BEESPY, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
    Wait(5)
    Set(LVarA, PRT_BUZZBEE_1)
    ExecWait(EVS_Buzzbee_BehindFollow_Beespy)
    Set(LVarC, 1)
    Exec(EVS_SetUp_Buzzbee_Attack)
    Wait(10)
    Set(LVarA, PRT_BUZZBEE_2)
    ExecWait(EVS_Buzzbee_BehindFollow_Beespy)
    Set(LVarA, PRT_BUZZBEE_1)
    Exec(EVS_Buzzbee_GoBehind_Beespy)
    Wait(5)
    Set(LVarC, 2)
    Exec(EVS_SetUp_Buzzbee_Attack)
    Wait(5)
    Set(LVarA, PRT_BUZZBEE_3)
    ExecWait(EVS_Buzzbee_BehindFollow_Beespy)
    Set(LVarA, PRT_BUZZBEE_2)
    Exec(EVS_Buzzbee_GoBehind_Beespy)
    Wait(10)
    Set(LVarC, 3)
    Exec(EVS_SetUp_Buzzbee_Attack)
    Wait(10)
    Set(LVarA, PRT_BUZZBEE_4)
    ExecWait(EVS_Buzzbee_BehindFollow_Beespy)
    Set(LVarA, PRT_BUZZBEE_3)
    Exec(EVS_Buzzbee_GoBehind_Beespy)
    Wait(5)
    Set(LVarC, 4)
    Exec(EVS_SetUp_Buzzbee_Attack)
    Wait(10)
    Set(LVarA, PRT_BUZZBEE_4)
    Exec(EVS_Buzzbee_GoBehind_Beespy)
    // Set(LVarC, 1)
    // Exec(EVS_SetUp_Buzzbee_Attack)
    // Wait(15)
    // Set(LVarC, 2)
    // Exec(EVS_SetUp_Buzzbee_Attack)
    // Wait(15)
    // Set(LVarC, 3)
    // Exec(EVS_SetUp_Buzzbee_Attack)
    // Wait(15)
    // Set(LVarC, 4)
    // Exec(EVS_SetUp_Buzzbee_Attack)
    WaitSecs(1)
    ExecWait(EVS_Beespy_ReturnHome)
    // Set(LVarA, PRT_BUZZBEE_1)
    // ExecWait(EVS_Buzzbee_GoBehind_Beespy)
    // Wait(15)
    // Set(LVarA, PRT_BUZZBEE_2)
    // ExecWait(EVS_Buzzbee_GoBehind_Beespy)
    // Wait(15)
    // Set(LVarA, PRT_BUZZBEE_3)
    // ExecWait(EVS_Buzzbee_GoBehind_Beespy)
    // Wait(15)
    // Set(LVarA, PRT_BUZZBEE_4)
    // ExecWait(EVS_Buzzbee_GoBehind_Beespy)
    // Wait(15)
    Call(YieldTurn)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_Creepy = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActorPart, ACTOR_SELF, PRT_CREEPY)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 0)
    Add(LVar1, -20)
    Add(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, PRT_CREEPY, LVar0, LVar1, LVar2)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_CREEPY, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_Creepy)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_CREEPY, Float(6.0))
    Call(SetPartJumpGravity, ACTOR_SELF, PRT_CREEPY, Float(1.8))
    Set(LVar0, 0)
    Set(LVar1, 0)
    Set(LVar2, 0)
    Call(JumpPartTo, ACTOR_SELF, PRT_CREEPY, LVar0, LVar1, LVar2, 15, true)
    Wait(5)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 50)
    Set(LVar1, 0)
    SetConst(LVarA, PRT_CREEPY)
    ExecWait(EVS_EnemyPart_HopToPos)
    Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_Creepy)
    Label(100)
    Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_CreepyMove)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_IGNORE_DEFENSE, 0, 2, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 5)
            Set(LVar1, 0)
            Sub(LVar2, 5)
            Call(SetPartJumpGravity, ACTOR_SELF, PRT_CREEPY, Float(3.0))
            Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_CreepyMove)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpPartTo, ACTOR_SELF, PRT_CREEPY, LVar0, LVar1, LVar2, 11, true)
            Call(SetPartDispOffset, ACTOR_SELF, PRT_CREEPY, 0, 0, 0)
            Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_CreepyMove)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 20)
            Call(SetPartJumpGravity, ACTOR_SELF, PRT_CREEPY, Float(3.0))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpPartTo, ACTOR_SELF, PRT_CREEPY, LVar0, LVar1, LVar2, 6, true)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 15)
            Call(SetPartJumpGravity, ACTOR_SELF, PRT_CREEPY, Float(3.0))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpPartTo, ACTOR_SELF, PRT_CREEPY, LVar0, LVar1, LVar2, 5, true)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Wait(4)
            Wait(5)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(5)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(YieldTurn)
            Call(SetPartYaw, ACTOR_SELF, PRT_CREEPY, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_CREEPY, 0, ACTOR_DECORATION_SWEAT)
            Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_Creepy)
            Add(LVar0, 300)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetPartMoveSpeed, ACTOR_SELF, PRT_CREEPY, Float(8.0))
            Call(SetPartJumpGravity, ACTOR_SELF, PRT_CREEPY, Float(1.8))
            SetConst(LVarA, PRT_CREEPY)
            ExecWait(EVS_EnemyPart_HopToPos)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_CREEPY, 0)
            Call(SetPartYaw, ACTOR_SELF, PRT_CREEPY, 0)
            Call(SetPartFlagBits, ACTOR_SELF, PRT_CREEPY, ACTOR_PART_FLAG_INVISIBLE, true)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
        CaseEq(HIT_RESULT_HIT_STATIC)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfFlag(LVar0, STATUS_FLAG_STATIC)
                Set(LFlag0, false)
            Else
                Set(LFlag0, true)
            EndIf
    EndSwitch
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(AddGoalPos, ACTOR_SELF, -3, -12, -15)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartJumpGravity, ACTOR_SELF, PRT_CREEPY, Float(3.0))
    Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_CreepyMove)
    Call(JumpPartTo, ACTOR_SELF, PRT_CREEPY, LVar0, LVar1, LVar2, 8, true)
    Call(UseBattleCamPreset, BTL_CAM_ACTOR_CLOSE)
    Call(BattleCamTargetActor, ACTOR_PLAYER)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_AVG, BTL_CAM_XADJ_AVG, false)
    Call(SetGoalToTarget, ACTOR_SELF)
    IfEq(LFlag0, true)
        Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_IGNORE_DEFENSE, 0, 0, 0, BS_FLAGS1_TRIGGER_EVENTS)
        Return
    EndIf
    Call(GetStatusFlags, ACTOR_PLAYER, LVar1)
    IfFlag(LVar1, STATUS_FLAG_STONE)
        Wait(10)
        Loop(2)
            Call(PlaySoundAtPart, ACTOR_SELF, PRT_CREEPY, SOUND_LEECH)
            Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_CreepyMove)
            Wait(21)
            Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_Creepy)
            Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 330)
            Call(PlaySoundAtPart, ACTOR_SELF, PRT_CREEPY, SOUND_LEECH)
            Call(SetPartDispOffset, ACTOR_SELF, PRT_CREEPY, -3, -2, 0)
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(1.0), Float(1.0), Float(1.0))
            Wait(1)
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(0.9), Float(1.2), Float(1.0))
            Wait(1)
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(0.8), Float(1.3), Float(1.0))
            Wait(1)
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(0.7), Float(1.4), Float(1.0))
            Wait(1)
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(0.7), Float(1.5), Float(1.0))
            Wait(2)
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(0.6), Float(1.6), Float(1.0))
            Wait(10)
            Wait(2)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(EnemyDamageTarget, ACTOR_SELF, LVarA, DAMAGE_TYPE_IGNORE_DEFENSE, 0, 0, dmgCreepy, BS_FLAGS1_NICE_HIT)
            Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 0)
            Call(SetPartDispOffset, ACTOR_SELF, PRT_CREEPY, 0, 8, 0)
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(1.0), Float(1.0), Float(1.0))
            Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_CreepyMove)
            Wait(30)
        EndLoop
    Else
        Call(ShowMessageBox, BTL_MSG_ACTION_TIP_MASH_BUTTON, 32767)
        Call(ShowActionHud, true)
        Call(LoadActionCommand, ACTION_COMMAND_STOP_LEECH)
        Call(action_command_stop_leech_init)
        Call(SetupMashMeter, 1, 23, 0, 0, 0, 0)
        Wait(10)
        Call(SetBattleFlagBits, BS_FLAGS1_4000, false)
        Call(action_command_stop_leech_start, 0, 32767, 3)
        Set(LVar9, 0)
        Loop(5)
            Call(UseIdleAnimation, ACTOR_PLAYER, false)
            Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_MarioB1_Leeching)
            Call(PlaySoundAtPart, ACTOR_SELF, PRT_CREEPY, SOUND_LEECH)
            Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_CreepyMove)
            Set(LFlag0, false)
            Loop(21)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_Creepy)
            Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 330)
            Call(PlaySoundAtPart, ACTOR_SELF, PRT_CREEPY, SOUND_LEECH)
            Call(SetPartDispOffset, ACTOR_SELF, PRT_CREEPY, -3, -2, 0)
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(1.0), Float(1.0), Float(1.0))
            Set(LFlag0, false)
            Loop(1)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(0.9), Float(1.2), Float(1.0))
            Set(LFlag0, false)
            Loop(1)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(0.8), Float(1.3), Float(1.0))
            Set(LFlag0, false)
            Loop(1)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(0.7), Float(1.4), Float(1.0))
            Set(LFlag0, false)
            Loop(1)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(0.7), Float(1.5), Float(1.0))
            Set(LFlag0, false)
            Loop(2)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(0.6), Float(1.6), Float(1.0))
            Set(LFlag0, false)
            Loop(10)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Set(LFlag0, false)
            Loop(2)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(UseIdleAnimation, ACTOR_PLAYER, true)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(EnemyDamageTarget, ACTOR_SELF, LVarA, DAMAGE_TYPE_UNBLOCKABLE | DAMAGE_TYPE_IGNORE_DEFENSE, 0, 0, dmgCreepy, BS_FLAGS1_NICE_HIT)
            Call(GetLastDamage, ACTOR_PLAYER, LVar3)
            IfNe(LVar3, 0)
                // If it does damage.
            Else
                Add(LVar9, 1)
                IfGt(LVar9, 1)
                    Call(InterruptLeechActionCommand)
                    BreakLoop
                EndIf
            EndIf
            Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 0)
            Call(SetPartDispOffset, ACTOR_SELF, PRT_CREEPY, 0, 8, 0)
            Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(1.0), Float(1.0), Float(1.0))
            Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_CreepyMove)
            Call(GetPlayerHP, LVar1)
            IfEq(LVar1, 0)
                Call(InterruptLeechActionCommand)
                BreakLoop
            EndIf
            Set(LFlag0, false)
            Loop(35)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                BreakLoop
            EndIf
        EndLoop
        Call(InterruptLeechActionCommand)
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 0)
        Call(SetPartDispOffset, ACTOR_SELF, PRT_CREEPY, 0, 8, 0)
        Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(1.0), Float(1.0), Float(1.0))
        Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_CreepyMove)
    EndIf
    Call(SetPartDispOffset, ACTOR_SELF, PRT_CREEPY, 0, 0, 0)
    Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_Creepy)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Thread
        Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(1.6), Float(0.6), Float(1.0))
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 330)
        Wait(1)
        Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(0.7), Float(1.5), Float(1.0))
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 300)
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 270)
        Wait(1)
        Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(1.0), Float(0.7), Float(1.0))
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 240)
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 210)
        Wait(1)
        Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(0.7), Float(1.4), Float(1.0))
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 180)
        Wait(2)
        Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(1.0), Float(0.7), Float(1.0))
        Wait(2)
        Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(1.2), Float(0.5), Float(1.0))
    EndThread
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 40)
    Set(LVar1, 0)
    Call(SetPartJumpGravity, ACTOR_SELF, PRT_CREEPY, Float(1.8))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpPartTo, ACTOR_SELF, PRT_CREEPY, LVar0, LVar1, LVar2, 10, true)
    Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(1.4), Float(0.2), Float(1.0))
    Wait(1)
    Thread
        Wait(1)
        Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(1.2), Float(0.5), Float(1.0))
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 150)
        Wait(1)
        Call(SetPartScale, ACTOR_SELF, PRT_CREEPY, Float(1.0), Float(1.0), Float(1.0))
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 120)
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 90)
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 60)
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 30)
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_CREEPY, 0, 0, 0)
    EndThread
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 30)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpPartTo, ACTOR_SELF, PRT_CREEPY, LVar0, LVar1, LVar2, 8, true)
    Wait(5)
    Call(YieldTurn)
    Call(SetPartDispOffset, ACTOR_SELF, PRT_CREEPY, 0, 0, 0)
    Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_Creepy)
    Add(LVar0, 300)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_CREEPY, Float(8.0))
    Call(SetPartJumpGravity, ACTOR_SELF, PRT_CREEPY, Float(1.8))
    Call(SetPartYaw, ACTOR_SELF, PRT_CREEPY, 180)
    SetConst(LVarA, PRT_CREEPY)
    ExecWait(EVS_EnemyPart_HopToPos)
    Call(SetAnimation, ACTOR_SELF, PRT_CREEPY, ANIM_InsectsDK3_Creepy)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_CREEPY, ACTOR_PART_FLAG_INVISIBLE, true)
    Call(SetPartYaw, ACTOR_SELF, PRT_CREEPY, 0)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 0)
    Add(LVar1, -20)
    Add(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, PRT_CREEPY, LVar0, LVar1, LVar2)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};


EvtScript EVS_Attack_VineEater = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActorPart, ACTOR_SELF, PRT_VINE_EATER)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 0)
    Add(LVar1, -20)
    Add(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, PRT_VINE_EATER, LVar0, LVar1, LVar2)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_VINE_EATER, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEater)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_VINE_EATER, Float(6.0))
    Call(SetPartJumpGravity, ACTOR_SELF, PRT_VINE_EATER, Float(1.8))
    Set(LVar0, 0)
    Set(LVar1, 0)
    Set(LVar2, 0)
    Call(JumpPartTo, ACTOR_SELF, PRT_VINE_EATER, LVar0, LVar1, LVar2, 15, true)
    Wait(5)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 50)
    Set(LVar1, 0)
    SetConst(LVarA, PRT_VINE_EATER)
    ExecWait(EVS_EnemyPart_HopToPos)
    Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEater)
    Label(100)
    Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEaterMove)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_IGNORE_DEFENSE, 0, 2, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 5)
            Set(LVar1, 0)
            Sub(LVar2, 5)
            Call(SetPartJumpGravity, ACTOR_SELF, PRT_VINE_EATER, Float(3.0))
            Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEaterMove)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpPartTo, ACTOR_SELF, PRT_VINE_EATER, LVar0, LVar1, LVar2, 11, true)
            Call(SetPartDispOffset, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 0)
            Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEaterMove)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 20)
            Call(SetPartJumpGravity, ACTOR_SELF, PRT_VINE_EATER, Float(3.0))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpPartTo, ACTOR_SELF, PRT_VINE_EATER, LVar0, LVar1, LVar2, 6, true)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 15)
            Call(SetPartJumpGravity, ACTOR_SELF, PRT_VINE_EATER, Float(3.0))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpPartTo, ACTOR_SELF, PRT_VINE_EATER, LVar0, LVar1, LVar2, 5, true)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Wait(4)
            Wait(5)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(5)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(YieldTurn)
            Call(SetPartYaw, ACTOR_SELF, PRT_VINE_EATER, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_VINE_EATER, 0, ACTOR_DECORATION_SWEAT)
            Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEater)
            Add(LVar0, 300)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetPartMoveSpeed, ACTOR_SELF, PRT_VINE_EATER, Float(8.0))
            Call(SetPartJumpGravity, ACTOR_SELF, PRT_VINE_EATER, Float(1.8))
            SetConst(LVarA, PRT_VINE_EATER)
            ExecWait(EVS_EnemyPart_HopToPos)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_VINE_EATER, 0)
            Call(SetPartYaw, ACTOR_SELF, PRT_VINE_EATER, 0)
            Call(SetPartFlagBits, ACTOR_SELF, PRT_VINE_EATER, ACTOR_PART_FLAG_INVISIBLE, true)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
        CaseEq(HIT_RESULT_HIT_STATIC)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfFlag(LVar0, STATUS_FLAG_STATIC)
                Set(LFlag0, false)
            Else
                Set(LFlag0, true)
            EndIf
    EndSwitch
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(AddGoalPos, ACTOR_SELF, -3, -12, -15)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartJumpGravity, ACTOR_SELF, PRT_VINE_EATER, Float(3.0))
    Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEaterMove)
    Call(JumpPartTo, ACTOR_SELF, PRT_VINE_EATER, LVar0, LVar1, LVar2, 8, true)
    Call(UseBattleCamPreset, BTL_CAM_ACTOR_CLOSE)
    Call(BattleCamTargetActor, ACTOR_PLAYER)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_AVG, BTL_CAM_XADJ_AVG, false)
    Call(SetGoalToTarget, ACTOR_SELF)
    IfEq(LFlag0, true)
        Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_IGNORE_DEFENSE, 0, 0, 0, BS_FLAGS1_TRIGGER_EVENTS)
        Return
    EndIf
    Call(GetStatusFlags, ACTOR_PLAYER, LVar1)
    IfFlag(LVar1, STATUS_FLAG_STONE)
        Wait(10)
        Loop(2)
            Call(PlaySoundAtPart, ACTOR_SELF, PRT_VINE_EATER, SOUND_LEECH)
            Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEaterMove)
            Wait(21)
            Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEater)
            Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 330)
            Call(PlaySoundAtPart, ACTOR_SELF, PRT_VINE_EATER, SOUND_LEECH)
            Call(SetPartDispOffset, ACTOR_SELF, PRT_VINE_EATER, -3, -2, 0)
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(1.0), Float(1.0), Float(1.0))
            Wait(1)
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(0.9), Float(1.2), Float(1.0))
            Wait(1)
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(0.8), Float(1.3), Float(1.0))
            Wait(1)
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(0.7), Float(1.4), Float(1.0))
            Wait(1)
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(0.7), Float(1.5), Float(1.0))
            Wait(2)
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(0.6), Float(1.6), Float(1.0))
            Wait(10)
            Wait(2)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(EnemyDamageTarget, ACTOR_SELF, LVarA, DAMAGE_TYPE_IGNORE_DEFENSE, 0, 0, dmgVineEater, BS_FLAGS1_NICE_HIT)
            Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 0)
            Call(SetPartDispOffset, ACTOR_SELF, PRT_VINE_EATER, 0, 8, 0)
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(1.0), Float(1.0), Float(1.0))
            Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEaterMove)
            Wait(30)
        EndLoop
    Else
        Call(ShowMessageBox, BTL_MSG_ACTION_TIP_MASH_BUTTON, 32767)
        Call(ShowActionHud, true)
        Call(LoadActionCommand, ACTION_COMMAND_STOP_LEECH)
        Call(action_command_stop_leech_init)
        Call(SetupMashMeter, 1, 23, 0, 0, 0, 0)
        Wait(10)
        Call(SetBattleFlagBits, BS_FLAGS1_4000, false)
        Call(action_command_stop_leech_start, 0, 32767, 3)
        Set(LVar9, 0)
        Loop(5)
            Call(UseIdleAnimation, ACTOR_PLAYER, false)
            Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_MarioB1_Leeching)
            Call(PlaySoundAtPart, ACTOR_SELF, PRT_VINE_EATER, SOUND_LEECH)
            Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEaterMove)
            Set(LFlag0, false)
            Loop(21)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEater)
            Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 330)
            Call(PlaySoundAtPart, ACTOR_SELF, PRT_VINE_EATER, SOUND_LEECH)
            Call(SetPartDispOffset, ACTOR_SELF, PRT_VINE_EATER, -3, -2, 0)
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(1.0), Float(1.0), Float(1.0))
            Set(LFlag0, false)
            Loop(1)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(0.9), Float(1.2), Float(1.0))
            Set(LFlag0, false)
            Loop(1)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(0.8), Float(1.3), Float(1.0))
            Set(LFlag0, false)
            Loop(1)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(0.7), Float(1.4), Float(1.0))
            Set(LFlag0, false)
            Loop(1)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(0.7), Float(1.5), Float(1.0))
            Set(LFlag0, false)
            Loop(2)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(0.6), Float(1.6), Float(1.0))
            Set(LFlag0, false)
            Loop(10)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Set(LFlag0, false)
            Loop(2)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                Call(UseIdleAnimation, ACTOR_PLAYER, true)
                BreakLoop
            EndIf
            Call(UseIdleAnimation, ACTOR_PLAYER, true)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(EnemyDamageTarget, ACTOR_SELF, LVarA, DAMAGE_TYPE_UNBLOCKABLE | DAMAGE_TYPE_IGNORE_DEFENSE, 0, 0, dmgVineEater, BS_FLAGS1_NICE_HIT)
            Call(GetLastDamage, ACTOR_PLAYER, LVar3)
            IfNe(LVar3, 0)
                // If it does damage.
            Else
                Add(LVar9, 1)
                IfGt(LVar9, 1)
                    Call(InterruptLeechActionCommand)
                    BreakLoop
                EndIf
            EndIf
            Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 0)
            Call(SetPartDispOffset, ACTOR_SELF, PRT_VINE_EATER, 0, 8, 0)
            Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(1.0), Float(1.0), Float(1.0))
            Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEaterMove)
            Call(GetPlayerHP, LVar1)
            IfEq(LVar1, 0)
                Call(InterruptLeechActionCommand)
                BreakLoop
            EndIf
            Set(LFlag0, false)
            Loop(35)
                Call(GetMashActionQuality, LVar1)
                IfEq(LVar1, 1)
                    Set(LFlag0, true)
                    BreakLoop
                EndIf
                Wait(1)
            EndLoop
            IfEq(LFlag0, true)
                BreakLoop
            EndIf
        EndLoop
        Call(InterruptLeechActionCommand)
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 0)
        Call(SetPartDispOffset, ACTOR_SELF, PRT_VINE_EATER, 0, 8, 0)
        Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(1.0), Float(1.0), Float(1.0))
        Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEaterMove)
    EndIf
    Call(SetPartDispOffset, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 0)
    Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEater)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Thread
        Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(1.6), Float(0.6), Float(1.0))
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 330)
        Wait(1)
        Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(0.7), Float(1.5), Float(1.0))
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 300)
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 270)
        Wait(1)
        Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(1.0), Float(0.7), Float(1.0))
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 240)
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 210)
        Wait(1)
        Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(0.7), Float(1.4), Float(1.0))
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 180)
        Wait(2)
        Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(1.0), Float(0.7), Float(1.0))
        Wait(2)
        Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(1.2), Float(0.5), Float(1.0))
    EndThread
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 40)
    Set(LVar1, 0)
    Call(SetPartJumpGravity, ACTOR_SELF, PRT_VINE_EATER, Float(1.8))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpPartTo, ACTOR_SELF, PRT_VINE_EATER, LVar0, LVar1, LVar2, 10, true)
    Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(1.4), Float(0.2), Float(1.0))
    Wait(1)
    Thread
        Wait(1)
        Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(1.2), Float(0.5), Float(1.0))
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 150)
        Wait(1)
        Call(SetPartScale, ACTOR_SELF, PRT_VINE_EATER, Float(1.0), Float(1.0), Float(1.0))
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 120)
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 90)
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 60)
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 30)
        Wait(1)
        Call(SetPartRotation, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 0)
    EndThread
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 30)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpPartTo, ACTOR_SELF, PRT_VINE_EATER, LVar0, LVar1, LVar2, 8, true)
    Wait(5)
    Call(YieldTurn)
    Call(SetPartDispOffset, ACTOR_SELF, PRT_VINE_EATER, 0, 0, 0)
    Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEater)
    Add(LVar0, 300)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartMoveSpeed, ACTOR_SELF, PRT_VINE_EATER, Float(8.0))
    Call(SetPartJumpGravity, ACTOR_SELF, PRT_VINE_EATER, Float(1.8))
    Call(SetPartYaw, ACTOR_SELF, PRT_VINE_EATER, 180)
    SetConst(LVarA, PRT_VINE_EATER)
    ExecWait(EVS_EnemyPart_HopToPos)
    Call(SetAnimation, ACTOR_SELF, PRT_VINE_EATER, ANIM_InsectsDK3_VineEater)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_VINE_EATER, ACTOR_PART_FLAG_INVISIBLE, true)
    Call(SetPartYaw, ACTOR_SELF, PRT_VINE_EATER, 0)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 0)
    Add(LVar1, -20)
    Add(LVar2, 2)
    Call(SetPartPos, ACTOR_SELF, PRT_VINE_EATER, LVar0, LVar1, LVar2)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

}; // namespace beehive

ActorBlueprint Beehive = {
    .flags = ACTOR_FLAG_UPSIDE_DOWN,
    .maxHP = beehive::hp,
    .type = ACTOR_TYPE_BEEHIVE,
    .level = ACTOR_LEVEL_BEEHIVE,
    .partCount = ARRAY_COUNT(beehive::ActorParts),
    .partsData = beehive::ActorParts,
    .initScript = &beehive::EVS_Init,
    .statusTable = beehive::StatusTable,
    .escapeChance = 50,
    .airLiftChance = 0,
    .hurricaneChance = 0,
    .spookChance = 0,
    .upAndAwayChance = 0,
    .spinSmashReq = 0,
    .powerBounceChance = 100,
    .coinReward = 0,
    .size = { 32, 32 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

}; // namespace battle::actor
