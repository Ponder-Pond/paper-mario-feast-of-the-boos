#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/ChainChomp.h"
#include "boss.hpp"
#include "train_heist_actors.hpp"
#include "dx/debug_menu.h"

namespace battle::actor {

namespace giant_chain_chomp {

extern EvtScript EVS_Init;
extern EvtScript EVS_Idle;
extern EvtScript EVS_TakeTurn;
extern EvtScript EVS_HandleEvent;
// extern EvtScript N(EVS_UpdateChain);
extern EvtScript EVS_Chomp_HopHome;
extern EvtScript EVS_Chomp_HopToPos;
extern EvtScript EVS_Attack_Bite;
extern EvtScript EVS_CounterAttack_QuickBite;

// enum N(ActorVars) {
//     AVAR_EnableChainSounds      = 8,
// };

enum ActorPartIDs {
    PRT_MAIN            = 1,
    PRT_TARGET          = 2,
    // PRT_CHAIN_1         = 3,
    // PRT_CHAIN_2         = 4,
    // PRT_CHAIN_3         = 5,
    // PRT_CHAIN_4         = 6,
    // PRT_CHAIN_5         = 7,
    // PRT_CHAIN_6         = 8,
    // PRT_CHAIN_7         = 9,
    // PRT_CHAIN_8         = 10,
};

// Actor Stats
constexpr s32 hp = 4;
constexpr s32 dmgBite = 3;
constexpr s32 dmgQuickBite = 6;

s32 BasicAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_ChainChomp_Idle,
    STATUS_KEY_STONE,     ANIM_ChainChomp_Still,
    STATUS_KEY_SLEEP,     ANIM_ChainChomp_Sleep,
    STATUS_KEY_POISON,    ANIM_ChainChomp_Idle,
    STATUS_KEY_STOP,      ANIM_ChainChomp_Still,
    STATUS_KEY_STATIC,    ANIM_ChainChomp_Still,
    STATUS_KEY_PARALYZE,  ANIM_ChainChomp_Still,
    STATUS_KEY_DIZZY,     ANIM_ChainChomp_Dizzy,
    STATUS_END,
};

s32 QuickBiteAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_ChainChomp_QuickBite,
    STATUS_KEY_POISON,    ANIM_ChainChomp_QuickBite,
    STATUS_KEY_STOP,      ANIM_ChainChomp_Still,
    STATUS_KEY_STONE,     ANIM_ChainChomp_Still,
    STATUS_KEY_SLEEP,     ANIM_ChainChomp_Sleep,
    STATUS_KEY_STATIC,    ANIM_ChainChomp_Still,
    STATUS_KEY_PARALYZE,  ANIM_ChainChomp_Still,
    STATUS_KEY_DIZZY,     ANIM_ChainChomp_Dizzy,
    STATUS_END,
};

s32 BiteAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_ChainChomp_Bite,
    STATUS_KEY_POISON,    ANIM_ChainChomp_Bite,
    STATUS_KEY_STOP,      ANIM_ChainChomp_Still,
    STATUS_KEY_STONE,     ANIM_ChainChomp_Still,
    STATUS_KEY_SLEEP,     ANIM_ChainChomp_Sleep,
    STATUS_KEY_STATIC,    ANIM_ChainChomp_Still,
    STATUS_KEY_PARALYZE,  ANIM_ChainChomp_Still,
    STATUS_KEY_DIZZY,     ANIM_ChainChomp_Dizzy,
    STATUS_END,
};

s32 SlowBiteAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_ChainChomp_SlowBite,
    STATUS_KEY_POISON,    ANIM_ChainChomp_SlowBite,
    STATUS_KEY_STOP,      ANIM_ChainChomp_Still,
    STATUS_KEY_STONE,     ANIM_ChainChomp_Still,
    STATUS_KEY_SLEEP,     ANIM_ChainChomp_Sleep,
    STATUS_KEY_STATIC,    ANIM_ChainChomp_Still,
    STATUS_KEY_PARALYZE,  ANIM_ChainChomp_Still,
    STATUS_KEY_DIZZY,     ANIM_ChainChomp_Dizzy,
    STATUS_END,
};

s32 ChainAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_ChainChomp_Chain,
    STATUS_END,
};

s32 DefenseTable[] = {
    ELEMENT_NORMAL,   100,
    ELEMENT_END,
};

s32 StatusTable[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,               0,
    STATUS_KEY_POISON,              0,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,              100,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,              0,
    STATUS_KEY_PARALYZE,            0,
    STATUS_KEY_SHRINK,              0,
    STATUS_KEY_STOP,                0,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,          0,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,         -1,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,       0,
    STATUS_TURN_MOD_SHRINK,         0,
    STATUS_TURN_MOD_STOP,           0,
    STATUS_END,
};

s32 StatusTableGC[] = {
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
        .flags = ACTOR_PART_FLAG_NO_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 48 },
        .opacity = 255,
        .idleAnimations = BasicAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
    },
    {
        .flags = ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION | ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_TARGET,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { -13, 80 },
        .opacity = 255,
        .idleAnimations = BasicAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -16 },
    },
    // {
    //     .flags = ACTOR_PART_FLAG_NO_DECORATIONS | ACTOR_PART_FLAG_TARGET_ONLY | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
    //     .index = PRT_CHAIN_1,
    //     .posOffset = { 0, 0, 0 },
    //     .targetOffset = { 0, 12 },
    //     .opacity = 255,
    //     .idleAnimations = ChainAnims,
    //     .defenseTable = DefenseTable,
    //     .eventFlags = ACTOR_EVENT_FLAGS_NONE,
    //     .elementImmunityFlags = 0,
    //     .projectileTargetOffset = { 0, 0 },
    // },
    // {
    //     .flags = ACTOR_PART_FLAG_NO_DECORATIONS | ACTOR_PART_FLAG_TARGET_ONLY | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
    //     .index = PRT_CHAIN_2,
    //     .posOffset = { 0, 0, 0 },
    //     .targetOffset = { 0, 12 },
    //     .opacity = 255,
    //     .idleAnimations = ChainAnims,
    //     .defenseTable = DefenseTable,
    //     .eventFlags = ACTOR_EVENT_FLAGS_NONE,
    //     .elementImmunityFlags = 0,
    //     .projectileTargetOffset = { 0, 0 },
    // },
    // {
    //     .flags = ACTOR_PART_FLAG_NO_DECORATIONS | ACTOR_PART_FLAG_TARGET_ONLY | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
    //     .index = PRT_CHAIN_3,
    //     .posOffset = { 0, 0, 0 },
    //     .targetOffset = { 0, 12 },
    //     .opacity = 255,
    //     .idleAnimations = ChainAnims,
    //     .defenseTable = DefenseTable,
    //     .eventFlags = ACTOR_EVENT_FLAGS_NONE,
    //     .elementImmunityFlags = 0,
    //     .projectileTargetOffset = { 0, 0 },
    // },
    // {
    //     .flags = ACTOR_PART_FLAG_NO_DECORATIONS | ACTOR_PART_FLAG_TARGET_ONLY | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
    //     .index = PRT_CHAIN_4,
    //     .posOffset = { 0, 0, 0 },
    //     .targetOffset = { 0, 12 },
    //     .opacity = 255,
    //     .idleAnimations = ChainAnims,
    //     .defenseTable = DefenseTable,
    //     .eventFlags = ACTOR_EVENT_FLAGS_NONE,
    //     .elementImmunityFlags = 0,
    //     .projectileTargetOffset = { 0, 0 },
    // },
    // {
    //     .flags = ACTOR_PART_FLAG_NO_DECORATIONS | ACTOR_PART_FLAG_TARGET_ONLY | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
    //     .index = PRT_CHAIN_5,
    //     .posOffset = { 0, 0, 0 },
    //     .targetOffset = { 0, 12 },
    //     .opacity = 255,
    //     .idleAnimations = ChainAnims,
    //     .defenseTable = DefenseTable,
    //     .eventFlags = ACTOR_EVENT_FLAGS_NONE,
    //     .elementImmunityFlags = 0,
    //     .projectileTargetOffset = { 0, 0 },
    // },
    // {
    //     .flags = ACTOR_PART_FLAG_NO_DECORATIONS | ACTOR_PART_FLAG_TARGET_ONLY | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
    //     .index = PRT_CHAIN_6,
    //     .posOffset = { 0, 0, 0 },
    //     .targetOffset = { 0, 12 },
    //     .opacity = 255,
    //     .idleAnimations = ChainAnims,
    //     .defenseTable = DefenseTable,
    //     .eventFlags = ACTOR_EVENT_FLAGS_NONE,
    //     .elementImmunityFlags = 0,
    //     .projectileTargetOffset = { 0, 0 },
    // },
    // {
    //     .flags = ACTOR_PART_FLAG_NO_DECORATIONS | ACTOR_PART_FLAG_TARGET_ONLY | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
    //     .index = PRT_CHAIN_7,
    //     .posOffset = { 0, 0, 0 },
    //     .targetOffset = { 0, 12 },
    //     .opacity = 255,
    //     .idleAnimations = ChainAnims,
    //     .defenseTable = DefenseTable,
    //     .eventFlags = ACTOR_EVENT_FLAGS_NONE,
    //     .elementImmunityFlags = 0,
    //     .projectileTargetOffset = { 0, 0 },
    // },
    // {
    //     .flags = ACTOR_PART_FLAG_NO_DECORATIONS | ACTOR_PART_FLAG_TARGET_ONLY | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION,
    //     .index = PRT_CHAIN_8,
    //     .posOffset = { 0, 0, 0 },
    //     .targetOffset = { 0, 12 },
    //     .opacity = 255,
    //     .idleAnimations = ChainAnims,
    //     .defenseTable = DefenseTable,
    //     .eventFlags = ACTOR_EVENT_FLAGS_NONE,
    //     .elementImmunityFlags = 0,
    //     .projectileTargetOffset = { 0, 0 },
    // },
};

// #define CHOMP_CHAIN_FIRST_PART_IDX  PRT_CHAIN_1
// #define CHOMP_CHAIN_LAST_PART_IDX   PRT_CHAIN_8
// #define CHOMP_CHAIN_AVAR_SOUNDS     AVAR_EnableChainSounds
// #define CHOMP_CHAIN_UPDATE_Z        true
// #include "common/battle/ChompChainSupport.inc.c"

EvtScript EVS_Init = {
    // Call(SetActorVar, ACTOR_SELF, AVAR_EnableChainSounds, false)
    Call(SetActorScale, ACTOR_SELF, Float(2.0), Float(2.0), Float(1.0))
    Call(BindTakeTurn, ACTOR_SELF, Ref(EVS_TakeTurn))
    Call(BindIdle, ACTOR_SELF, Ref(EVS_Idle))
    Call(BindHandleEvent, ACTOR_SELF, Ref(EVS_HandleEvent))
    // Call(SetActorPos, ACTOR_SELF, NPC_DISPOSE_LOCATION)
    // Call(ForceHomePos, ACTOR_SELF, NPC_DISPOSE_LOCATION)
    // Call(HPBarToHome, ACTOR_SELF)
    // Call(SetPartFlagBits, ACTOR_SELF, PRT_MAIN, ACTOR_PART_FLAG_INVISIBLE | ACTOR_PART_FLAG_NO_TARGET, true)
    // Call(SetPartFlagBits, ACTOR_SELF, PRT_TARGET, ACTOR_PART_FLAG_NO_TARGET, true)
    // Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_ATTACK | ACTOR_FLAG_SKIP_TURN, true)
    // Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    // Call(SetPartPos, ACTOR_SELF, PRT_TARGET, LVar0, LVar1, LVar2)
    // Set(LVar0, PRT_CHAIN_1)
    // Loop(1 + (PRT_CHAIN_8 - PRT_CHAIN_1))
    //     Call(SetPartSize, ACTOR_SELF, LVar0, 16, 16)
    //     Add(LVar0, 1)
    // EndLoop
    // Call(N(ChompChainInit))
    // Exec(N(EVS_UpdateChain))
    // Call(GetOwnerID, LVar9)
    // DebugPrintf("Giant Chain Chomp Actor ID: (%d)\n", LVar9)
    Return
    End
};

EvtScript EVS_UpdateTargetPartPos = {
    Call(GetStatusFlags, ACTOR_SELF, LVar0)
    IfFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetPartPos, ACTOR_SELF, PRT_TARGET, LVar0, LVar1, LVar2)
        Call(SetTargetOffset, ACTOR_SELF, PRT_TARGET, 0, 48)
        Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_TARGET, 0, 0)
    Else
        Call(SetIdleGoalToHome, ACTOR_SELF)
        Call(GetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetPartPos, ACTOR_SELF, PRT_TARGET, LVar0, LVar1, LVar2)
        Call(SetTargetOffset, ACTOR_SELF, PRT_TARGET, -13, 80)
        Call(SetProjectileTargetOffset, ACTOR_SELF, PRT_TARGET, 0, -16)
    EndIf
    Return
    End
};

EvtScript EVS_Idle = {
    Label(0)
        Loop(0)
            ExecWait(EVS_UpdateTargetPartPos)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                BreakLoop
            EndIf
            Wait(1)
        EndLoop
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(SlowBiteAnims))
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(1.0))
        Call(SetIdleGoalToHome, ACTOR_SELF)
        Call(GetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar0, 10)
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(0.8))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleJumpToGoal, ACTOR_SELF, 11, 1)
        Loop(0)
            ExecWait(EVS_UpdateTargetPartPos)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                BreakLoop
            EndIf
            Wait(1)
        EndLoop
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(1.0))
        Call(SetIdleGoalToHome, ACTOR_SELF)
        Call(GetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleJumpToGoal, ACTOR_SELF, 6, 1)
        Call(RandInt, 10, LVar0)
        Add(LVar0, 1)
        Wait(LVar0)
        Loop(0)
            ExecWait(EVS_UpdateTargetPartPos)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                BreakLoop
            EndIf
            Wait(1)
        EndLoop
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(BiteAnims))
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(0.8))
        Call(SetIdleGoalToHome, ACTOR_SELF)
        Call(GetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleJumpToGoal, ACTOR_SELF, 15, 1)
        Loop(0)
            ExecWait(EVS_UpdateTargetPartPos)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                BreakLoop
            EndIf
            Wait(1)
        EndLoop
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(SlowBiteAnims))
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(0.8))
        Call(SetIdleGoalToHome, ACTOR_SELF)
        Call(GetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Sub(LVar0, 20)
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleJumpToGoal, ACTOR_SELF, 12, 1)
        Loop(0)
            ExecWait(EVS_UpdateTargetPartPos)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                BreakLoop
            EndIf
            Wait(1)
        EndLoop
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(0.8))
        Call(SetIdleGoalToHome, ACTOR_SELF)
        Call(GetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Sub(LVar0, 10)
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleJumpToGoal, ACTOR_SELF, 9, 1)
        Loop(0)
            ExecWait(EVS_UpdateTargetPartPos)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                BreakLoop
            EndIf
            Wait(1)
        EndLoop
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(QuickBiteAnims))
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(0.8))
        Call(SetIdleGoalToHome, ACTOR_SELF)
        Call(GetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleJumpToGoal, ACTOR_SELF, 6, 1)
        Call(RandInt, 10, LVar0)
        Add(LVar0, 10)
        Wait(LVar0)
        Goto(0)
    Return
    End
};

// EvtScript N(EVS_UpdateChain) = {
//     Label(0)
//         Wait(1)
//         Call(ActorExists, ACTOR_GIANT_CHOMP, LVar0)
//         IfEq(LVar0, true)
//             Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
//             Call(N(ChompChainUpdate), LVar2)
//             Goto(0)
//         EndIf
//     Label(1)
//     Return
//     End
// };

EvtScript EVS_HandleEvent = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Set(LVarF, LVar0)
    Switch(LVar0)
        CaseEq(EVENT_HIT_COMBO)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            ExecWait(EVS_Enemy_Hit)
        CaseEq(EVENT_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            ExecWait(EVS_Enemy_Hit)
            Call(ActorExists, ACTOR_GIANT_CHOMP, LVar0)
            IfEq(LVar0, true)
                Call(GetStatusFlags, ACTOR_GIANT_CHOMP, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_DIZZY)
                    Call(SetPartFlagBits, ACTOR_YELLOW_BANDIT, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, true)
                    Call(SetStatusTable, ACTOR_YELLOW_BANDIT, Ref(StatusTable))
                    Call(SetPartFlagBits, ACTOR_YELLOW_HAMMER_BRO, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, true)
                    Call(SetStatusTable, ACTOR_YELLOW_HAMMER_BRO, Ref(StatusTable))
                Else
                    Call(SetPartFlagBits, ACTOR_YELLOW_BANDIT, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, false)
                    Call(SetStatusTable, ACTOR_YELLOW_BANDIT, Ref(StatusTableGC))
                    Call(SetPartFlagBits, ACTOR_YELLOW_HAMMER_BRO, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, false)
                    Call(SetStatusTable, ACTOR_YELLOW_HAMMER_BRO, Ref(StatusTableGC))
                EndIf
            Else
                Call(SetPartFlagBits, ACTOR_YELLOW_BANDIT, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, false)
                Call(SetStatusTable, ACTOR_YELLOW_BANDIT, Ref(StatusTableGC))
                Call(SetPartFlagBits, ACTOR_YELLOW_HAMMER_BRO, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, false)
                Call(SetStatusTable, ACTOR_YELLOW_HAMMER_BRO, Ref(StatusTableGC))
            EndIf
        CaseEq(EVENT_BEGIN_FIRST_STRIKE)
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            SetConst(LVar2, -1)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            SetConst(LVar2, -1)
            ExecWait(EVS_Enemy_BurnHit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            // ExecWait(N(EVS_Chomp_SpinSmashHit))
            ExecWait(EVS_Enemy_SpinSmashHit)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(SetAnimation, ACTOR_SELF, LVar0, LVar1)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Set(LVar1, 0)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.4))
            Call(AddGoalPos, ACTOR_SELF, 30, 0, 0)
            Call(JumpToGoal, ACTOR_SELF, 15, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Call(AddGoalPos, ACTOR_SELF, 15, 0, 0)
            Call(JumpToGoal, ACTOR_SELF, 8, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Call(AddGoalPos, ACTOR_SELF, 5, 0, 0)
            Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_SlowBite)
            ExecWait(EVS_Chomp_HopHome)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.6))
            Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
            EndThread
            Call(SetGoalToHome, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetPartPos, ACTOR_SELF, PRT_TARGET, LVar0, LVar1, LVar2)
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
                Call(SetGoalToHome, ACTOR_SELF)
                Call(GetGoalPos, ACTOR_SELF, LVar1, LVar2, LVar3)
                IfFlag(LVar0, STATUS_FLAG_SHRINK)
                    Sub(LVar1, 10)
                    Add(LVar2, 4)
                Else
                    Sub(LVar1, 5)
                    Add(LVar2, 11)
                EndIf
                Call(SetActorPos, ACTOR_SELF, LVar1, LVar2, LVar3)
            EndIf
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            ExecWait(EVS_Enemy_AirLift)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        CaseEq(EVENT_BLOW_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            ExecWait(EVS_Enemy_BlowAway)
            Return
        CaseEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Idle)
            ExecWait(EVS_Enemy_NoDamageHit)
            Call(SetAnimationRate, ACTOR_SELF, PRT_MAIN, Float(1.0))
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            ExecWait(EVS_Enemy_NoDamageHit)
            Call(ActorExists, ACTOR_GIANT_CHOMP, LVar0)
            IfEq(LVar0, true)
                Call(GetStatusFlags, ACTOR_GIANT_CHOMP, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_DIZZY)
                    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Hurt)
                    Wait(50)
                    ExecWait(EVS_CounterAttack_QuickBite)
                EndIf
            Else
                Call(ClearStatusEffects, ACTOR_SELF)
            EndIf
        EndCaseGroup
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            // ExecWait(N(EVS_Chomp_SpinSmashHit))
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Hurt)
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_RECOVER_STATUS)
            Call(ActorExists, ACTOR_GIANT_CHOMP, LVar0)
            IfEq(LVar0, true)
                Call(SetPartFlagBits, ACTOR_YELLOW_BANDIT, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, true)
                Call(SetStatusTable, ACTOR_YELLOW_BANDIT, Ref(StatusTable))
                Call(SetPartFlagBits, ACTOR_YELLOW_HAMMER_BRO, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, true)
                Call(SetStatusTable, ACTOR_YELLOW_HAMMER_BRO, Ref(StatusTable))
            EndIf
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Idle)
            ExecWait(EVS_Enemy_Recover)
        CaseDefault
    EndSwitch
    Call(GetStatusFlags, ACTOR_SELF, LVar0)
    IfFlag(LVar0, STATUS_FLAG_SLEEP | STATUS_FLAG_PARALYZE | STATUS_FLAG_DIZZY | STATUS_FLAG_STONE)
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        IfNe(LVar1, 0)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
            Call(SetGoalPos, ACTOR_SELF, LVar0, 0, LVar2)
            Call(FallToGoal, ACTOR_SELF, 11)
        EndIf
    EndIf
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_TakeTurn = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    ExecWait(EVS_Attack_Bite)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_Bite = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.5))
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Set(LVar1, 0)
    Add(LVar0, 45)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 15, false, true, false)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
    Thread
        Call(GetStatusFlags, ACTOR_SELF, LVar0)
        IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
            Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
        EndIf
    EndThread
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_SlowBite)
    Wait(15)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_ACTOR_HURT)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(PlaySound, SOUND_ACTOR_HURT)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Idle)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 20)
            Set(LVar1, 0)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 10, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 25)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 6, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Sub(LVar0, 15)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 4, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Bite)
            Wait(20)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(MoveBattleCamOver, 20)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Idle)
            ExecWait(EVS_Chomp_HopHome)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Idle)
            Call(HPBarToHome, ACTOR_SELF)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
            Call(SetActorYaw, ACTOR_SELF, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
            Return
        EndCaseGroup
    EndSwitch
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Idle)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(GetStatusFlags, ACTOR_SELF, LVarA)
    IfFlag(LVarA, STATUS_FLAG_SHRINK)
        Add(LVar0, 4)
    Else
        Add(LVar0, 10)
    EndIf
    Set(LVar1, 27)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.2))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
    Wait(2)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartPos, ACTOR_SELF, PRT_TARGET, LVar0, LVar1, LVar2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, 0, 0, 0, dmgBite, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVarF)
        CaseDefault
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(MoveBattleCamOver, 20)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Bite)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar0, 40)
            Set(LVar1, 0)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 10, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Add(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 8, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Add(LVar0, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 6, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Sub(LVar0, 10)
            Call(JumpToGoal, ACTOR_SELF, 4, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Idle)
            Wait(8)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            IfEq(LVarF, HIT_RESULT_10)
                Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                Call(SetPartPos, ACTOR_SELF, PRT_TARGET, LVar0, LVar1, LVar2)
                Return
            EndIf
            Call(YieldTurn)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Idle)
            ExecWait(EVS_Chomp_HopHome)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Idle)
    EndSwitch
    Call(HPBarToHome, ACTOR_SELF)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_CounterAttack_QuickBite = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.5))
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Set(LVar1, 0)
    Add(LVar0, 45)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 7, false, true, false)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
    Thread
        Call(GetStatusFlags, ACTOR_SELF, LVar0)
        IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
            Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
        EndIf
    EndThread
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_QuickBite)
    Wait(7)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_ACTOR_HURT)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Set(LVarA, LVar0)
            Call(PlaySound, SOUND_ACTOR_HURT)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Idle)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 20)
            Set(LVar1, 0)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 25)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 3, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Sub(LVar0, 15)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 2, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Bite)
            Wait(10)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(MoveBattleCamOver, 20)
            Call(SetActorYaw, ACTOR_SELF, 180)
            Call(AddActorDecoration, ACTOR_SELF, PRT_MAIN, 0, ACTOR_DECORATION_SWEAT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Idle)
            ExecWait(EVS_Chomp_HopHome)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Idle)
            Call(HPBarToHome, ACTOR_SELF)
            Call(RemoveActorDecoration, ACTOR_SELF, PRT_MAIN, 0)
            Call(SetActorYaw, ACTOR_SELF, 0)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
            Return
        EndCaseGroup
    EndSwitch
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Idle)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(GetStatusFlags, ACTOR_SELF, LVarA)
    IfFlag(LVarA, STATUS_FLAG_SHRINK)
        Add(LVar0, 4)
    Else
        Add(LVar0, 10)
    EndIf
    Set(LVar1, 27)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.2))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
    Wait(2)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartPos, ACTOR_SELF, PRT_TARGET, LVar0, LVar1, LVar2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, 0, 0, 0, dmgQuickBite, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVarF)
        CaseDefault
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(MoveBattleCamOver, 20)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Bite)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar0, 40)
            Set(LVar1, 0)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Add(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 8, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Add(LVar0, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 3, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Sub(LVar0, 10)
            Call(JumpToGoal, ACTOR_SELF, 2, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
            Thread
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                EndIf
            EndThread
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Idle)
            Wait(8)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            IfEq(LVarF, HIT_RESULT_10)
                Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                Call(SetPartPos, ACTOR_SELF, PRT_TARGET, LVar0, LVar1, LVar2)
                Return
            EndIf
            Call(YieldTurn)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_ChainChomp_Idle)
            ExecWait(EVS_Chomp_HopHome)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Idle)
    EndSwitch
    Call(HPBarToHome, ACTOR_SELF)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};


// Custom version of EVS_Enemy_HopHome
// (in) LVar0: part idx
// (in) LVar1: hopping animID
EvtScript EVS_Chomp_HopHome = {
    Call(SetAnimation, ACTOR_SELF, LVar0, LVar1)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_ChainChomp_Bite)
    Call(SetActorSpeed, ACTOR_SELF, Float(4.0))
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.8))
    Call(SetGoalToHome, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    ExecWait(EVS_Chomp_HopToPos)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetPartPos, ACTOR_SELF, PRT_TARGET, LVar0, LVar1, LVar2)
    Call(SetActorYaw, ACTOR_SELF, 0)
    Return
    End
};

// Custom version of EVS_Enemy_HopToPos
// (in) LVar0: target posX
// (in) LVar1: target posY
// (in) LVar2: target posZ
EvtScript EVS_Chomp_HopToPos = {
    Call(GetActorPos, ACTOR_SELF, LVar3, LVar4, LVar5)
    IfLt(LVar3, LVar0)
        Call(SetActorYaw, ACTOR_SELF, 180)
    Else
        Call(SetActorYaw, ACTOR_SELF, 0)
    EndIf
    Label(0)
        Call(GetActorPos, ACTOR_SELF, LVar3, LVar4, LVar5)
        IfEq(LVar3, LVar0)
            Goto(10)
        EndIf
        IfLt(LVar3, LVar0)
            Set(LVar4, LVar0)
            Sub(LVar4, LVar3)
            IfLt(LVar4, 30)
                Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                Call(JumpToGoal, ACTOR_SELF, 6, false, true, false)
                Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
                Thread
                    Call(GetStatusFlags, ACTOR_SELF, LVar0)
                    IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                        Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                    EndIf
                EndThread
            Else
                Set(LVar4, LVar3)
                Add(LVar3, 30)
                Call(SetGoalPos, ACTOR_SELF, LVar3, LVar1, LVar2)
                Call(JumpToGoal, ACTOR_SELF, 6, false, true, false)
                Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
                Thread
                    Call(GetStatusFlags, ACTOR_SELF, LVar0)
                    IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                        Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                    EndIf
                EndThread
            EndIf
        Else
            Set(LVar4, LVar3)
            Sub(LVar4, LVar0)
            IfLt(LVar4, 30)
                Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                Call(JumpToGoal, ACTOR_SELF, 6, false, true, false)
                Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
                Thread
                    Call(GetStatusFlags, ACTOR_SELF, LVar0)
                    IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                        Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                    EndIf
                EndThread
            Else
                Sub(LVar3, 30)
                Call(SetGoalPos, ACTOR_SELF, LVar3, LVar1, LVar2)
                Call(JumpToGoal, ACTOR_SELF, 6, false, true, false)
                Call(PlaySoundAtActor, ACTOR_SELF, SOUND_CHAIN_CHOMP_THUD)
                Thread
                    Call(GetStatusFlags, ACTOR_SELF, LVar0)
                    IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                        Call(ShakeCam, CAM_BATTLE, 0, 1, Float(0.5))
                    EndIf
                EndThread
            EndIf
        EndIf
        Goto(0)
    Label(10)
    Return
    End
};

}; // namespace giant_chain_chomp

ActorBlueprint GiantChainChomp = {
    .flags = ACTOR_FLAG_NO_HEALTH_BAR,
    .maxHP = giant_chain_chomp::hp,
    .type = ACTOR_TYPE_GIANT_CHOMP,
    .level = ACTOR_LEVEL_GIANT_CHOMP,
    .partCount = ARRAY_COUNT(giant_chain_chomp::ActorParts),
    .partsData = giant_chain_chomp::ActorParts,
    .initScript = &giant_chain_chomp::EVS_Init,
    .statusTable = giant_chain_chomp::StatusTable,
    .escapeChance = 0,
    .airLiftChance = 0,
    .hurricaneChance = 0,
    .spookChance = 0,
    .upAndAwayChance = 0,
    .spinSmashReq = 0,
    .powerBounceChance = 100,
    .coinReward = 0,
    .size = { 56, 40 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -30, 40 },
    .statusTextOffset = { 10, 40 },
};

}; // namespace battle::actor
