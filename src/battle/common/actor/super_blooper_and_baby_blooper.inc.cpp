#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "model.h"
#include "sprite/npc/Blooper.h"
#include "sprite/npc/BabyBlooper.h"
#include "battle/action_cmd/stop_leech.h"
#include "sprite/player.h"

namespace battle::actor {

namespace super_blooper {

extern EvtScript EVS_Init;
extern EvtScript EVS_Idle;
extern EvtScript EVS_TakeTurn;
extern EvtScript EVS_HandleEvent;
extern EvtScript EVS_Attack_InkBlast;
extern EvtScript EVS_Attack_SpinDrop;
extern EvtScript EVS_Attack_RageDrop;
extern EvtScript EVS_Move_MakeBabies;
extern EvtScript EVS_Move_Enrage;
extern EvtScript EVS_Enrage;
extern EvtScript EVS_LoseRage;
extern EvtScript EVS_Death;

extern Formation BabyFormation1;
extern Formation BabyFormation2;

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
};

enum ActorVars {
    AVAR_Enraged            = 0,
    AVAR_NextMove           = 1,
    AVAL_Move_Basic         = 0,
    AVAL_Move_MakeBabies    = 1,
    AVAL_Move_Enrage        = 2,
    AVAL_Move_RageDrop      = 3,
};

// Mecha Blooper Actor Stats
constexpr s32 hp = 60;
constexpr s32 dmgSpinDrop = 5;
constexpr s32 dmgInkBlast = 5;
constexpr s32 dmgChargedDrop = 10;

EvtScript EVS_FloatToPos = {
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim00)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0C)
    Call(GetActorPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    IfGt(LVarB, 20)
        Sub(LVarB, 20)
    Else
        Set(LVarB, 0)
    EndIf
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.3))
    Call(SetGoalPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Call(FallToGoal, ACTOR_SELF, 8)
    Call(GetActorPos, ACTOR_SELF, LVar4, LVar5, LVar6)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BLOOPER_MOVE)
    Set(LVar7, LVar4)
    Add(LVar7, LVar4)
    Add(LVar7, LVar0)
    MulF(LVar7, Float(0.33))
    Set(LVar8, LVar5)
    Add(LVar8, LVar5)
    Add(LVar8, LVar1)
    MulF(LVar8, Float(0.33))
    Set(LVar9, LVar6)
    Add(LVar9, LVar6)
    Add(LVar9, LVar2)
    MulF(LVar9, Float(0.33))
    Call(SetActorSpeed, ACTOR_SELF, Float(3.0))
    Call(SetGoalPos, ACTOR_SELF, LVar7, LVar8, LVar9)
    Call(FlyToGoal, ACTOR_SELF, LVar3, -2, EASING_CUBIC_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim00)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0C)
    Call(GetActorPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Sub(LVarB, 20)
    Call(SetGoalPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.3))
    Call(FallToGoal, ACTOR_SELF, 8)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BLOOPER_MOVE)
    Set(LVar7, LVar0)
    Add(LVar7, LVar0)
    Add(LVar7, LVar4)
    MulF(LVar7, Float(0.33))
    Set(LVar8, LVar1)
    Add(LVar8, LVar1)
    Add(LVar8, LVar5)
    MulF(LVar8, Float(0.33))
    Set(LVar9, LVar2)
    Add(LVar9, LVar2)
    Add(LVar9, LVar6)
    MulF(LVar9, Float(0.33))
    Call(SetActorSpeed, ACTOR_SELF, Float(3.0))
    Call(SetGoalPos, ACTOR_SELF, LVar7, LVar8, LVar9)
    Call(FlyToGoal, ACTOR_SELF, LVar3, -2, EASING_CUBIC_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim00)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0C)
    Call(GetActorPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Sub(LVarB, 20)
    Call(SetGoalPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.3))
    Call(FallToGoal, ACTOR_SELF, 8)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BLOOPER_MOVE)
    Call(SetActorSpeed, ACTOR_SELF, Float(3.0))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(FlyToGoal, ACTOR_SELF, LVar3, -2, EASING_CUBIC_OUT)
    Return
    End
};

EvtScript EVS_FloatToHome = {
    Call(SetGoalToHome, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Set(LVar3, 12)
    ExecWait(EVS_FloatToPos)
    Return
    End
};

s32 DefaultAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_Blooper_Anim00,
    STATUS_KEY_STOP,      ANIM_Blooper_Anim00,
    STATUS_KEY_SLEEP,     ANIM_Blooper_Anim07,
    STATUS_KEY_PARALYZE,  ANIM_Blooper_Anim08,
    STATUS_KEY_PARALYZE,  ANIM_Blooper_Anim00,
    STATUS_KEY_DIZZY,     ANIM_Blooper_Anim08,
    STATUS_KEY_SHRINK,    ANIM_Blooper_Anim00,
    STATUS_END,
};

s32 AscendAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_Blooper_Anim0C,
    STATUS_KEY_STOP,      ANIM_Blooper_Anim00,
    STATUS_KEY_SLEEP,     ANIM_Blooper_Anim07,
    STATUS_KEY_PARALYZE,  ANIM_Blooper_Anim08,
    STATUS_KEY_PARALYZE,  ANIM_Blooper_Anim00,
    STATUS_KEY_DIZZY,     ANIM_Blooper_Anim08,
    STATUS_KEY_SHRINK,    ANIM_Blooper_Anim0C,
    STATUS_END,
};

s32 DescendAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_Blooper_Anim00,
    STATUS_KEY_STOP,      ANIM_Blooper_Anim00,
    STATUS_KEY_SLEEP,     ANIM_Blooper_Anim07,
    STATUS_KEY_PARALYZE,  ANIM_Blooper_Anim08,
    STATUS_KEY_PARALYZE,  ANIM_Blooper_Anim00,
    STATUS_KEY_DIZZY,     ANIM_Blooper_Anim08,
    STATUS_KEY_SHRINK,    ANIM_Blooper_Anim00,
    STATUS_END,
};

s32 DefenseTable[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_END,
};

s32 StatusTable[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,              40,
    STATUS_KEY_POISON,              0,
    STATUS_KEY_FROZEN,              0,
    STATUS_KEY_DIZZY,              60,
    STATUS_KEY_FEAR,                0,
    STATUS_KEY_STATIC,              0,
    STATUS_KEY_PARALYZE,           50,
    STATUS_KEY_SHRINK,             70,
    STATUS_KEY_STOP,               60,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,         -1,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,          0,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,      -1,
    STATUS_TURN_MOD_SHRINK,        -1,
    STATUS_TURN_MOD_STOP,          -2,
    STATUS_END,
};

s32 EnragedStatusTable[] = {
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
    STATUS_TURN_MOD_SLEEP,         -1,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,          0,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,      -1,
    STATUS_TURN_MOD_SHRINK,        -1,
    STATUS_TURN_MOD_STOP,          -2,
    STATUS_END,
};

ActorPartBlueprint ActorParts[] = {
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 80 },
        .opacity = 255,
        .idleAnimations = DefaultAnims,
        .defenseTable = DefenseTable,
        .eventFlags = 0,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
    },
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_TARGET,
        .posOffset = { 0, 20, 0 },
        .targetOffset = { 0, 60 },
        .opacity = 255,
        .idleAnimations = NULL,
        .defenseTable = DefenseTable,
        .eventFlags = 0,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -30 },
    },
};

#include "common/StartRumbleWithParams.inc.c"
// #include "common/FadeBackgroundDarken.inc.c"

API_CALLABLE((FadeBackgroundDarken)) {
    if (isInitialCall) {
        mdl_set_all_tint_type(ENV_TINT_SHROUD);
        *gBackgroundTintModePtr = ENV_TINT_SHROUD;
        mdl_set_shroud_tint_params(0, 0, 0, 0);
        script->functionTemp[0] = 20;
    }

    mdl_set_shroud_tint_params(0, 0, 0, ((20 - script->functionTemp[0]) * 10) & 254);
    script->functionTemp[0]--;

    if (script->functionTemp[0] == 0) {
        return ApiStatus_DONE2;
    } else {
        return ApiStatus_BLOCK;
    }
}

// #include "common/FadeBackgroundLighten.inc.c"

API_CALLABLE((FadeBackgroundLighten)) {
    if (isInitialCall) {
        script->functionTemp[0] = 20;
    }

    mdl_set_shroud_tint_params(0, 0, 0, (script->functionTemp[0] * 10) & 254);

    script->functionTemp[0]--;
    if (script->functionTemp[0] == 0) {
        mdl_set_shroud_tint_params(0, 0, 0, 0);
        return ApiStatus_DONE2;
    }
    return ApiStatus_BLOCK;
}

#include "common/SpitInk.inc.c"

EvtScript EVS_Init = {
    Call(ForceHomePos, ACTOR_SELF, 90, 45, -10)
    Call(HPBarToHome, ACTOR_SELF)
    Call(SetActorScale, ACTOR_SELF, Float(2.0), Float(2.0), Float(2.0))
    Call(SetActorVar, ACTOR_SELF, AVAR_Enraged, false)
    Call(SetActorVar, ACTOR_SELF, AVAR_NextMove, AVAL_Move_Basic)
    Call(BindTakeTurn, ACTOR_SELF, Ref(EVS_TakeTurn))
    Call(BindIdle, ACTOR_SELF, Ref(EVS_Idle))
    Call(BindHandleEvent, ACTOR_SELF, Ref(EVS_HandleEvent))
    Return
    End
};

EvtScript EVS_Idle = {
    Label(0)
        Call(GetStatusFlags, ACTOR_SELF, LVarA)
        IfFlag(LVarA, STATUS_FLAG_SLEEP | STATUS_FLAG_PARALYZE | STATUS_FLAG_DIZZY | STATUS_FLAG_STONE | STATUS_FLAG_STOP)
            Wait(1)
            Goto(0)
        EndIf
        Call(SetIdleGoalToHome, ACTOR_SELF)
        Call(GetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, 0)
        Call(GetActorPos, ACTOR_SELF, LVar3, LVar4, LVar5)
        IfGe(LVar4, LVar1)
            Goto(11)
        EndIf
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(AscendAnims))
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim00)
        Wait(9)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0C)
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(1.0))
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(1.0))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleFlyToGoal, ACTOR_SELF, 0, 0, EASING_SIN_OUT)
        Label(11)
        Call(GetStatusFlags, ACTOR_SELF, LVarA)
        IfFlag(LVarA, STATUS_FLAG_SLEEP | STATUS_FLAG_PARALYZE | STATUS_FLAG_DIZZY | STATUS_FLAG_STONE | STATUS_FLAG_STOP)
            Wait(1)
            Goto(0)
        EndIf
        Call(SetIdleGoalToHome, ACTOR_SELF)
        Call(GetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, 10)
        Call(GetActorPos, ACTOR_SELF, LVar3, LVar4, LVar5)
        IfGe(LVar4, LVar1)
            Goto(12)
        EndIf
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(AscendAnims))
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim00)
        Wait(9)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0C)
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(1.0))
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(1.0))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleFlyToGoal, ACTOR_SELF, 0, 0, EASING_SIN_OUT)
        Wait(3)
        Label(12)
        Call(GetStatusFlags, ACTOR_SELF, LVarA)
        IfFlag(LVarA, STATUS_FLAG_SLEEP | STATUS_FLAG_PARALYZE | STATUS_FLAG_DIZZY | STATUS_FLAG_STONE | STATUS_FLAG_STOP)
            Wait(1)
            Goto(0)
        EndIf
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(DescendAnims))
        Call(SetIdleGoalToHome, ACTOR_SELF)
        Call(GetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, -10)
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(0.8))
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(0.3))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleFlyToGoal, ACTOR_SELF, 0, 1, EASING_SIN_OUT)
        Wait(1)
        Goto(0)
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
            SetConst(LVar1, ANIM_Blooper_Anim04)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim04)
            ExecWait(EVS_Enemy_Hit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim04)
            ExecWait(EVS_Death)
            Return
        CaseEq(EVENT_BURN_HIT)
            Set(LVar0, PRT_MAIN)
            Set(LVar1, ANIM_Blooper_Anim05)
            Set(LVar2, ANIM_Blooper_Anim06)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            Set(LVar0, PRT_MAIN)
            Set(LVar1, ANIM_Blooper_Anim05)
            Set(LVar2, ANIM_Blooper_Anim06)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim06)
            ExecWait(EVS_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim04)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim04)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim04)
            ExecWait(EVS_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
        CaseOrEq(EVENT_INVUNERABLE_TAUNT)
        CaseOrEq(EVENT_1E)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim00)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseOrEq(EVENT_STAR_BEAM)
        CaseOrEq(EVENT_PEACH_BEAM)
            ExecWait(EVS_LoseRage)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim04)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_SHOCK_HIT)
            ExecWait(EVS_LoseRage)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim04)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim04)
            ExecWait(EVS_Enemy_Knockback)
            ExecWait(EVS_FloatToHome)
        CaseEq(EVENT_SHOCK_DEATH)
            ExecWait(EVS_LoseRage)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim04)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim04)
            ExecWait(EVS_Enemy_Knockback)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim04)
            ExecWait(EVS_Death)
            Return
        CaseEq(EVENT_END_FIRST_STRIKE)
            ExecWait(EVS_FloatToHome)
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Blooper_Anim00)
            ExecWait(EVS_Enemy_Recover)
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Death = {
    ExecWait(EVS_Enemy_Death)
    Return
    End
};

EvtScript EVS_TakeTurn = {
    Call(GetActorVar, ACTOR_SELF, AVAR_NextMove, LVar0)
    Switch(LVar0)
        CaseEq(AVAL_Move_RageDrop)
            Call(GetActorVar, ACTOR_SELF, AVAR_Enraged, LVar0)
            IfEq(LVar0, 0)
                Call(SetActorVar, ACTOR_SELF, AVAR_NextMove, AVAL_Move_Basic)
            EndIf
        CaseEq(AVAL_Move_MakeBabies)
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfFlag(LVar0, STATUS_FLAG_SHRINK)
                Call(SetActorVar, ACTOR_SELF, AVAR_NextMove, AVAL_Move_Enrage)
            Else
                Call(ActorExists, ACTOR_ENEMY1, LVar0)
                IfNe(LVar0, 0)
                    Call(ActorExists, ACTOR_ENEMY2, LVar0)
                    IfNe(LVar0, 0)
                        Call(SetActorVar, ACTOR_SELF, AVAR_NextMove, AVAL_Move_Enrage)
                    EndIf
                EndIf
            EndIf
        CaseDefault
    EndSwitch
    Call(GetActorVar, ACTOR_SELF, AVAR_NextMove, LVar0)
    Switch(LVar0)
        CaseEq(AVAL_Move_MakeBabies)
            Call(SetActorVar, ACTOR_SELF, AVAR_NextMove, AVAL_Move_Enrage)
            ExecWait(EVS_Move_MakeBabies)
            Return
        CaseEq(AVAL_Move_Enrage)
            Call(SetActorVar, ACTOR_SELF, AVAR_NextMove, AVAL_Move_RageDrop)
            ExecWait(EVS_Move_Enrage)
            Return
        CaseEq(AVAL_Move_RageDrop)
            Call(SetActorVar, ACTOR_SELF, AVAR_NextMove, AVAL_Move_Basic)
            ExecWait(EVS_Attack_RageDrop)
            Return
        CaseDefault
            Call(GetStatusFlags, ACTOR_SELF, LVar0)
            IfNotFlag(LVar0, STATUS_FLAG_SHRINK)
                Call(SetActorVar, ACTOR_SELF, AVAR_NextMove, AVAL_Move_MakeBabies)
                Call(RandInt, 100, LVar0)
                IfLt(LVar0, 50)
                    ExecWait(EVS_Attack_SpinDrop)
                Else
                    ExecWait(EVS_Attack_InkBlast)
                EndIf
            Else
                ExecWait(EVS_Attack_SpinDrop)
            EndIf
    EndSwitch
    Return
    End
};

EvtScript EVS_Attack_SpinDrop = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Set(LVar1, 100)
    Set(LVar3, 15)
    ExecWait(EVS_FloatToPos)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BLOOPER_FALL)
    Call(EnemyTestTarget, ACTOR_SELF, LVarF, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVarF)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Thread
                Set(LVar0, 0)
                Loop(36)
                    Add(LVar0, 30)
                    Call(SetActorYaw, ACTOR_SELF, LVar0)
                    Wait(1)
                EndLoop
                Call(SetActorYaw, ACTOR_SELF, 0)
            EndThread
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0D)
            Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.5))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 16, false, true, false)
            IfEq(LVarF, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Add(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 12, false, true, false)
            Add(LVar0, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 8, false, true, false)
            ExecWait(EVS_FloatToHome)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
    EndSwitch
    Thread
        Set(LVar0, 0)
        Loop(16)
            Add(LVar0, 30)
            Call(SetActorYaw, ACTOR_SELF, LVar0)
            Wait(1)
        EndLoop
        Call(SetActorYaw, ACTOR_SELF, 0)
    EndThread
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0D)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.5))
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Add(LVar1, 30)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 16, false, true, false)
    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, 0, 0, 0, dmgSpinDrop, BS_FLAGS1_TRIGGER_EVENTS)
    Call(GetStatusFlags, ACTOR_PLAYER, LVar0)
    IfFlag(LVar0, STATUS_FLAG_STONE)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim04)
    Else
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim00)
    EndIf
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 30)
    Sub(LVar1, 15)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.2))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 10, false, true, false)
    ExecWait(EVS_FloatToHome)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_InkBlast = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 85)
    Add(LVar1, 45)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim00)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0C)
    Call(GetActorPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    IfGt(LVarB, 30)
        Sub(LVarB, 30)
    Else
        Set(LVarB, 0)
    EndIf
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.3))
    Call(SetGoalPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Call(FallToGoal, ACTOR_SELF, 8)
    Call(GetActorPos, ACTOR_SELF, LVar4, LVar5, LVar6)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BLOOPER_MOVE)
    Set(LVar7, LVar4)
    Add(LVar7, LVar0)
    MulF(LVar7, Float(0.5))
    Set(LVar8, LVar5)
    Add(LVar8, LVar1)
    MulF(LVar8, Float(0.5))
    Set(LVar9, LVar6)
    Add(LVar9, LVar2)
    MulF(LVar9, Float(0.5))
    Call(SetActorSpeed, ACTOR_SELF, Float(3.0))
    Call(SetGoalPos, ACTOR_SELF, LVar7, LVar8, LVar9)
    Call(FlyToGoal, ACTOR_SELF, 15, -2, EASING_CUBIC_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim00)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0C)
    Call(GetActorPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Sub(LVarB, 20)
    Call(SetGoalPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.3))
    Call(FallToGoal, ACTOR_SELF, 8)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BLOOPER_MOVE)
    Call(SetActorSpeed, ACTOR_SELF, Float(3.0))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(FlyToGoal, ACTOR_SELF, 15, -2, EASING_CUBIC_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim03)
    Call(GetActorPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Sub(LVarB, 30)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.2))
    Call(SetActorSpeed, ACTOR_SELF, Float(1.0))
    Call(SetGoalPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Call(FlyToGoal, ACTOR_SELF, 16, 0, EASING_LINEAR)
    Call(SetActorRotationOffset, ACTOR_SELF, 0, 40, 0)
    Call(SetActorRotation, ACTOR_SELF, 0, 0, -40)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BLOOPER_SQUIRT)
    Call(GetActorPos, ACTOR_SELF, LVar3, LVar4, LVar5)
    Add(LVar3, 1)
    Sub(LVar4, 30)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Call(N(SpitInk), LVar0, LVar1, LVar2, LVar3, LVar4, LVar5)
    Call(EnemyTestTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_NO_CONTACT, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVarF)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Wait(10)
            IfEq(LVarF, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Wait(30)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim00)
            Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
            Call(SetActorRotationOffset, ACTOR_SELF, 0, 0, 0)
            ExecWait(EVS_FloatToHome)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
    EndSwitch
    Wait(10)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(SetDamageSource, DMG_SRC_INK_BLAST)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_NO_CONTACT, 0, 0, dmgInkBlast, BS_FLAGS1_TRIGGER_EVENTS)
    Wait(30)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim00)
    Call(SetActorRotation, ACTOR_SELF, 0, 0, 0)
    Call(SetActorRotationOffset, ACTOR_SELF, 0, 0, 0)
    ExecWait(EVS_FloatToHome)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Move_MakeBabies = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(UseBattleCamPreset, BTL_CAM_ACTOR_GOAL_SIMPLE)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamOffsetY, 50)
    Call(SetBattleCamDist, 330)
    Call(MoveBattleCamOver, 40)
    Wait(20)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BIG_POWER_UP)
    Call((FadeBackgroundDarken))
    Call(N(StartRumbleWithParams), 50, 20)
    Thread
        Call(ShakeCam, CAM_BATTLE, 0, 10, Float(0.3))
    EndThread
    Call(UseBattleCamPreset, BTL_CAM_ACTOR_GOAL_SIMPLE)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamOffsetY, 65)
    Call(SetBattleCamDist, 240)
    Call(MoveBattleCamOver, 15)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim09)
    Wait(15)
    Call(N(StartRumbleWithParams), 100, 20)
    Thread
        Call(ShakeCam, CAM_BATTLE, 0, 10, Float(0.4))
    EndThread
    Call(UseBattleCamPreset, BTL_CAM_ACTOR_GOAL_SIMPLE)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamOffsetY, 80)
    Call(SetBattleCamDist, 150)
    Call(MoveBattleCamOver, 15)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0A)
    Wait(15)
    Call(N(StartRumbleWithParams), 150, 20)
    Thread
        Call(ShakeCam, CAM_BATTLE, 0, 10, Float(0.5))
    EndThread
    Call(UseBattleCamPreset, BTL_CAM_ACTOR_GOAL_SIMPLE)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamOffsetY, 95)
    Call(SetBattleCamDist, 60)
    Call(MoveBattleCamOver, 15)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0B)
    Wait(30)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 20)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim03)
    Wait(10)
    Call(ActorExists, ACTOR_ENEMY1, LVar1)
    IfFalse(LVar1)
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_LIGHT_THROW)
        Call(SummonEnemy, Ref(BabyFormation1), false)
    EndIf
    Call(ActorExists, ACTOR_ENEMY2, LVar1)
    IfFalse(LVar1)
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_LIGHT_THROW)
        Call(SummonEnemy, Ref(BabyFormation2), false)
    EndIf
    Wait(2)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0C)
    Call((FadeBackgroundLighten))
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    Return
    End
};

EvtScript EVS_Move_Enrage = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(UseBattleCamPreset, BTL_CAM_ACTOR_GOAL_SIMPLE)
    Call(BattleCamTargetActor, ACTOR_SELF)
    Call(SetBattleCamOffsetY, 28)
    Call(SetBattleCamDist, 297)
    Call(MoveBattleCamOver, 50)
    Wait(20)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BIG_POWER_UP)
    Call((FadeBackgroundDarken))
    Call(N(StartRumbleWithParams), 70, 80)
    Thread
        Call(ShakeCam, CAM_BATTLE, 0, 40, Float(0.3))
    EndThread
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim09)
    Wait(10)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0A)
    Wait(10)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0B)
    Wait(10)
    ExecWait(EVS_Enrage)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 30)
    Call((FadeBackgroundLighten))
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    Return
    End
};

EvtScript EVS_Attack_RageDrop = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Set(LVar1, 100)
    Set(LVar3, 15)
    ExecWait(EVS_FloatToPos)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BLOOPER_FALL)
    Call(EnemyTestTarget, ACTOR_SELF, LVarF, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVarF)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Thread
                Set(LVar0, 0)
                Loop(36)
                    Add(LVar0, 30)
                    Call(SetActorYaw, ACTOR_SELF, LVar0)
                    Wait(1)
                EndLoop
                Call(SetActorYaw, ACTOR_SELF, 0)
            EndThread
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0D)
            Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.5))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 16, false, true, false)
            ExecWait(EVS_LoseRage)
            IfEq(LVarF, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Add(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 12, false, true, false)
            Add(LVar0, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 8, false, true, false)
            ExecWait(EVS_FloatToHome)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
    EndSwitch
    Thread
        Set(LVar0, 0)
        Loop(16)
            Add(LVar0, 30)
            Call(SetActorYaw, ACTOR_SELF, LVar0)
            Wait(1)
        EndLoop
        Call(SetActorYaw, ACTOR_SELF, 0)
    EndThread
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim0D)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.5))
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Add(LVar1, 30)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 16, false, true, false)
    Call(GetStatusFlags, ACTOR_PLAYER, LVarE)
    IfFlag(LVarE, STATUS_FLAG_STONE)
        Goto(0)
    EndIf
    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, 0, 0, 0, dmgChargedDrop, BS_FLAGS1_NICE_HIT)
    Call(GetPlayerHP, LVarE)
    IfEq(LVarE, 0)
        Goto(1)
    EndIf
    Thread
        Set(LVar0, 0)
        Loop(20)
            Add(LVar0, 36)
            Call(SetActorYaw, ACTOR_SELF, LVar0)
            Wait(1)
        EndLoop
        Call(SetActorYaw, ACTOR_SELF, 0)
    EndThread
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.4))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 20, false, true, false)
    Label(0)
    Wait(2)
    Call(EnemyDamageTarget, ACTOR_SELF, LVarF, 0, 0, 0, dmgChargedDrop, BS_FLAGS1_TRIGGER_EVENTS)
    Label(1)
    Call(GetStatusFlags, ACTOR_PLAYER, LVar0)
    IfFlag(LVar0, STATUS_FLAG_STONE)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim04)
    Else
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Blooper_Anim00)
    EndIf
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 30)
    Sub(LVar1, 15)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.2))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 10, false, true, false)
    ExecWait(EVS_LoseRage)
    ExecWait(EVS_FloatToHome)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Enrage = {
    Call(SetActorVar, ACTOR_SELF, AVAR_Enraged, true)
    Call(EnableActorPaletteEffects, ACTOR_SELF, PRT_MAIN, true)
    Call(SetActorPaletteSwapParams, ACTOR_SELF, PRT_MAIN, SPR_PAL_Blooper, SPR_PAL_Blooper_Supercharged, 0, 6, 12, 6, 0, 0)
    Call(SetActorPaletteEffect, ACTOR_SELF, PRT_MAIN, ACTOR_PAL_ADJUST_BLEND_PALETTES_VARYING_INTERVALS)
    Call(SetStatusTable, ACTOR_SELF, Ref(EnragedStatusTable))
    Call(SetPartEventBits, ACTOR_SELF, PRT_TARGET, ACTOR_EVENT_FLAG_ATTACK_CHARGED, true)
    Return
    End
};

EvtScript EVS_LoseRage = {
    Call(SetActorVar, ACTOR_SELF, AVAR_Enraged, false)
    Call(EnableActorPaletteEffects, ACTOR_SELF, PRT_MAIN, false)
    Call(SetStatusTable, ACTOR_SELF, Ref(StatusTable))
    Call(SetPartEventBits, ACTOR_SELF, PRT_TARGET, ACTOR_EVENT_FLAG_ATTACK_CHARGED, false)
    Return
    End
};

Vec3i SummonPos = { 400, 0, 0 };

Formation BabyFormation1 = {
    ACTOR_BY_POS(battle::actor::BabyBlooper, SummonPos, 95, .var0 = 30, .var1 = 68),
};

Formation BabyFormation2 = {
    ACTOR_BY_POS(battle::actor::BabyBlooper, SummonPos, 94, .var0 = 134, .var1 = 45),
};

}; // namespace super_blooper

namespace baby_blooper {

extern EvtScript EVS_Init;
extern EvtScript EVS_Idle;
extern EvtScript EVS_TakeTurn;
extern EvtScript EVS_HandleEvent;
extern EvtScript EVS_Death;

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_TARGET      = 2,
};

enum ActorVars {
    AVAR_HomePosX   = 0,
    AVAR_HomePosY   = 1,
    AVAR_HomePosZ   = 2,
};

// Baby Blooper Actor Stats
constexpr s32 hp = 6;
constexpr s32 dmgLeech = 2;

EvtScript EVS_FloatToPos = {
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim00)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim01)
    Call(GetActorPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    IfGt(LVarB, 20)
        Sub(LVarB, 20)
    Else
        Set(LVarB, 0)
    EndIf
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.3))
    Call(SetGoalPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Call(FallToGoal, ACTOR_SELF, 8)
    Call(GetActorPos, ACTOR_SELF, LVar4, LVar5, LVar6)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BABY_BLOOPER_MOVE)
    Set(LVar7, LVar4)
    Add(LVar7, LVar4)
    Add(LVar7, LVar0)
    MulF(LVar7, Float(0.33))
    Set(LVar8, LVar5)
    Add(LVar8, LVar5)
    Add(LVar8, LVar1)
    MulF(LVar8, Float(0.33))
    Set(LVar9, LVar6)
    Add(LVar9, LVar6)
    Add(LVar9, LVar2)
    MulF(LVar9, Float(0.33))
    Call(SetActorSpeed, ACTOR_SELF, Float(3.0))
    Call(SetGoalPos, ACTOR_SELF, LVar7, LVar8, LVar9)
    Call(FlyToGoal, ACTOR_SELF, LVar3, -2, EASING_CUBIC_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim00)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim01)
    Call(GetActorPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Sub(LVarB, 20)
    Call(SetGoalPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.3))
    Call(FallToGoal, ACTOR_SELF, 8)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BABY_BLOOPER_MOVE)
    Set(LVar7, LVar0)
    Add(LVar7, LVar0)
    Add(LVar7, LVar4)
    MulF(LVar7, Float(0.33))
    Set(LVar8, LVar1)
    Add(LVar8, LVar1)
    Add(LVar8, LVar5)
    MulF(LVar8, Float(0.33))
    Set(LVar9, LVar2)
    Add(LVar9, LVar2)
    Add(LVar9, LVar6)
    MulF(LVar9, Float(0.33))
    Call(SetActorSpeed, ACTOR_SELF, Float(3.0))
    Call(SetGoalPos, ACTOR_SELF, LVar7, LVar8, LVar9)
    Call(FlyToGoal, ACTOR_SELF, LVar3, -2, EASING_CUBIC_OUT)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim00)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim01)
    Call(GetActorPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Sub(LVarB, 20)
    Call(SetGoalPos, ACTOR_SELF, LVarA, LVarB, LVarC)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.3))
    Call(FallToGoal, ACTOR_SELF, 8)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BABY_BLOOPER_MOVE)
    Call(SetActorSpeed, ACTOR_SELF, Float(3.0))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(FlyToGoal, ACTOR_SELF, LVar3, -2, EASING_CUBIC_OUT)
    Return
    End
};

EvtScript EVS_FloatToHome = {
    Call(SetGoalToHome, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Set(LVar3, 12)
    ExecWait(EVS_FloatToPos)
    Return
    End
};

s32 DefaultAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_BabyBlooper_Anim00,
    STATUS_KEY_STOP,      ANIM_BabyBlooper_Anim00,
    STATUS_KEY_SLEEP,     ANIM_BabyBlooper_Anim06,
    STATUS_KEY_PARALYZE,  ANIM_BabyBlooper_Anim07,
    STATUS_KEY_PARALYZE,  ANIM_BabyBlooper_Anim00,
    STATUS_KEY_DIZZY,     ANIM_BabyBlooper_Anim07,
    STATUS_KEY_SHRINK,    ANIM_BabyBlooper_Anim00,
    STATUS_END,
};

s32 AscendAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_BabyBlooper_Anim01,
    STATUS_KEY_STOP,      ANIM_BabyBlooper_Anim00,
    STATUS_KEY_SLEEP,     ANIM_BabyBlooper_Anim06,
    STATUS_KEY_PARALYZE,  ANIM_BabyBlooper_Anim07,
    STATUS_KEY_PARALYZE,  ANIM_BabyBlooper_Anim00,
    STATUS_KEY_DIZZY,     ANIM_BabyBlooper_Anim07,
    STATUS_KEY_SHRINK,    ANIM_BabyBlooper_Anim01,
    STATUS_END,
};

s32 DescendAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_BabyBlooper_Anim00,
    STATUS_KEY_STOP,      ANIM_BabyBlooper_Anim00,
    STATUS_KEY_SLEEP,     ANIM_BabyBlooper_Anim06,
    STATUS_KEY_PARALYZE,  ANIM_BabyBlooper_Anim07,
    STATUS_KEY_PARALYZE,  ANIM_BabyBlooper_Anim00,
    STATUS_KEY_DIZZY,     ANIM_BabyBlooper_Anim07,
    STATUS_KEY_SHRINK,    ANIM_BabyBlooper_Anim00,
    STATUS_END,
};

s32 DefenseTable[] = {
    ELEMENT_NORMAL,   0,
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
    STATUS_KEY_SHRINK,              0,
    STATUS_KEY_STOP,              100,
    STATUS_TURN_MOD_DEFAULT,        0,
    STATUS_TURN_MOD_SLEEP,         -1,
    STATUS_TURN_MOD_POISON,         0,
    STATUS_TURN_MOD_FROZEN,         0,
    STATUS_TURN_MOD_DIZZY,          0,
    STATUS_TURN_MOD_FEAR,           0,
    STATUS_TURN_MOD_STATIC,         0,
    STATUS_TURN_MOD_PARALYZE,      -1,
    STATUS_TURN_MOD_SHRINK,        -1,
    STATUS_TURN_MOD_STOP,          -2,
    STATUS_END,
};

ActorPartBlueprint ActorParts[] = {
    {
        .flags = ACTOR_PART_FLAG_NO_TARGET,
        .index = PRT_MAIN,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 85 },
        .opacity = 255,
        .idleAnimations = DefaultAnims,
        .defenseTable = DefenseTable,
        .eventFlags = 0,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
    },
    {
        .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
        .index = PRT_TARGET,
        .posOffset = { 0, 27, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = NULL,
        .defenseTable = DefenseTable,
        .eventFlags = 0,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 1, -13 },
    },
};

#include "common/StartRumbleWithParams.inc.c"
#include "common/SpawnEnemyDrainFX.inc.c"

EvtScript EVS_Init = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(EVS_TakeTurn))
    Call(BindHandleEvent, ACTOR_SELF, Ref(EVS_HandleEvent))
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BABY_BLOOPER_MOVE)
    Call(N(StartRumbleWithParams), 80, 20)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim00)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim01)
    Call(GetActorPos, ACTOR_ENEMY0, LVar0, LVar1, LVar2)
    Add(LVar1, 10)
    Sub(LVar2, 10)
    Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Set(LVar1, 20)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.3))
    Call(SetActorSpeed, ACTOR_SELF, Float(0.3))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(FlyToGoal, ACTOR_SELF, 8, 0, EASING_LINEAR)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BABY_BLOOPER_MOVE)
    Call(N(StartRumbleWithParams), 80, 20)
    Call(GetActorVar, ACTOR_SELF, AVAR_HomePosX, LVar0)
    Call(GetActorVar, ACTOR_SELF, AVAR_HomePosY, LVar1)
    Call(GetActorVar, ACTOR_SELF, AVAR_HomePosZ, LVar2)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(SetActorSpeed, ACTOR_SELF, Float(3.0))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(FlyToGoal, ACTOR_SELF, 12, -2, EASING_CUBIC_OUT)
    Call(ForceHomePos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(BindIdle, ACTOR_SELF, Ref(EVS_Idle))
    Call(HPBarToHome, ACTOR_SELF)
    Return
    End
};

EvtScript EVS_Idle = {
    Label(0)
        Call(GetStatusFlags, ACTOR_SELF, LVarA)
        IfFlag(LVarA, STATUS_FLAG_SLEEP | STATUS_FLAG_PARALYZE | STATUS_FLAG_DIZZY | STATUS_FLAG_STONE | STATUS_FLAG_STOP)
            Wait(1)
            Goto(0)
        EndIf
        Call(SetGoalToHome, ACTOR_SELF)
        Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, 0)
        Call(GetActorPos, ACTOR_SELF, LVar3, LVar4, LVar5)
        IfGe(LVar4, LVar1)
            Goto(11)
        EndIf
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(AscendAnims))
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim00)
        Wait(9)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim01)
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(1.0))
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(1.0))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleFlyToGoal, ACTOR_SELF, 0, 0, EASING_SIN_OUT)
        Label(11)
        Call(GetStatusFlags, ACTOR_SELF, LVarA)
        IfFlag(LVarA, STATUS_FLAG_SLEEP | STATUS_FLAG_PARALYZE | STATUS_FLAG_DIZZY | STATUS_FLAG_STONE | STATUS_FLAG_STOP)
            Wait(1)
            Goto(0)
        EndIf
        Call(SetGoalToHome, ACTOR_SELF)
        Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, 5)
        Call(GetActorPos, ACTOR_SELF, LVar3, LVar4, LVar5)
        IfGe(LVar4, LVar1)
            Goto(12)
        EndIf
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(AscendAnims))
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim00)
        Wait(9)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim01)
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(1.0))
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(1.0))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleFlyToGoal, ACTOR_SELF, 0, 0, EASING_SIN_OUT)
        Wait(3)
        Label(12)
        Call(GetStatusFlags, ACTOR_SELF, LVarA)
        IfFlag(LVarA, STATUS_FLAG_SLEEP | STATUS_FLAG_PARALYZE | STATUS_FLAG_DIZZY | STATUS_FLAG_STONE | STATUS_FLAG_STOP)
            Wait(1)
            Goto(0)
        EndIf
        Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(DescendAnims))
        Call(SetGoalToHome, ACTOR_SELF)
        Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
        Add(LVar1, -5)
        Call(SetActorIdleJumpGravity, ACTOR_SELF, Float(0.8))
        Call(SetActorIdleSpeed, ACTOR_SELF, Float(0.3))
        Call(SetIdleGoal, ACTOR_SELF, LVar0, LVar1, LVar2)
        Call(IdleFlyToGoal, ACTOR_SELF, 0, 1, EASING_SIN_OUT)
        Wait(1)
        Goto(0)
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
            SetConst(LVar1, ANIM_BabyBlooper_Anim02)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BabyBlooper_Anim02)
            ExecWait(EVS_Enemy_Hit)
            SetConst(LVar1, ANIM_BabyBlooper_Anim03)
            ExecWait(EVS_Death)
            Return
        CaseEq(EVENT_BURN_HIT)
            Set(LVar0, 1)
            Set(LVar1, ANIM_BabyBlooper_Anim04)
            Set(LVar2, ANIM_BabyBlooper_Anim05)
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            Set(LVar0, 1)
            Set(LVar1, ANIM_BabyBlooper_Anim04)
            Set(LVar2, ANIM_BabyBlooper_Anim05)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar1, ANIM_BabyBlooper_Anim05)
            ExecWait(EVS_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BabyBlooper_Anim02)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BabyBlooper_Anim02)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BabyBlooper_Anim03)
            ExecWait(EVS_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BabyBlooper_Anim00)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_SHOCK_HIT)
            Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BabyBlooper_Anim02)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BabyBlooper_Anim02)
            ExecWait(EVS_Enemy_Knockback)
            ExecWait(EVS_FloatToHome)
        CaseOrEq(EVENT_SHOCK_DEATH)
            Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BabyBlooper_Anim02)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BabyBlooper_Anim02)
            ExecWait(EVS_Enemy_Knockback)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BabyBlooper_Anim03)
            ExecWait(EVS_Death)
            Return
        CaseEq(EVENT_END_FIRST_STRIKE)
            ExecWait(EVS_FloatToHome)
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_BabyBlooper_Anim00)
            ExecWait(EVS_Enemy_Recover)
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Death = {
    SetConst(LVar0, PRT_MAIN)
    ExecWait(EVS_Enemy_Death)
    Return
    End
};

EvtScript EVS_TakeTurn = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Set(LVar1, 100)
    Set(LVar3, 12)
    ExecWait(EVS_FloatToPos)
    Thread
        Wait(2)
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_FALL_QUICK)
    EndThread
    Call(EnemyTestTarget, ACTOR_SELF, LVarF, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVarF)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim02)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Set(LVar1, 0)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 16, false, true, false)
            IfEq(LVarF, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, ACTOR_SELF, LVarF, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Add(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 12, false, true, false)
            Add(LVar0, 20)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 8, false, true, false)
            ExecWait(EVS_FloatToHome)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
        CaseOrEq(HIT_RESULT_HIT_STATIC)
        CaseOrEq(HIT_RESULT_IMMUNE)
            Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim02)
            Call(SetGoalToTarget, ACTOR_SELF)
            Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(SetActorJumpGravity, ACTOR_SELF, Float(1.5))
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 16, false, true, false)
            IfEq(LVarF, 7)
                Call(EnemyDamageTarget, ACTOR_SELF, LVarF, 0, 0, 0, 0, BS_FLAGS1_NICE_HIT)
            EndIf
            Add(LVar0, 30)
            Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Call(JumpToGoal, ACTOR_SELF, 15, false, true, false)
            ExecWait(EVS_FloatToHome)
            Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
            Call(UseIdleAnimation, ACTOR_SELF, true)
            Return
        EndCaseGroup
    EndSwitch
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Add(LVar1, 50)
    Call(UseBattleCamPreset, BTL_CAM_REPOSITION)
    Call(SetBattleCamTarget, LVar0, LVar1, LVar2)
    Call(SetBattleCamDist, 325)
    Call(MoveBattleCamOver, 30)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim02)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(0.8))
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(GetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Sub(LVar1, 10)
    Sub(LVar2, 5)
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 16, false, true, false)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim08)
    Call(ShowMessageBox, BTL_MSG_ACTION_TIP_MASH_BUTTON, AC_LEECH_MAX_TIME)
    Call(ShowActionHud, true)
    Call(LoadActionCommand, ACTION_COMMAND_STOP_LEECH)
    Call(action_command_stop_leech_init)
    Call(SetupMashMeter, 1, 15, 0, 0, 0, 0)
    Wait(10)
    Call(SetBattleFlagBits, BS_FLAGS1_4000, false)
    Call(action_command_stop_leech_start, 0, AC_LEECH_MAX_TIME, AC_DIFFICULTY_3)
    Loop(5)
        Call(UseIdleAnimation, ACTOR_PLAYER, false)
        Call(SetAnimation, ACTOR_PLAYER, 0, ANIM_MarioB1_Leeching)
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
            Call(DispatchEventPlayer, EVENT_66)
            Call(UseIdleAnimation, ACTOR_PLAYER, true)
            BreakLoop
        EndIf
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_LEECH)
        Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
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
            Call(DispatchEventPlayer, EVENT_66)
            Call(UseIdleAnimation, ACTOR_PLAYER, true)
            BreakLoop
        EndIf
        Call(SetActorScale, ACTOR_SELF, Float(0.9), Float(1.1), Float(1.0))
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
            Call(DispatchEventPlayer, EVENT_66)
            Call(UseIdleAnimation, ACTOR_PLAYER, true)
            BreakLoop
        EndIf
        Call(SetActorScale, ACTOR_SELF, Float(0.8), Float(1.2), Float(1.0))
        Set(LFlag0, false)
        Loop(3)
            Call(GetMashActionQuality, LVar1)
            IfEq(LVar1, 1)
                Set(LFlag0, true)
                BreakLoop
            EndIf
            Wait(1)
        EndLoop
        IfEq(LFlag0, true)
            Call(DispatchEventPlayer, EVENT_66)
            Call(UseIdleAnimation, ACTOR_PLAYER, true)
            BreakLoop
        EndIf
        Call(SetActorScale, ACTOR_SELF, Float(0.7), Float(1.4), Float(1.0))
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
            Call(DispatchEventPlayer, EVENT_66)
            Call(UseIdleAnimation, ACTOR_PLAYER, true)
            BreakLoop
        EndIf
        Call(UseIdleAnimation, ACTOR_PLAYER, true)
        Call(SetDamageSource, DMG_SRC_LEECH)
        Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_UNBLOCKABLE | DAMAGE_TYPE_IGNORE_DEFENSE, 0, 0, dmgLeech, BS_FLAGS1_NICE_HIT)
        Call(StartRumble, BTL_RUMBLE_HIT_MIN)
        Call(GetLastDamage, ACTOR_PLAYER, LVar3)
        IfNe(LVar3, 0)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_RECOVER_HEART)
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_HEART_BOUNCE)
            Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
            Add(LVar1, 10)
            Call(N(SpawnDrainHealthStartFX), LVar0, LVar1, LVar2, LVar3)
            Thread
                Wait(15)
                Call(PlaySoundAtActor, ACTOR_SELF, SOUND_STAR_BOUNCE_A)
                Call(N(SpawnDrainHealthContinueFX), LVar0, LVar1, LVar2, LVar3)
            EndThread
            Add(LVar0, 20)
            Add(LVar1, 20)
            PlayEffect(EFFECT_RECOVER, 0, LVar0, LVar1, LVar2, LVar3, 0)
            Call(GetEnemyMaxHP, ACTOR_SELF, LVar1)
            Call(GetActorHP, ACTOR_SELF, LVar0)
            Add(LVar0, LVar3)
            IfGt(LVar0, LVar1)
                Set(LVar0, LVar1)
            EndIf
            Call(SetEnemyHP, ACTOR_SELF, LVar0)
        Else
            Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
            Call(DispatchEventPlayer, EVENT_66)
            Call(InterruptLeechActionCommand)
            BreakLoop
        EndIf
        Call(SetActorScale, ACTOR_SELF, Float(0.7), Float(1.4), Float(1.0))
        Set(LFlag0, false)
        Loop(3)
            Call(GetMashActionQuality, LVar1)
            IfEq(LVar1, 1)
                Set(LFlag0, true)
                BreakLoop
            EndIf
            Wait(1)
        EndLoop
        IfEq(LFlag0, true)
            Call(DispatchEventPlayer, EVENT_66)
            Call(UseIdleAnimation, ACTOR_PLAYER, true)
            BreakLoop
        EndIf
        Call(SetActorScale, ACTOR_SELF, Float(0.8), Float(1.2), Float(1.0))
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
            Call(DispatchEventPlayer, EVENT_66)
            Call(UseIdleAnimation, ACTOR_PLAYER, true)
            BreakLoop
        EndIf
        Call(SetActorScale, ACTOR_SELF, Float(0.9), Float(1.1), Float(1.0))
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
            Call(DispatchEventPlayer, EVENT_66)
            Call(UseIdleAnimation, ACTOR_PLAYER, true)
            BreakLoop
        EndIf
        Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
        Call(GetPlayerHP, LVar1)
        IfEq(LVar1, 0)
            Call(DispatchEventPlayer, EVENT_66)
            Call(InterruptLeechActionCommand)
            BreakLoop
        EndIf
        Set(LFlag0, false)
        Loop(12)
            Call(GetMashActionQuality, LVar1)
            IfEq(LVar1, 1)
                Set(LFlag0, true)
                BreakLoop
            EndIf
            Wait(1)
        EndLoop
        IfEq(LFlag0, true)
            Call(DispatchEventPlayer, EVENT_66)
            BreakLoop
        EndIf
    EndLoop
    Call(DispatchEventPlayer, EVENT_66)
    Call(InterruptLeechActionCommand)
    Call(UseIdleAnimation, ACTOR_PLAYER, true)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 20)
    Call(GetStatusFlags, ACTOR_PLAYER, LVar0)
    IfFlag(LVar0, STATUS_FLAG_STONE)
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim02)
    Else
        Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BabyBlooper_Anim00)
    EndIf
    Call(SetActorScale, ACTOR_SELF, Float(1.0), Float(1.0), Float(1.0))
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar0, 30)
    Sub(LVar1, 15)
    Call(SetActorJumpGravity, ACTOR_SELF, Float(1.2))
    Call(SetGoalPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Call(JumpToGoal, ACTOR_SELF, 10, false, true, false)
    ExecWait(EVS_FloatToHome)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_RESTART)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};


}; // namespace baby_blooper

ActorBlueprint SuperBlooper = {
    .flags = ACTOR_FLAG_FLYING,
    .maxHP = super_blooper::hp,
    .type = ACTOR_TYPE_SUPER_BLOOPER1,
    .level = ACTOR_LEVEL_SUPER_BLOOPER1,
    .partCount = ARRAY_COUNT(super_blooper::ActorParts),
    .partsData = super_blooper::ActorParts,
    .initScript = &super_blooper::EVS_Init,
    .statusTable = super_blooper::StatusTable,
    .escapeChance = 0,
    .airLiftChance = 0,
    .hurricaneChance = 0,
    .spookChance = 0,
    .upAndAwayChance = 0,
    .spinSmashReq = 4,
    .powerBounceChance = 75,
    .coinReward = 0,
    .size = { -111, -88 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { 0, 60 },
    .statusTextOffset = { 40, 80 },
};

ActorBlueprint BabyBlooper = {
    .flags = ACTOR_FLAG_FLYING,
    .maxHP = baby_blooper::hp,
    .type = ACTOR_TYPE_BLOOPER_BABY,
    .level = ACTOR_LEVEL_BLOOPER_BABY,
    .partCount = ARRAY_COUNT(baby_blooper::ActorParts),
    .partsData = baby_blooper::ActorParts,
    .initScript = &baby_blooper::EVS_Init,
    .statusTable = baby_blooper::StatusTable,
    .escapeChance = 0,
    .airLiftChance = 0,
    .hurricaneChance = 0,
    .spookChance = 0,
    .upAndAwayChance = 0,
    .spinSmashReq = 0,
    .powerBounceChance = 95,
    .coinReward = 0,
    .size = { 28, 25 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -12, 20 },
    .statusTextOffset = { 10, 25 },
};

}; // namespace battle::actor
