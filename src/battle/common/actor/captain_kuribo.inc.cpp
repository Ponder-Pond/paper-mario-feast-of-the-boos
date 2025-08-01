#include "battle/battle.h"
#include "script_api/battle.h"
#include "effects.h"
#include "sprite/npc/CaptainKuribo.h"
#include "sprite/npc/Goomates.h"
#include "sprite/player.h"
// #include "goomba_king_common.h"

namespace battle::actor {

namespace captain_kuribo {

extern EvtScript EVS_Init;
extern EvtScript EVS_Idle;
extern EvtScript EVS_TakeTurn;
extern EvtScript EVS_HandleEvent;
extern EvtScript EVS_HandlePhase;
extern EvtScript EVS_ShakeScreenWhileWalking;
extern EvtScript EVS_Attack_GroundStomp;
extern EvtScript EVS_Attack_Stomp;
extern EvtScript EVS_Attack_SpinKick;
extern EvtScript EVS_Death;
extern EvtScript EVS_ReturnHome;

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
};

enum ActorVars {
    AVAR_TurnCount,
    AVAR_RedDead,
    AVAR_IncreaseShaking, // when true walking around causes more camera shaking
    AVAR_Scene_BeginBattle,
    AVAR_ScreenShakingScriptID,
};

// Actor Stats
constexpr s32 hp = 12;
constexpr s32 dmgKickWeak = 2;
constexpr s32 dmgKick = 2;
constexpr s32 dmgStompWeak = 1;
constexpr s32 dmgStomp = 1;
constexpr s32 dmgGroundStomp = 4;

// enum ActorParams {
//     DMG_SPIN        = 1,
//     DMG_STOMP       = 2,
// };

s32 DefaultAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_CaptainKuribo_Idle,
    STATUS_KEY_STONE,     ANIM_CaptainKuribo_Still,
    STATUS_KEY_SLEEP,     ANIM_CaptainKuribo_Sleep,
    STATUS_KEY_POISON,    ANIM_CaptainKuribo_Still,
    STATUS_KEY_STOP,      ANIM_CaptainKuribo_Still,
    STATUS_KEY_STATIC,    ANIM_CaptainKuribo_Still,
    STATUS_KEY_PARALYZE,  ANIM_CaptainKuribo_Still,
    STATUS_KEY_DIZZY,     ANIM_CaptainKuribo_Dizzy,
    STATUS_END,
};

// while shuffling around during idle
s32 ShuffleAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_CaptainKuribo_Run,
    STATUS_KEY_STONE,     ANIM_CaptainKuribo_Still,
    STATUS_KEY_SLEEP,     ANIM_CaptainKuribo_Sleep,
    STATUS_KEY_POISON,    ANIM_CaptainKuribo_Still,
    STATUS_KEY_STOP,      ANIM_CaptainKuribo_Still,
    STATUS_KEY_STATIC,    ANIM_CaptainKuribo_Still,
    STATUS_KEY_PARALYZE,  ANIM_CaptainKuribo_Still,
    STATUS_KEY_DIZZY,     ANIM_CaptainKuribo_Dizzy,
    STATUS_END,
};

s32 DangerAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_CaptainKuribo_Dizzy,
    STATUS_KEY_STONE,     ANIM_CaptainKuribo_Still,
    STATUS_KEY_SLEEP,     ANIM_CaptainKuribo_Sleep,
    STATUS_KEY_POISON,    ANIM_CaptainKuribo_Still,
    STATUS_KEY_STOP,      ANIM_CaptainKuribo_Still,
    STATUS_KEY_STATIC,    ANIM_CaptainKuribo_Still,
    STATUS_KEY_PARALYZE,  ANIM_CaptainKuribo_Still,
    STATUS_KEY_DIZZY,     ANIM_CaptainKuribo_Dizzy,
    STATUS_END,
};

s32 FuryAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_CaptainKuribo_Angry,
    STATUS_KEY_STONE,     ANIM_CaptainKuribo_Still,
    STATUS_KEY_SLEEP,     ANIM_CaptainKuribo_Sleep,
    STATUS_KEY_POISON,    ANIM_CaptainKuribo_Still,
    STATUS_KEY_STOP,      ANIM_CaptainKuribo_Still,
    STATUS_KEY_STATIC,    ANIM_CaptainKuribo_Still,
    STATUS_KEY_PARALYZE,  ANIM_CaptainKuribo_Still,
    STATUS_KEY_DIZZY,     ANIM_CaptainKuribo_Dizzy,
    STATUS_END,
};

s32 DefenseTable[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_FIRE,     -1,
    ELEMENT_END,
};

s32 StatusTable[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,             100,
    STATUS_KEY_POISON,              0,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,             100,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,              0,
    STATUS_KEY_PARALYZE,          100,
    STATUS_KEY_SHRINK,            100,
    STATUS_KEY_STOP,              100,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,         -1,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,         -1,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,        -1,
    STATUS_TURN_MOD_PARALYZE,      -1,
    STATUS_TURN_MOD_SHRINK,        -1,
    STATUS_TURN_MOD_STOP,          -1,
    STATUS_END,
};

ActorPartBlueprint ActorParts[] = {
    {
        .flags = ACTOR_PART_FLAG_IGNORE_BELOW_CHECK | ACTOR_PART_FLAG_NO_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { -25, 15 },
        .opacity = 255,
        .idleAnimations = DefaultAnims,
        .defenseTable = DefenseTable,
        .eventFlags = 0,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
    },
    {
        .flags = ACTOR_PART_FLAG_IGNORE_BELOW_CHECK | ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_TARGET,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 80 },
        .opacity = 255,
        .idleAnimations = NULL,
        .defenseTable = DefenseTable,
        .eventFlags = 0,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
    },
};

// Unused
API_CALLABLE(SpawnImpactDustFX) {
    Bytecode* args = script->ptrReadPos;
    s32 x = evt_get_variable(script, *args++);
    s32 y = evt_get_variable(script, *args++);
    s32 z = evt_get_variable(script, *args++);

    fx_landing_dust(2, x, y, z, 0);
    return ApiStatus_DONE2;
}

EvtScript EVS_Init = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(EVS_TakeTurn))
    Call(BindIdle, ACTOR_SELF, Ref(EVS_Idle))
    Call(BindHandleEvent, ACTOR_SELF, Ref(EVS_HandleEvent))
    Call(BindHandlePhase, ACTOR_SELF, Ref(EVS_HandlePhase))
    Call(SetActorVar, ACTOR_SELF, AVAR_TurnCount, 1)
    Call(SetActorVar, ACTOR_SELF, AVAR_RedDead, false)
    Call(SetActorVar, ACTOR_SELF, AVAR_IncreaseShaking, false)
    // Call(SetActorVar, ACTOR_SELF, AVAR_Scene_BeginBattle, false)
    ExecGetTID(EVS_ShakeScreenWhileWalking, LVar0)
    Call(SetActorVar, ACTOR_SELF, AVAR_ScreenShakingScriptID, LVar0)
    Return
    End
};

EvtScript EVS_Idle = {
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(FuryAnims))

    Label(0)
    Call(RandInt, 80, LVar0)
    Add(LVar0, 80)
    Wait(LVar0)
    Label(1)
        Call(GetStatusFlags, ACTOR_SELF, LVar0)
        IfFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
            Wait(1)
            Goto(1)
        EndIf
    Call(SetGoalToHome, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 5)
    Call(SetActorIdleSpeed, ACTOR_SELF, Float(1.0))
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(ShuffleAnims))
    Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(IdleRunToGoal, ACTOR_SELF, 0)
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(DefaultAnims))
    Wait(20)
    Label(2)
        Call(GetStatusFlags, ACTOR_SELF, LVar0)
        IfFlag(LVar0, STATUS_FLAGS_IMMOBILIZED)
            Wait(1)
            Goto(2)
        EndIf
    Call(SetGoalToHome, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetActorIdleSpeed, ACTOR_SELF, Float(1.0))
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(ShuffleAnims))
    Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(IdleRunToGoal, ACTOR_SELF, 0)
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(DefaultAnims))
    Wait(80)
    Goto(0)
    Return
    End
};

EvtScript EVS_FuryIdle = {
    Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(FuryAnims))
    Return
    End
};

EvtScript EVS_ReturnHome = {
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Idle)
    Call(SetGoalToHome, ACTOR_SELF)
    Call(SetActorSpeed, ACTOR_SELF, Float(3.0))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Run)
    Call(RunToGoal, ACTOR_SELF, 0, false)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Idle)
    Return
    End
};

EvtScript EVS_ShockKnockback = {
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(SetAnimation, ACTOR_SELF, LVar0, LVar1)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Set(LVar1, 0)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.4))
    Call(AddGoalPos, ACTOR_SELF, 30, 0, 0)
    Call(JumpToGoal, ACTOR_SELF, 15, false, true, false)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_HEAVY_NPC_STEP_B)
    Call(AddGoalPos, ACTOR_SELF, 15, 0, 0)
    Call(JumpToGoal, ACTOR_SELF, 8, false, true, false)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_HEAVY_NPC_STEP_B)
    Call(AddGoalPos, ACTOR_SELF, 5, 0, 0)
    Call(JumpToGoal, ACTOR_SELF, 5, false, true, false)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_HEAVY_NPC_STEP_B)
    Return
    End
};

EvtScript EVS_HandleEvent = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseEq(EVENT_HIT_COMBO)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Dead)
            ExecWait(EVS_Enemy_Hit)
        CaseEq(EVENT_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Dead)
            ExecWait(EVS_Enemy_Hit)
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_BurnHurt)
            SetConst(LVar2, ANIM_CaptainKuribo_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Dead)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Dead)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Dead)
            ExecWait(EVS_ShockKnockback)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Run)
            ExecWait(EVS_Enemy_ReturnHome)
        CaseEq(EVENT_SHOCK_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Dead)
            ExecWait(EVS_Enemy_ShockHit)
            Call(GetActorVar, ACTOR_SELF, AVAR_ScreenShakingScriptID, LVar0)
            KillThread(LVar0)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Dead)
            ExecWait(EVS_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Idle)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Dead)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            Call(GetActorVar, ACTOR_SELF, AVAR_ScreenShakingScriptID, LVar0)
            KillThread(LVar0)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Dead)
            ExecWait(EVS_Death)
            Return
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_BurnHurt)
            SetConst(LVar2, ANIM_CaptainKuribo_BurnStill)
            ExecWait(EVS_Enemy_BurnHit)
            Wait(10)
            Call(GetActorVar, ACTOR_SELF, AVAR_ScreenShakingScriptID, LVar0)
            KillThread(LVar0)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_BurnStill)
            ExecWait(EVS_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Dead)
            ExecWait(EVS_Enemy_SpinSmashHit)
            Call(GetActorVar, ACTOR_SELF, AVAR_ScreenShakingScriptID, LVar0)
            KillThread(LVar0)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Dead)
            ExecWait(EVS_Death)
            Return
        CaseEq(EVENT_SPIKE_CONTACT)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Dead)
            Wait(20)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Run)
            Call(SetGoalToHome, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
            Call(RunToGoal, ACTOR_SELF, 0, false)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Idle)
        CaseEq(EVENT_END_FIRST_STRIKE)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Idle)
            ExecWait(EVS_Enemy_ReturnHome)
            Call(HPBarToHome, ACTOR_SELF)
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_CaptainKuribo_Idle)
            ExecWait(EVS_Enemy_Recover)
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
    Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
    Call(GetActorVar, ACTOR_SELF, AVAR_RedDead, LVar3)
    IfEq(LVar3, true)
        Call(SetActorVar, ACTOR_SELF, AVAR_RedDead, 2)
        Call(UseBattleCamPreset, BTL_CAM_ACTOR)
        Call(BattleCamTargetActor, ACTOR_SELF)
        Call(MoveBattleCamOver, 30)
        Call(BindIdle, ACTOR_SELF, Ref(EVS_FuryIdle))
        Wait(30)
        ExecWait(EVS_Attack_SpinKick)
        Return
    EndIf
    ExecWait(EVS_Attack_SpinKick)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Return
    End
};

EvtScript EVS_Attack_SpinKick = {
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(GetStatusFlags, ACTOR_SELF, LVar0)
    IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
        Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
        Call(AddBattleCamDist, -100)
        Call(SetBattleCamOffsetY, 20)
        Call(BattleCamTargetActor, ACTOR_SELF)
        Call(MoveBattleCamOver, 30)
        Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
    Else
        Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
        Call(AddBattleCamDist, -100)
        Call(BattleCamTargetActor, ACTOR_SELF)
        Call(MoveBattleCamOver, 20)
        Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
    EndIf
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(GetStatusFlags, ACTOR_SELF, LVar0)
    IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
        Call(SetGoalToTarget, ACTOR_SELF)
        Call(AddGoalPos, ACTOR_SELF, 32, 0, 0)
    Else
        Call(SetGoalToTarget, ACTOR_SELF)
        Call(AddGoalPos, ACTOR_SELF, 12, 0, 0)
    EndIf
    Call(SetActorSpeed, ACTOR_SELF, Float(2.0))
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Run)
    Call(RunToGoal, ACTOR_SELF, 0, false)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Idle)
    Wait(8)
    Thread
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_GOOMBA_KING_SPIN)
        Wait(10)
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_GOOMBA_KING_SPIN)
    EndThread
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Kick)
    Set(LVar0, 0)
    Loop(20)
        Add(LVar0, 18)
        Call(SetActorYaw, ACTOR_SELF, LVar0)
        Wait(1)
    EndLoop
    Call(SetActorYaw, ACTOR_SELF, 0)
    Call(EnemyTestTarget, ACTOR_SELF, LVarA, 0, 0, 2, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVarA)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Dizzy)
            Set(LVar0, 0)
            Loop(20)
                Add(LVar0, 18)
                Call(SetActorYaw, ACTOR_SELF, LVar0)
                Wait(1)
            EndLoop
            Call(SetActorYaw, ACTOR_SELF, 0)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Call(MoveBattleCamOver, 20)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Idle)
            Call(SetGoalToHome, ACTOR_SELF)
            Call(SetActorSpeed, ACTOR_SELF, Float(4.0))
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Run)
            Call(RunToGoal, ACTOR_SELF, 0, false)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Idle)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
    EndSwitch
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 20)
    Wait(2)
    Call(SetGoalToTarget, ACTOR_SELF)
    // Call(ActorExists, ACTOR_ENEMY1, LVar0)
    // IfEq(LVar0, false)
        Call(EnemyDamageTarget, ACTOR_SELF, LVarA, 0, 0, 0, dmgKick, BS_FLAGS1_TRIGGER_EVENTS)
    // Else
    //     Call(GetActorVar, ACTOR_ENEMY1, 3, LVar6)
    //     IfEq(LVar6, false)
    //         Call(EnemyDamageTarget, ACTOR_SELF, LVarA, 0, 0, 0, dmgKick, BS_FLAGS1_TRIGGER_EVENTS)
    //     Else
    //         Call(EnemyDamageTarget, ACTOR_SELF, LVarA, 0, 0, 0, dmgKickWeak, BS_FLAGS1_TRIGGER_EVENTS)
    //     EndIf
    // EndIf
    Call(GetBattleFlags, LVarA)
    IfNotFlag(LVarA, BS_FLAGS1_ATK_BLOCKED)
        Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
        Call(SetActorYaw, ACTOR_SELF, 0)
        Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Idle)
        ExecWait(EVS_Attack_Stomp)
    Else
        Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
        Call(SetActorYaw, ACTOR_SELF, 0)
        Call(SetActorDispOffset, ACTOR_SELF, 0, 0, 0)
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar0, 30)
        Set(LVar1, 0)
        Add(LVar2, 30)
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
        Thread
            Wait(3)
            Set(LVar0, 90)
            Loop(3)
                Sub(LVar0, 30)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, LVar0)
                Wait(1)
            EndLoop
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Dead)
        EndThread
        Call(JumpToGoal, ACTOR_SELF, 20, false, true, false)
        Call(SetOwnerTarget, ACTOR_SELF, PRT_MAIN)
        Call(SetGoalToTarget, ACTOR_SELF)
        Call(EnemyDamageTarget, ACTOR_SELF, LVarA, 0, 0, 0, dmgStomp, BS_FLAGS1_TRIGGER_EVENTS)
        Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar0, 30)
        Set(LVar1, 0)
        Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(JumpToGoal, ACTOR_SELF, 20, false, true, false)
        Call(SetOwnerTarget, ACTOR_SELF, PRT_MAIN)
        Call(SetGoalToTarget, ACTOR_SELF)
        Call(EnemyDamageTarget, ACTOR_SELF, LVarA, 0, 0, 0, dmgStomp, BS_FLAGS1_TRIGGER_EVENTS)
        Thread
            Wait(3)
            Set(LVar0, 90)
            Loop(3)
                Add(LVar0, 30)
                Call(SetActorRotation, ACTOR_SELF, 0, 0, LVar0)
                Wait(1)
            EndLoop
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Dead)
        EndThread
        Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
        ExecWait(EVS_ReturnHome)
    EndIf
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_Stomp = {
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(UseBattleCamPreset, BTL_CAM_ENEMY_APPROACH)
    Call(SetBattleCamDist, 180)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
    Call(SetGoalToTarget, ACTOR_SELF)
    Loop(2)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Angry)
        Call(EnemyTestTarget, ACTOR_SELF, LVarA, 0, 0, 2, BS_FLAGS1_INCLUDE_POWER_UPS)
        Switch(LVarA)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Sub(LVar0, 20)
            Set(LVar1, 0)
            Add(LVar2, 5)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 20, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_HEAVY_NPC_STEP_B)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Still)
            Thread
                Call(StartRumble, BTL_RUMBLE_PLAYER_MAX)
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 10, Float(4.0))
                EndIf
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Idle)
            EndThread
            Call(ShakeCam, CAM_BATTLE, 0, 5, Float(3.0))
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            PlayEffect(EFFECT_SMOKE_IMPACT, 0, LVar0, LVar1, LVar2, 60, 8, 10, 20, 0)
            Wait(2)
            Call(SetGoalToTarget, ACTOR_SELF)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Angry)
            Wait(15)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
        CaseDefault
            Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 20, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_HEAVY_NPC_STEP_B)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Still)
            Thread
                Call(StartRumble, BTL_RUMBLE_PLAYER_MAX)
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 10, Float(4.0))
                EndIf
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Idle)
            EndThread
        EndSwitch
        Wait(2)
        Call(SetGoalToTarget, ACTOR_SELF)
        // Call(ActorExists, ACTOR_ENEMY1, LVar0)
        // IfEq(LVar0, false)
            Call(EnemyDamageTarget, ACTOR_SELF, LVarA, 0, 0, 0, dmgStomp, BS_FLAGS1_TRIGGER_EVENTS)
        // Else
        //     Call(GetActorVar, ACTOR_ENEMY1, 3, LVar6)
        //     IfEq(LVar6, false)
        //         Call(EnemyDamageTarget, ACTOR_SELF, LVarA, 0, 0, 0, dmgStomp, BS_FLAGS1_TRIGGER_EVENTS)
        //     Else
        //         Call(EnemyDamageTarget, ACTOR_SELF, LVarA, 0, 0, 0, dmgStompWeak, BS_FLAGS1_TRIGGER_EVENTS)
        //     EndIf
        // EndIf
    EndLoop
    Switch(LVar0)
        CaseDefault
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar0, 20)
            Set(LVar1, 0)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
            Call(JumpToGoal, ACTOR_SELF, 20, false, true, false)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_HEAVY_NPC_STEP_B)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Still)
            ExecWait(EVS_ReturnHome)
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

// EvtScript EVS_Attack_GroundStomp = {
//     Call(UseBattleCamPreset, BTL_CAM_ACTOR_FAR)
//     Call(BattleCamTargetActor, ACTOR_SELF)
//     Call(MoveBattleCamOver, 40)
//     Wait(15)
//     Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_NONE, BTL_CAM_XADJ_AVG, false)
//     Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Angry)
//     Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
//     Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
//     Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
//     Call(JumpToGoal, ACTOR_SELF, 20, false, true, false)
//     Call(PlaySoundAtActor, ACTOR_SELF, SOUND_HEAVY_NPC_STEP_B)
//     Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
//     Call(MoveBattleCamOver, 30)
//     Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Still)
//     Thread
//         Call(StartRumble, BTL_RUMBLE_PLAYER_MAX)
//         Call(GetStatusFlags, ACTOR_SELF, LVar0)
//         IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
//             Call(ShakeCam, CAM_BATTLE, 0, 10, Float(4.0))
//         EndIf
//         Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_CaptainKuribo_Idle)
//         PlayEffect(EFFECT_DROP_LEAVES, 0, -80, 125, 0, 100, 0)
//         PlayEffect(EFFECT_DROP_LEAVES, 0, -15, 135, 0, 85, 0)
//         PlayEffect(EFFECT_DROP_LEAVES, 0, 100, 103, 0, 70, 0)
//     EndThread
//     Call(EnemyTestTarget, ACTOR_SELF, LVarA, 0, 0, 2, BS_FLAGS1_INCLUDE_POWER_UPS)
//     Switch(LVarA)
//         CaseOrEq(HIT_RESULT_MISS)
//         CaseOrEq(HIT_RESULT_LUCKY)
//             Thread
//                 Wait(12)
//                 Call(PlaySoundAtPart, ACTOR_TREE, PRT_TREE_NUT_1, SOUND_FALL_QUICK)
//             EndThread
//             Thread
//                 Wait(10)
//                 Call(SetPartJumpGravity, ACTOR_TREE, PRT_TREE_NUT_1, Float(1.5))
//                 Call(GetPartOffset, ACTOR_TREE, PRT_TREE_NUT_1, LVar0, LVar1, LVar2)
//                 Call(FallPartTo, ACTOR_TREE, PRT_TREE_NUT_1, LVar0, 10, LVar2, 14)
//                 Sub(LVar0, 12)
//                 Call(JumpPartTo, ACTOR_TREE, PRT_TREE_NUT_1, LVar0, 10, LVar2, 10, true)
//                 Sub(LVar0, 7)
//                 Call(JumpPartTo, ACTOR_TREE, PRT_TREE_NUT_1, LVar0, 10, LVar2, 7, true)
//                 Loop(20)
//                     Call(SetPartFlagBits, ACTOR_TREE, PRT_TREE_NUT_1, ACTOR_PART_FLAG_INVISIBLE, false)
//                     Wait(1)
//                     Call(SetPartFlagBits, ACTOR_TREE, PRT_TREE_NUT_1, ACTOR_PART_FLAG_INVISIBLE, true)
//                     Wait(1)
//                 EndLoop
//             EndThread
//             Thread
//                 Wait(5)
//                 Call(PlaySoundAtPart, ACTOR_TREE, PRT_TREE_NUT_2, SOUND_FALL_QUICK)
//             EndThread
//             Thread
//                 Wait(3)
//                 Call(SetPartJumpGravity, ACTOR_TREE, PRT_TREE_NUT_2, Float(1.5))
//                 Call(GetPartOffset, ACTOR_TREE, PRT_TREE_NUT_2, LVar0, LVar1, LVar2)
//                 Call(FallPartTo, ACTOR_TREE, PRT_TREE_NUT_2, LVar0, 10, LVar2, 15)
//                 Sub(LVar0, 12)
//                 Call(JumpPartTo, ACTOR_TREE, PRT_TREE_NUT_2, LVar0, 10, LVar2, 10, true)
//                 Sub(LVar0, 7)
//                 Call(JumpPartTo, ACTOR_TREE, PRT_TREE_NUT_2, LVar0, 10, LVar2, 7, true)
//                 Loop(20)
//                     Call(SetPartFlagBits, ACTOR_TREE, PRT_TREE_NUT_2, ACTOR_PART_FLAG_INVISIBLE, false)
//                     Wait(1)
//                     Call(SetPartFlagBits, ACTOR_TREE, PRT_TREE_NUT_2, ACTOR_PART_FLAG_INVISIBLE, true)
//                     Wait(1)
//                 EndLoop
//             EndThread
//             Thread
//                 Wait(17)
//                 Call(PlaySoundAtPart, ACTOR_TREE, PRT_TREE_NUT_3, SOUND_FALL_QUICK)
//             EndThread
//             Thread
//                 Wait(15)
//                 Call(SetPartJumpGravity, ACTOR_TREE, PRT_TREE_NUT_3, Float(1.5))
//                 Call(GetPartOffset, ACTOR_TREE, PRT_TREE_NUT_3, LVar0, LVar1, LVar2)
//                 Call(FallPartTo, ACTOR_TREE, PRT_TREE_NUT_3, LVar0, 10, LVar2, 17)
//                 Add(LVar0, 12)
//                 Call(JumpPartTo, ACTOR_TREE, PRT_TREE_NUT_3, LVar0, 10, LVar2, 10, true)
//                 Add(LVar0, 7)
//                 Call(JumpPartTo, ACTOR_TREE, PRT_TREE_NUT_3, LVar0, 10, LVar2, 7, true)
//                 Loop(20)
//                     Call(SetPartFlagBits, ACTOR_TREE, PRT_TREE_NUT_3, ACTOR_PART_FLAG_INVISIBLE, false)
//                     Wait(1)
//                     Call(SetPartFlagBits, ACTOR_TREE, PRT_TREE_NUT_3, ACTOR_PART_FLAG_INVISIBLE, true)
//                     Wait(1)
//                 EndLoop
//             EndThread
//             Wait(15)
//             IfEq(LVarA, HIT_RESULT_LUCKY)
//                 Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
//             EndIf
//             Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
//             Call(UseIdleAnimation, ACTOR_SELF, true)
//             Return
//         EndCaseGroup
//     EndSwitch
//     Thread
//         Wait(12)
//         Call(PlaySoundAtPart, ACTOR_TREE, PRT_TREE_NUT_1, SOUND_FALL_QUICK)
//     EndThread
//     Thread
//         Wait(10)
//         Call(SetPartJumpGravity, ACTOR_TREE, PRT_TREE_NUT_1, Float(1.5))
//         Call(GetPartOffset, ACTOR_TREE, PRT_TREE_NUT_1, LVar0, LVar1, LVar2)
//         Call(FallPartTo, ACTOR_TREE, PRT_TREE_NUT_1, LVar0, 35, LVar2, 14)
//         Sub(LVar0, 12)
//         Call(JumpPartTo, ACTOR_TREE, PRT_TREE_NUT_1, LVar0, 10, LVar2, 10, true)
//         Sub(LVar0, 7)
//         Call(JumpPartTo, ACTOR_TREE, PRT_TREE_NUT_1, LVar0, 10, LVar2, 7, true)
//         Loop(20)
//             Call(SetPartFlagBits, ACTOR_TREE, PRT_TREE_NUT_1, ACTOR_PART_FLAG_INVISIBLE, false)
//             Wait(1)
//             Call(SetPartFlagBits, ACTOR_TREE, PRT_TREE_NUT_1, ACTOR_PART_FLAG_INVISIBLE, true)
//             Wait(1)
//         EndLoop
//     EndThread
//     Thread
//         Wait(5)
//         Call(PlaySoundAtPart, ACTOR_TREE, PRT_TREE_NUT_2, SOUND_FALL_QUICK)
//     EndThread
//     Thread
//         Wait(3)
//         Call(SetPartJumpGravity, ACTOR_TREE, PRT_TREE_NUT_2, Float(1.5))
//         Call(GetPartOffset, ACTOR_TREE, PRT_TREE_NUT_2, LVar0, LVar1, LVar2)
//         Call(FallPartTo, ACTOR_TREE, PRT_TREE_NUT_2, LVar0, 10, LVar2, 15)
//         Sub(LVar0, 12)
//         Call(JumpPartTo, ACTOR_TREE, PRT_TREE_NUT_2, LVar0, 10, LVar2, 10, true)
//         Sub(LVar0, 7)
//         Call(JumpPartTo, ACTOR_TREE, PRT_TREE_NUT_2, LVar0, 10, LVar2, 7, true)
//         Loop(20)
//             Call(SetPartFlagBits, ACTOR_TREE, PRT_TREE_NUT_2, ACTOR_PART_FLAG_INVISIBLE, false)
//             Wait(1)
//             Call(SetPartFlagBits, ACTOR_TREE, PRT_TREE_NUT_2, ACTOR_PART_FLAG_INVISIBLE, true)
//             Wait(1)
//         EndLoop
//     EndThread
//     Thread
//         Wait(17)
//         Call(PlaySoundAtPart, ACTOR_TREE, PRT_TREE_NUT_3, SOUND_FALL_QUICK)
//     EndThread
//     Thread
//         Wait(15)
//         Call(SetPartJumpGravity, ACTOR_TREE, PRT_TREE_NUT_3, Float(1.5))
//         Call(GetPartOffset, ACTOR_TREE, PRT_TREE_NUT_3, LVar0, LVar1, LVar2)
//         Call(FallPartTo, ACTOR_TREE, PRT_TREE_NUT_3, LVar0, 10, LVar2, 17)
//         Add(LVar0, 12)
//         Call(JumpPartTo, ACTOR_TREE, PRT_TREE_NUT_3, LVar0, 10, LVar2, 10, true)
//         Add(LVar0, 7)
//         Call(JumpPartTo, ACTOR_TREE, PRT_TREE_NUT_3, LVar0, 10, LVar2, 7, true)
//         Loop(20)
//             Call(SetPartFlagBits, ACTOR_TREE, PRT_TREE_NUT_3, ACTOR_PART_FLAG_INVISIBLE, false)
//             Wait(1)
//             Call(SetPartFlagBits, ACTOR_TREE, PRT_TREE_NUT_3, ACTOR_PART_FLAG_INVISIBLE, true)
//             Wait(1)
//         EndLoop
//     EndThread
//     Wait(24)
//     Wait(2)
//     Call(SetGoalToTarget, ACTOR_SELF)
//     Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, SUPPRESS_EVENT_ALL, 0, dmgGroundStomp, BS_FLAGS1_TRIGGER_EVENTS)
//     Switch(LVar0)
//         CaseEq(HIT_RESULT_10)
//             Return
//     EndSwitch
//     Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
//     Call(UseIdleAnimation, ACTOR_SELF, true)
//     Return
//     End
// };

EvtScript EVS_HandlePhase = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    // Call(GetBattlePhase, LVar0)
    // Switch(LVar0)
    //     CaseEq(PHASE_PLAYER_BEGIN)
    //         Call(GetActorVar, ACTOR_SELF, AVAR_RedDead, LVar3)
    //         IfEq(LVar3, true)
    //             Call(SetActorVar, ACTOR_SELF, AVAR_RedDead, 2)
    //             Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(FuryAnims))
    //             ExecWait(EVS_Attack_SpinKick)
    //         EndIf
    // EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_ShakeScreenWhileWalking = {
    Label(0)
        Wait(1)
        Call(GetAnimation, ACTOR_SELF, PRT_MAIN, LVar0)
        IfNe(LVar0, ANIM_CaptainKuribo_Run)
            Goto(0)
        EndIf
        Call(GetAnimation, ACTOR_SELF, PRT_MAIN, LVar0)
        IfNe(LVar0, ANIM_CaptainKuribo_Run)
            Goto(0)
        EndIf
        Call(StartRumble, BTL_RUMBLE_LONG)
        Call(GetPartAnimNotify, ACTOR_SELF, PRT_MAIN, LVar0)
        IfEq(LVar0, 1)
            Call(GetActorVar, ACTOR_SELF, AVAR_IncreaseShaking, LVar0)
            IfEq(LVar0, false)
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 2, Float(0.2))
                EndIf
            Else
                Call(GetStatusFlags, ACTOR_SELF, LVar0)
                IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                    Call(ShakeCam, CAM_BATTLE, 0, 2, Float(0.7))
                EndIf
            EndIf
        EndIf
        Goto(0)
    Return
    End
};

EvtScript EVS_Death = {
    Call(HideHealthBar, ACTOR_SELF)
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(UseBattleCamPreset, BTL_CAM_ACTOR)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(MoveBattleCamOver, 30)
    Wait(30)
    Set(LVar2, 0)
    Call(SetAnimation, ACTOR_SELF, LVar0, LVar1)
    Wait(10)
    Loop(24)
        Call(SetActorYaw, ACTOR_SELF, LVar2)
        Add(LVar2, 30)
        Wait(1)
    EndLoop
    Call(SetActorYaw, ACTOR_SELF, 0)
    Call(GetActorPos, ACTOR_SELF, LVar2, LVar3, LVar4)
    PlayEffect(EFFECT_BIG_SMOKE_PUFF, LVar2, LVar3, LVar4, 0, 0, 0, 0, 0)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_ACTOR_DEATH)
    Call(DropStarPoints, ACTOR_SELF)
    Set(LVar3, 0)
    Loop(12)
        Call(SetActorRotation, ACTOR_SELF, LVar3, 0, 0)
        Add(LVar3, 8)
        Wait(1)
    EndLoop
    Call(SetPartFlagBits, ACTOR_SELF, PRT_MAIN, ACTOR_PART_FLAG_INVISIBLE, true)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, true)
    Wait(30)
    Call(RemoveActor, ACTOR_SELF)
    Return
    End
};

}; // namespace captain_kuribo

ActorBlueprint CaptainKuribo = {
    .flags = 0,
    .maxHP = captain_kuribo::hp,
    .type = ACTOR_TYPE_CAPTAIN_KURIBO,
    .level = ACTOR_LEVEL_CAPTAIN_KURIBO,
    .partCount = ARRAY_COUNT(captain_kuribo::ActorParts),
    .partsData = captain_kuribo::ActorParts,
    .initScript = &captain_kuribo::EVS_Init,
    .statusTable = captain_kuribo::StatusTable,
    .escapeChance = 0,
    .airLiftChance = 0,
    .hurricaneChance = 0,
    .spookChance = 0,
    .upAndAwayChance = 0,
    .spinSmashReq = 4,
    .powerBounceChance = 80,
    .coinReward = 0,
    .size = { 56, 80 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -6, 38 },
    .statusTextOffset = { 12, 75 },
};

}; // namespace battle::actor
