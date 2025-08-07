#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/Panser.h"
#include "sff_actors.hpp"

namespace battle::actor {

namespace purple_panser {

extern EvtScript EVS_Init;
extern s32 DefaultAnims[];
extern s32 BitAnims[];
extern EvtScript EVS_Idle;
extern EvtScript EVS_TakeTurn;
extern EvtScript EVS_HandleEvent;
extern EvtScript EVS_ReturnHome;
extern EvtScript EVS_Attack_Bit;

enum ActorPartIDs {
    PRT_MAIN        = 1,
    PRT_BIT         = 2,
};

enum ActorVars {
    AVAR_Bit_ParticlesEffect    = 0,
};

// Actor Stats
constexpr s32 hp = 4;
constexpr s32 dmgBit = 4;

s32 DefaultAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_Panser_Purple_SmugIdle,
    STATUS_KEY_STONE,     ANIM_Panser_Purple_SmugStill,
    STATUS_KEY_SLEEP,     ANIM_Panser_Purple_SmugIdle,
    STATUS_KEY_POISON,    ANIM_Panser_Purple_SmugIdle,
    STATUS_KEY_STOP,      ANIM_Panser_Purple_SmugStill,
    STATUS_KEY_STATIC,    ANIM_Panser_Purple_SmugIdle,
    STATUS_KEY_PARALYZE,  ANIM_Panser_Purple_SmugStill,
    STATUS_KEY_DIZZY,     ANIM_Panser_Purple_SmugIdle,
    STATUS_KEY_FEAR,      ANIM_Panser_Purple_SmugIdle,
    STATUS_END,
};

s32 BitAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_Panser_Purple_DiamondBit,
    STATUS_END,
};

s32 DefenseTable[] = {
    ELEMENT_NORMAL,   0,
    ELEMENT_FIRE,    -1,
    ELEMENT_ICE,     99,
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
        .index = PRT_BIT,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = BitAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAG_ICY,
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

API_CALLABLE(UpdateCrystalBitEffect) {
    Bytecode* args = script->ptrReadPos;
    Actor* actor = get_actor(script->owner1.actorID);
    ActorPart* actorPart = get_actor_part(actor, 2);
    EffectInstance* effect = (EffectInstance*) evt_get_variable(script, *args);

    effect->data.miscParticles->scaleX = actorPart->scale.x * 24.0f;
    effect->data.miscParticles->scaleY = actorPart->scale.y * 24.0f;
    if (actorPart->flags & ACTOR_PART_FLAG_INVISIBLE) {
        effect->data.miscParticles->pos.x = actorPart->curPos.x;
        effect->data.miscParticles->pos.y = NPC_DISPOSE_POS_Y;
        effect->data.miscParticles->pos.z = actorPart->curPos.z;
    } else {
        effect->data.miscParticles->pos.x = actorPart->curPos.x;
        effect->data.miscParticles->pos.y = actorPart->curPos.y;
        effect->data.miscParticles->pos.z = actorPart->curPos.z;
    }

    return ApiStatus_BLOCK;
}

EvtScript EVS_Idle = {
    PlayEffect(EFFECT_MISC_PARTICLES, 1, NPC_DISPOSE_LOCATION, 24, 24, Float(1.0), 5, 0, 0, 0, 0)
    Call(SetActorVar, ACTOR_SELF, AVAR_Bit_ParticlesEffect, LVarF)
    ChildThread
        Call(UpdateCrystalBitEffect, LVarF)
    EndChildThread
    Loop(0)
        Wait(1)
    EndLoop
    Return
    End
};

EvtScript EVS_ReturnHome = {
    Call(ResetAllActorSounds, ACTOR_SELF)
    SetConst(LVar0, PRT_MAIN)
    SetConst(LVar1, ANIM_Panser_Purple_SmugIdle)
    ExecWait(EVS_Enemy_ReturnHome)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Panser_Purple_SmugIdle)
    Return
    End
};

EvtScript EVS_HandleEvent = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    // Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetLastEvent, ACTOR_SELF, LVar0)
    Switch(LVar0)
        CaseOrEq(EVENT_HIT_COMBO)
        CaseOrEq(EVENT_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt)
            ExecWait(EVS_Enemy_Hit)
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt) //BurnHurt
            SetConst(LVar2, ANIM_Panser_Purple_Hurt) //BurnStill
            ExecWait(EVS_Enemy_BurnHit)
        CaseEq(EVENT_BURN_DEATH)
            // Call(GetActorVar, ACTOR_SELF, AVAR_Bit_ParticlesEffect, LVar0)
            // Call(RemoveEffect, LVar0)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt) //BurnHurt
            SetConst(LVar2, ANIM_Panser_Purple_Hurt) //BurnStill
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt) //BurnStill
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SPIN_SMASH_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
        CaseEq(EVENT_SPIN_SMASH_DEATH)
            // Call(GetActorVar, ACTOR_SELF, AVAR_Bit_ParticlesEffect, LVar0)
            // Call(RemoveEffect, LVar0)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt)
            ExecWait(EVS_Enemy_SpinSmashHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt) //Death
            ExecWait(EVS_Enemy_Death)
            Return
        CaseEq(EVENT_SHOCK_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt)
            ExecWait(EVS_Enemy_Knockback)
            ExecWait(EVS_ReturnHome)
        CaseEq(EVENT_SHOCK_DEATH)
            // Call(GetActorVar, ACTOR_SELF, AVAR_Bit_ParticlesEffect, LVar0)
            // Call(RemoveEffect, LVar0)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt)
            ExecWait(EVS_Enemy_ShockHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt)//Death
            ExecWait(EVS_Enemy_Death)
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_SmugIdle)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            // Call(GetActorVar, ACTOR_SELF, AVAR_Bit_ParticlesEffect, LVar0)
            // Call(RemoveEffect, LVar0)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt)
            ExecWait(EVS_Enemy_Hit)
            Wait(10)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_Hurt)//Death
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
            SetConst(LVar1, ANIM_Panser_Purple_SmugIdle)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_FLYING, false)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_SmugIdle)//Move?
            SetConst(LVar2, ANIM_Panser_Purple_Hurt)
            ExecWait(EVS_Enemy_ScareAway)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_Panser_Purple_SmugIdle)
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
            SetConst(LVar1, ANIM_Panser_Purple_Hurt)
            ExecWait(EVS_Enemy_BlowAway)
        CaseDefault
    EndSwitch
    // Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_TakeTurn = {
    // Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(UseIdleAnimation, ACTOR_SELF, false)
    ExecWait(EVS_Attack_Bit)
    // Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript EVS_Attack_Bit = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    // Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_Panser_Purple_Shot)
    Thread
        Wait(10)
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_ACTOR_HURT)
        Wait(14)
        Call(PlaySoundAtActor, ACTOR_SELF, SOUND_ACTOR_HURT)
    EndThread
    Wait(40)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_POWER_UP)
    Wait(18)
    Call(SetAnimation, ACTOR_SELF, PRT_BIT, ANIM_Panser_Purple_DiamondBit)
    Wait(15)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar1, 10)
    Call(SetPartPos, ACTOR_SELF, PRT_BIT, LVar0, LVar1, LVar2)
    Call(SetPartJumpGravity, ACTOR_SELF, PRT_BIT, Float(3.0))
    // Call(SetPartScale, ACTOR_SELF, PRT_BIT, Float(1.5), Float(1.5), Float(1.0))
    Wait(45)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_BIT, ACTOR_PART_FLAG_INVISIBLE, false)
    Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            // Set(LVarA, LVar0)
            Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
            Add(LVar1, -32) // target mario's feet
            Call(PlaySoundAtActor, ACTOR_SELF, SOUND_MOLE_THROW)
            Call(JumpPartTo, ACTOR_SELF, PRT_BIT, LVar0, LVar1, LVar2, 15, true)
            // Call(GetActorVar, ACTOR_SELF, AVAR_Bit_ParticlesEffect, LVar0)
            // Call(RemoveEffect, LVar0)
            Call(SetPartFlagBits, ACTOR_SELF, PRT_BIT, ACTOR_PART_FLAG_INVISIBLE, true)
            Wait(2)
            IfEq(LVarA, HIT_RESULT_LUCKY)
                Call(SetGoalToTarget, ACTOR_SELF)
                Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
        EndCaseGroup
    EndSwitch
    Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
    Call(PlaySoundAtActor, ACTOR_SELF, SOUND_MOLE_THROW)
    Call(JumpPartTo, ACTOR_SELF, PRT_BIT, LVar0, LVar1, LVar2, 15, true)
    // Call(GetActorVar, ACTOR_SELF, AVAR_Bit_ParticlesEffect, LVar0)
    // Call(RemoveEffect, LVar0)
    Call(SetPartFlagBits, ACTOR_SELF, PRT_BIT, ACTOR_PART_FLAG_INVISIBLE, true)
    Wait(2)
    Call(SetGoalToTarget, ACTOR_SELF)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_NO_CONTACT | DAMAGE_TYPE_ICE, 0, 0, dmgBit, BS_FLAGS1_TRIGGER_EVENTS)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_HIT)
        CaseOrEq(HIT_RESULT_NO_DAMAGE)
            Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
            Wait(20)
            Call(YieldTurn)
            ExecWait(EVS_ReturnHome)
        EndCaseGroup
    EndSwitch
    // Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

}; // namespace purple_panser

ActorBlueprint PurplePanser = {
    .flags = 0,
    .maxHP = purple_panser::hp,
    .type = ACTOR_TYPE_PURPLE_PANSER,
    .level = ACTOR_LEVEL_PURPLE_PANSER,
    .partCount = ARRAY_COUNT(purple_panser::ActorParts),
    .partsData = purple_panser::ActorParts,
    .initScript = &purple_panser::EVS_Init,
    .statusTable = purple_panser::StatusTable,
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
