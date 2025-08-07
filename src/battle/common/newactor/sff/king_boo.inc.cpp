#include "common.h"
#include "effects.h"
#include "battle/battle.h"
#include "script_api/battle.h"
#include "sprite/npc/KingBoo.h"
#include "sff_actors.hpp"
#include "king_boo_common.hpp"
#include "dx/debug_menu.h"

namespace battle::actor {

namespace king_boo {

extern ActorBlueprint A(clone);
extern EvtScript EVS_Init;
extern s32 DefaultAnims[];
extern s32 N(LaughAnims)[];
extern s32 N(FireBallAnims)[];
extern s32 N(BooAnims)[];
extern EvtScript EVS_Idle;
extern EvtScript EVS_TakeTurn;
extern EvtScript EVS_HandleEvent;
extern EvtScript N(EVS_OnHit);
extern EvtScript N(EVS_OnDeath);
extern EvtScript N(EVS_Init_Removal);
extern EvtScript N(EVS_RemoveClone);
extern EvtScript N(EVS_Scare_Phase1Attacks);
extern EvtScript N(EVS_CrownMagic_Phase1Attacks);
extern EvtScript N(EVS_Attack_Scare);
extern EvtScript N(EVS_Attack_FakeoutScare);
extern EvtScript N(EVS_Attack_Fireball);
extern EvtScript N(EVS_Attack_Lightning);
extern EvtScript N(EVS_IllusoryClones);
extern EvtScript N(EVS_BooFlood);
extern EvtScript N(EVS_ClonePosition);
extern EvtScript N(EVS_Attack_FakeoutScareA);
extern EvtScript N(EVS_Attack_FakeoutScareB);
extern EvtScript N(EVS_Attack_BoostedLightning);
extern EvtScript N(EVS_Move_Heal);
extern EvtScript N(EVS_Move_Buff);
// Clone Actor
extern EvtScript N(EVS_Init_Clone);
extern EvtScript N(EVS_Clone_Dummy);
extern EvtScript N(EVS_TakeTurn_Clone);
extern EvtScript N(EVS_HandleEvent_Clone);
extern EvtScript N(EVS_OnCloneDeath);

enum N(ActorPartIDs) {
    PRT_MAIN        = 1,
    PRT_FIREBALL    = 2,
    PRT_BOO         = 3,
};

// enum N(ActorVars) {
//     AVAR_KB_Phase                  = 0,
//     AVAL_KB_CheckPhase             = 0,
//     AVAL_KB_FirstPhase             = 1,
//     AVAL_KB_SecondPhase            = 2,
//     AVAL_KB_ThirdPhase             = 3,
//     AVAR_KB_ClonesDefeated         = 1,
//     AVAL_KB_ClonesAlive            = 0,
//     AVAL_KB_1CloneDead             = 1,
//     AVAL_KB_2ClonesDead            = 2,
//     AVAL_KB_3ClonesDead            = 3,
// };

enum N(ActorParams) {
    DMG_SCARE               = 3,
    DMG_FIREBALL            = 4,
    DMG_LIGHTNING           = 5,
    DMG_FAKEOUT_SCARE_A     = 4,
    DMG_FAKEOUT_SCARE_B     = 2,
    DMG_BOOSTED_LIGHTNING   = 7,
};

s32 DefenseTable[] = {
    ELEMENT_NORMAL,     0,
    ELEMENT_JUMP,       0,
    ELEMENT_SMASH,      0,
    ELEMENT_THROW,     -2,
    ELEMENT_WATER,      3,
    ELEMENT_FIRE,       3,
    ELEMENT_ICE,        3,
    ELEMENT_BLAST,     -4,
    ELEMENT_MAGIC,    100,
    ELEMENT_COSMIC,  -100,
    ELEMENT_END,
};

s32 StatusTable[] = {
    STATUS_KEY_NORMAL,              0,
    STATUS_KEY_DEFAULT,             0,
    STATUS_KEY_SLEEP,             100,
    STATUS_KEY_POISON,            100,
    STATUS_KEY_FROZEN,            100,
    STATUS_KEY_DIZZY,             100,
    STATUS_KEY_FEAR,              100,
    STATUS_KEY_STATIC,            100,
    STATUS_KEY_PARALYZE,          100,
    STATUS_KEY_SHRINK,            100,
    STATUS_KEY_STOP,              100,
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
        .targetOffset = { 0, 43 },
        .opacity = 255,
        .idleAnimations = DefaultAnims,
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, -10 },
    },
    {
        .flags = ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE,
        .index = PRT_FIREBALL,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = N(FireBallAnims),
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
    },
    {
        .flags = ACTOR_PART_FLAG_USE_ABSOLUTE_POSITION | ACTOR_PART_FLAG_NO_TARGET | ACTOR_PART_FLAG_INVISIBLE,
        .index = PRT_BOO,
        .posOffset = { 0, 0, 0 },
        .targetOffset = { 0, 0 },
        .opacity = 255,
        .idleAnimations = N(BooAnims),
        .defenseTable = DefenseTable,
        .eventFlags = ACTOR_EVENT_FLAGS_NONE,
        .elementImmunityFlags = 0,
        .projectileTargetOffset = { 0, 0 },
    },
};

ActorBlueprint NAMESPACE = {
    .flags = ACTOR_FLAG_FLYING,
    .type = ACTOR_TYPE_KING_BOO,
    .level = ACTOR_LEVEL_KING_BOO,
    .maxHP = 60,
    .partCount = ARRAY_COUNT(ActorParts),
    .partsData = ActorParts,
    .initScript = &EVS_Init,
    .statusTable = StatusTable,
    .escapeChance = 100,
    .airLiftChance = 0,
    .hurricaneChance = 0,
    .spookChance = 0,
    .upAndAwayChance = 0,
    .spinSmashReq = 0,
    .powerBounceChance = 100,
    .coinReward = 0,
    .size = { 48, 45 },
    .healthBarOffset = { 0, 0 },
    .statusIconOffset = { -10, 20 },
    .statusTextOffset = { 10, 20 },
};

s32 DefaultAnims[] = {
    STATUS_KEY_NORMAL,    ANIM_KingBoo_Idle,
    STATUS_KEY_STONE,     ANIM_KingBoo_Still,
    STATUS_KEY_SLEEP,     ANIM_KingBoo_Still,
    STATUS_KEY_POISON,    ANIM_KingBoo_Idle,
    STATUS_KEY_STOP,      ANIM_KingBoo_Still,
    STATUS_KEY_STATIC,    ANIM_KingBoo_Idle,
    STATUS_KEY_PARALYZE,  ANIM_KingBoo_Still,
    STATUS_KEY_DIZZY,     ANIM_KingBoo_Idle,
    STATUS_KEY_FEAR,      ANIM_KingBoo_Idle,
    STATUS_END,
};

s32 N(LaughAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_KingBoo_Laugh,
    STATUS_END,
};

s32 N(FireBallAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_KingBoo_FireBall,
    STATUS_END,
};

s32 N(BooAnims)[] = {
    STATUS_KEY_NORMAL,    ANIM_KingBoo_Boo,
    STATUS_END,
};

EvtScript EVS_Init = {
    Call(BindTakeTurn, ACTOR_SELF, Ref(EVS_TakeTurn))
    Call(BindIdle, ACTOR_SELF, Ref(EVS_Idle))
    Call(BindHandleEvent, ACTOR_SELF, Ref(EVS_HandleEvent))
	Call(SetActorVar, ACTOR_SELF, AVAR_KB_Phase, AVAL_KB_FirstPhase)
	Call(SetActorVar, ACTOR_SELF, AVAR_KB_ClonesDefeated, AVAL_KB_1CloneDead)
	// Boo 1
	Call(CreateNpc, 1, ANIM_KingBoo_Boo)
	Call(SetNpcYaw, 1, 0)
	Call(SetNpcPos, 1, 0, -1000, 0)
	// Boo 2
	Call(CreateNpc, 2, ANIM_KingBoo_Boo)
	Call(SetNpcYaw, 2, 0)
	Call(SetNpcPos, 2, 0, -1000, 0)
	// Boo 3
	Call(CreateNpc, 3, ANIM_KingBoo_Boo)
	Call(SetNpcYaw, 3, 0)
	Call(SetNpcPos, 3, 0, -1000, 0)
	// Boo 4
	Call(CreateNpc, 4, ANIM_KingBoo_Boo)
	Call(SetNpcYaw, 4, 0)
	Call(SetNpcPos, 4, 0, -1000, 0)
	// Boo 5
	Call(CreateNpc, 5, ANIM_KingBoo_Boo)
	Call(SetNpcYaw, 5, 0)
	Call(SetNpcPos, 5, 0, -1000, 0)
	// Boo 6
	Call(CreateNpc, 6, ANIM_KingBoo_Boo)
	Call(SetNpcYaw, 6, 0)
	Call(SetNpcPos, 6, 0, -1000, 0)
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
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KingBoo_Hurt)
            ExecWait(EVS_Enemy_Hit)
        CaseOrEq(EVENT_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KingBoo_Hurt)
            ExecWait(EVS_Enemy_Hit)
			ExecWait(N(EVS_OnHit))
        EndCaseGroup
        CaseEq(EVENT_BURN_HIT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KingBoo_Hurt)
            ExecWait(EVS_Enemy_BurnHit)
			ExecWait(N(EVS_OnHit))
        CaseEq(EVENT_BURN_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KingBoo_Hurt)
            SetConst(LVar2, ANIM_KingBoo_Hurt)
            ExecWait(EVS_Enemy_BurnHit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KingBoo_Hurt)
            ExecWait(N(EVS_OnDeath))
            Return
        CaseOrEq(EVENT_ZERO_DAMAGE)
        CaseOrEq(EVENT_IMMUNE)
        CaseOrEq(EVENT_AIR_LIFT_FAILED)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KingBoo_Idle)
            ExecWait(EVS_Enemy_NoDamageHit)
        EndCaseGroup
        CaseEq(EVENT_DEATH)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KingBoo_Hurt)
            ExecWait(EVS_Enemy_Hit)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KingBoo_Hurt)
            ExecWait(N(EVS_OnDeath))
            Return
        CaseEq(EVENT_RECOVER_STATUS)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KingBoo_Idle)
            ExecWait(EVS_Enemy_Recover)
        CaseEq(EVENT_SCARE_AWAY)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KingBoo_Slink)
            SetConst(LVar2, ANIM_KingBoo_Laugh)
            ExecWait(EVS_Enemy_NoDamageHit)
            Return
        CaseEq(EVENT_BEGIN_AIR_LIFT)
            SetConst(LVar0, PRT_MAIN)
            SetConst(LVar1, ANIM_KingBoo_Idle)
            ExecWait(EVS_Enemy_NoDamageHit)
        CaseDefault
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript N(EVS_OnHit) = {
	Call(UseIdleAnimation, ACTOR_SELF, false)
	Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
	// Call(HideHealthBar, ACTOR_SELF)
	// Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_MoveFast)
	// Call(SetActorSpeed, ACTOR_SELF, Float(6.0))
	// Call(SetGoalPos, ACTOR_SELF, 70, 50, 5)
	// Call(FlyToGoal, ACTOR_SELF, 0, 0, EASING_COS_IN_OUT)
	// Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Idle)
	// Wait(5)
	// Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
	// Call(ForceHomePos, ACTOR_SELF, LVar0, LVar1, LVar2)
	// Call(HPBarToHome, ACTOR_SELF)
	Call(UseIdleAnimation, ACTOR_SELF, true)
	Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
	Return
	End
};

EvtScript N(EVS_OnDeath) = {
	Call(UseIdleAnimation, ACTOR_SELF, false)
	Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    // Boo 1 Disappears
    Call(PlaySoundAtNpc, 1, SOUND_BOO_VANISH_A, SOUND_SPACE_DEFAULT)
    SetF(LVar0, 128) // ALPHA
    Call(SetNpcRenderMode, 1, RENDER_MODE_SURFACE_XLU_LAYER1)
	Loop(8)
		Sub(LVar0, 16)
		Call(SetNpcImgFXParams, 1, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
		Wait(1)
	EndLoop
	Call(SetNpcAnimation, 1, ANIM_KingBoo_Boo)
    Call(SetNpcPos, 1, 0, -1000, -125)
    // Boo 2 Disappears
    Call(PlaySoundAtNpc, 2, SOUND_BOO_VANISH_A, SOUND_SPACE_DEFAULT)
    SetF(LVar0, 128) // ALPHA
    Call(SetNpcRenderMode, 2, RENDER_MODE_SURFACE_XLU_LAYER1)
	Loop(8)
		Sub(LVar0, 16)
		Call(SetNpcImgFXParams, 2, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
		Wait(1)
	EndLoop
	Call(SetNpcAnimation, 2, ANIM_KingBoo_Boo)
    Call(SetNpcPos, 2, 0, -1000, -125)
    // Boo 3 Disappears
    Call(PlaySoundAtNpc, 3, SOUND_BOO_VANISH_A, SOUND_SPACE_DEFAULT)
    SetF(LVar0, 128) // ALPHA
    Call(SetNpcRenderMode, 3, RENDER_MODE_SURFACE_XLU_LAYER1)
	Loop(8)
		Sub(LVar0, 16)
		Call(SetNpcImgFXParams, 3, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
		Wait(1)
	EndLoop
	Call(SetNpcAnimation, 3, ANIM_KingBoo_Boo)
    Call(SetNpcPos, 3, 0, -1000, -125)
    // Boo 4 Disappears
    Call(PlaySoundAtNpc, 4, SOUND_BOO_VANISH_A, SOUND_SPACE_DEFAULT)
    SetF(LVar0, 128) // ALPHA
    Call(SetNpcRenderMode, 4, RENDER_MODE_SURFACE_XLU_LAYER1)
	Loop(8)
		Sub(LVar0, 16)
		Call(SetNpcImgFXParams, 4, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
		Wait(1)
	EndLoop
	Call(SetNpcAnimation, 4, ANIM_KingBoo_Boo)
    Call(SetNpcPos, 4, 0, -1000, -125)
    // Boo 5 Disappears
    Call(PlaySoundAtNpc, 5, SOUND_BOO_VANISH_A, SOUND_SPACE_DEFAULT)
    SetF(LVar0, 128) // ALPHA
    Call(SetNpcRenderMode, 5, RENDER_MODE_SURFACE_XLU_LAYER1)
	Loop(8)
		Sub(LVar0, 16)
		Call(SetNpcImgFXParams, 5, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
		Wait(1)
	EndLoop
	Call(SetNpcAnimation, 5, ANIM_KingBoo_Boo)
    Call(SetNpcPos, 5, 0, -1000, -125)
    // Boo 6 Disappears
    Call(PlaySoundAtNpc, 6, SOUND_BOO_VANISH_A, SOUND_SPACE_DEFAULT)
    SetF(LVar0, 128) // ALPHA
    Call(SetNpcRenderMode, 6, RENDER_MODE_SURFACE_XLU_LAYER1)
	Loop(8)
		Sub(LVar0, 16)
		Call(SetNpcImgFXParams, 6, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
		Wait(1)
	EndLoop
	Call(SetNpcAnimation, 6, ANIM_KingBoo_Boo)
    Call(SetNpcPos, 6, 0, -1000, -125)
	// King Boo's "Death"
	Wait(15)
    Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_HEALTH_BAR, true)
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOO_VANISH_A)
	SetF(LVar0, 256)
	Loop(16)
		Sub(LVar0, 16)
		Call(SetPartAlpha, ACTOR_SELF, PRT_MAIN, LVar0)
		Wait(1)
	EndLoop
	Wait(30)
	Call(RemoveActor, ACTOR_SELF)
	Return
	End
};

EvtScript N(EVS_RemoveClone) = {
	Call(UseIdleAnimation, ACTOR_SELF, false)
	Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
	Call(UseIdleAnimation, LVar9, false)
	Call(SetAnimation, LVar9, PRT_MAIN, ANIM_KingBoo_CloneReact)
	Wait(20)
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Idle)
	Call(RemoveActor, LVar9)
	Call(SetIdleAnimations, ACTOR_SELF, PRT_MAIN, Ref(DefaultAnims))
	Call(UseIdleAnimation, ACTOR_SELF, true)
	Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
	Return
	End
};

#define LBL_SKIP_DAMAGE 0

EvtScript EVS_TakeTurn = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetActorVar, ACTOR_SELF, AVAR_KB_Phase, LVar6)
    Switch(LVar6)
        CaseEq(AVAL_KB_FirstPhase)
            Call(GetActorHP, ACTOR_SELF, LVar4)
            IfLt(LVar4, 41)
                ExecWait(N(EVS_IllusoryClones))
                Call(SetActorVar, ACTOR_SELF, AVAR_KB_Phase, AVAL_KB_SecondPhase)
            Else
                Call(RandInt, 100, LVar1)
                Switch(LVar1)
                    CaseLt(70)
                        Set(LVar0, 0)
                    CaseDefault
                        Set(LVar0, 1)
                EndSwitch
                // Set(LVar0, 1) // Set attack. For testing.
                Switch(LVar0)
                    CaseEq(0)
                        ExecWait(N(EVS_Scare_Phase1Attacks))
                    CaseEq(1)
                        ExecWait(N(EVS_CrownMagic_Phase1Attacks))
                EndSwitch
            EndIf
        CaseEq(AVAL_KB_SecondPhase)
            Call(GetActorVar, ACTOR_SELF, AVAR_KB_ClonesDefeated, LVar4)
            IfEq(LVar4, AVAL_KB_AllClonesDefeated)
                DebugPrintf("All Clones Dead %d\n", LVar6)
                Call(SetActorVar, ACTOR_SELF, AVAR_KB_Phase, AVAL_KB_ThirdPhase)
                Call(SetEnemyHP, ACTOR_SELF, 10)
            EndIf
        CaseEq(AVAL_KB_ThirdPhase)
            Call(SetPartFlagBits, ACTOR_SELF, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, false)
            ExecWait(N(EVS_Attack_BoostedLightning))
    EndSwitch
    Call(GetActorVar, ACTOR_SELF, AVAR_KB_Phase, LVar6)
    DebugPrintf("Phase: %d\n", LVar6)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript N(EVS_Scare_Phase1Attacks) = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(RandInt, 100, LVar1)
    Switch(LVar1)
        CaseLt(70)
            Set(LVar0, 0)
        CaseDefault
            Set(LVar0, 1)
    EndSwitch
    //Set(LVar0, 0) // Set attack. For testing.
    Switch(LVar0)
        CaseEq(0)
            ExecWait(N(EVS_Attack_Scare))
        CaseEq(1)
            ExecWait(N(EVS_Attack_FakeoutScare))
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript N(EVS_CrownMagic_Phase1Attacks) = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(RandInt, 100, LVar1)
    Switch(LVar1)
        CaseLt(60)
            Set(LVar0, 0)
        CaseDefault
            Set(LVar0, 1)
    EndSwitch
    // Set(LVar0, 1) // Set attack. For testing.
    Switch(LVar0)
        CaseEq(0)
            Set(LVarA, ACTOR_SELF)
            ExecWait(N(EVS_Attack_Fireball))
        CaseEq(1)
            Set(LVarA, ACTOR_SELF)
            ExecWait(N(EVS_Attack_Lightning))
    EndSwitch
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript N(EVS_Attack_Scare) = {
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
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Hide)
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
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Scare)
	Wait(30)
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
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOW_SPOOK)
	Wait(5)
    Call(SetGoalToTarget, ACTOR_SELF)
	Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_NO_CONTACT, 0, 0, DMG_SCARE, BS_FLAGS1_TRIGGER_EVENTS) // 3 Damage
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
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Idle)
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
};

EvtScript N(EVS_Attack_FakeoutScare) = {
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
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Hide)
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

	Thread
		Wait(15)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Sub(LVarD, 1)
		Call(SetActorPos, ACTOR_SELF,  LVarD, LVarE, LVarF)
		Wait(2)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Add(LVarD, 1)
		Call(SetActorPos, ACTOR_SELF,  LVarD, LVarE, LVarF)
		Wait(2)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Sub(LVarD, 2)
		Call(SetActorPos, ACTOR_SELF,  LVarD, LVarE, LVarF)
		Wait(2)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Add(LVarD, 2)
		Call(SetActorPos, ACTOR_SELF,  LVarD, LVarE, LVarF)
		Wait(2)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Sub(LVarD, 3)
		Call(SetActorPos, ACTOR_SELF,  LVarD, LVarE, LVarF)
		Wait(2)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Add(LVarD, 3)
		Call(SetActorPos, ACTOR_SELF,  LVarD, LVarE, LVarF)
		Wait(2)
	EndThread

	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Fakeout)
	Wait(40)

	// Wiggle a little bit
	Thread
		Wait(15)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Sub(LVarD, 1)
		Call(SetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Wait(2)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Add(LVarD, 1)
		Call(SetActorPos, ACTOR_SELF,  LVarD, LVarE, LVarF)
		Wait(2)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Sub (LVarD, 2)
		Call(SetActorPos, ACTOR_SELF,  LVarD, LVarE, LVarF)
		Wait(2)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Add(LVarD, 2)
		Call(SetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Wait(2)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Sub(LVarD, 3)
		Call(SetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Wait(2)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Add(LVarD, 3)
		Call(SetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Wait(2)
		Call(GetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
		Sub(LVarD, 4)
		Call(SetActorPos, ACTOR_SELF, LVarD, LVarE, LVarF)
	EndThread
	Wait(30)
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
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOW_SPOOK)
	Wait(5)
    Call(SetGoalToTarget, ACTOR_SELF)
	Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_NO_CONTACT, 0, 0, DMG_SCARE, BS_FLAGS1_TRIGGER_EVENTS) // 3 Damage
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
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Idle)
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
};

#include "common/CalculateArcsinDeg.inc.c"

#include "common/UnkBackgroundFunc3.inc.c"

#include "common/SetBackgroundAlpha.inc.c"

EvtScript N(EVS_Attack_Fireball) = {
    Call(UseIdleAnimation, LVarA, false)
    Call(EnableIdleScript, LVarA, IDLE_SCRIPT_DISABLE)
	Call(SetTargetActor, LVarA, ACTOR_PLAYER)
	Call(BattleCamTargetActor, LVarA)
	Call(N(UnkBackgroundFunc3))
    Call(MakeLerp, 0, 200, 30, EASING_COS_IN_OUT)
    Label(0)
        Call(UpdateLerp)
        Call(N(SetBackgroundAlpha), LVar0)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(0)
        EndIf
    Wait(5)
	Call(SetAnimation, LVarA, PRT_MAIN, ANIM_KingBoo_CrownAttack)
	Call(PlaySoundAtActor, LVarA, SOUND_SPELL_CAST4)
	Wait(50)
    Call(SetAnimation, LVarA, PRT_FIREBALL, ANIM_KingBoo_FireBall)
    Wait(15)
    Call(GetActorPos, LVarA, LVar0, LVar1, LVar2)
    Add(LVar1, 30)
	Add(LVar2, 2)
    Call(SetPartPos, LVarA, PRT_FIREBALL, LVar0, LVar1, LVar2)
    Call(SetPartScale, LVarA, PRT_FIREBALL, Float(1.0), Float(1.0), Float(1.0))
	Call(SetPartRotation, LVarA, PRT_FIREBALL, 0, 0, 0)
    Call(SetPartJumpGravity, LVarA, PRT_FIREBALL, Float(2.0))
    Call(SetPartFlagBits, LVarA, PRT_FIREBALL, ACTOR_PART_FLAG_INVISIBLE, false)
	Call(MakeLerp, 200, 0, 1, EASING_LINEAR)
    Label(2)
        Call(UpdateLerp)
        Call(N(SetBackgroundAlpha), LVar0)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(2)
        EndIf
    Call(SetGoalToTarget, LVarA)
    Call(GetGoalPos, LVarA, LVar0, LVar1, LVar2)
    Call(EnemyTestTarget, LVarA, LVar3, DAMAGE_TYPE_NO_CONTACT, 0, 1, BS_FLAGS1_INCLUDE_POWER_UPS)
    Switch(LVar3)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            Add(LVar1, -32) // Target mario's feet
            Call(PlaySoundAtActor, LVarA, SOUND_EMBER_SPIT)
			Thread
				Call(GetPartPos, LVarA, PRT_FIREBALL, LVar1, LVar2, LVar0)
                Set(LVar0,  0)
				Loop(16)
					Call(GetPartPos, LVarA, PRT_FIREBALL, LVar4, LVar5, LVar6)
					Call(N(CalculateArcsinDeg), LVar1, LVar2, LVar4, LVar5, LVar0)
					Call(SetPartRotation, LVarA, PRT_FIREBALL, 0, 0, LVar0)
					Set(LVar1, LVar4)
					Set(LVar2, LVar5)
					Set(LVar3, LVar6)
					Wait(1)
				EndLoop
			EndThread
            Call(JumpPartTo, LVarA, PRT_FIREBALL, LVar0, LVar1, LVar2, 20, true)
            Call(SetPartFlagBits, LVarA, PRT_FIREBALL, ACTOR_PART_FLAG_INVISIBLE, true)
            Call(SetGoalToTarget, LVarA)
            Wait(2)
            IfEq(LVar0, HIT_RESULT_LUCKY)
                Call(EnemyTestTarget, LVarA, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Goto(LBL_SKIP_DAMAGE)
    EndSwitch
    Call(PlaySoundAtActor, LVarA, SOUND_EMBER_SPIT)
    Thread
        Call(GetPartPos, LVarA, PRT_FIREBALL, LVar1, LVar2, LVar0)
        Set(LVar0,  0)
        Loop(16)
            Call(GetPartPos, LVarA, PRT_FIREBALL, LVar4, LVar5, LVar6)
            Call(N(CalculateArcsinDeg), LVar1, LVar2, LVar4, LVar5, LVar0)
            Call(SetPartRotation, LVarA, PRT_FIREBALL, 0, 0, LVar0)
            Set(LVar1, LVar4)
            Set(LVar2, LVar5)
            Set(LVar3, LVar6)
            Wait(1)
        EndLoop
    EndThread
    Call(JumpPartTo, LVarA, PRT_FIREBALL, LVar0, LVar1, LVar2, 20, true)
    Call(SetPartFlagBits, LVarA, PRT_FIREBALL, ACTOR_PART_FLAG_INVISIBLE, true)
    Wait(2)
    Call(SetGoalToTarget, LVarA)
    Call(EnemyDamageTarget, LVarA, LVar0, DAMAGE_TYPE_MAGIC | DAMAGE_TYPE_FIRE | DAMAGE_TYPE_NO_CONTACT, 0, 0, DMG_FIREBALL, BS_FLAGS1_TRIGGER_EVENTS) // 4 Damage
    Label(LBL_SKIP_DAMAGE)
    Call(EnableIdleScript, LVarA, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, LVarA, true)
    Return
    End
};

API_CALLABLE(N(SetBoltColors)) {
    Bytecode* args = script->ptrReadPos;
    EffectInstance* effect = (EffectInstance*) evt_get_variable(script, *args++);

    effect->data.lightningBolt->outerColor.r = 128;
    effect->data.lightningBolt->outerColor.g = 0;
    effect->data.lightningBolt->outerColor.b = 128;
    effect->data.lightningBolt->innerColor.r = 255;
    effect->data.lightningBolt->innerColor.g = 255;
    effect->data.lightningBolt->innerColor.b = 255;
    return ApiStatus_DONE2;
}

EvtScript N(EVS_Attack_Lightning) = {
	// Go to player
	Call(UseIdleAnimation, LVarA, false)
	Call(EnableIdleScript, LVarA, IDLE_SCRIPT_DISABLE)
	Call(SetTargetActor, LVarA, ACTOR_PLAYER)
	Call(BattleCamTargetActor, LVarA)
	Call(N(UnkBackgroundFunc3))
    Call(MakeLerp, 0, 200, 60, EASING_COS_IN_OUT)
    Label(0)
        Call(UpdateLerp)
        Call(N(SetBackgroundAlpha), LVar0)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(0)
        EndIf
	Wait(10)
	// Animation here
	Call(SetAnimation, LVarA, PRT_MAIN, ANIM_KingBoo_CrownAttack)
	Call(PlaySoundAtActor, LVarA, SOUND_SPELL_CAST4)
	Wait(60)
	Call(StartRumble, BTL_RUMBLE_HIT_EXTREME)
	Call(ShakeCam, CAM_BATTLE, 0, 5, Float(1.0))
	Call(PlaySoundAtActor, LVarA, SOUND_LIGHTNING_STRIKE)
	Call(EnemyTestTarget, LVarA, LVar0, 0, 0, 0, 0)
	Switch(LVar0)
		CaseOrEq(HIT_RESULT_MISS)
		CaseOrEq(HIT_RESULT_LUCKY)
			IfEq(LVar0, HIT_RESULT_LUCKY)
                Call(SetGoalToTarget, LVarA)
				Call(GetActorPos, LVarA, LVar0, LVar1, LVar2)
				Call(RandInt, 40, LVar1)
				Sub(LVar0, LVar1)
				Set(LVar1, 200)
				Call(GetActorPos, ACTOR_PLAYER, LVar3, LVar4, LVar5)
				Add(LVar3, 30)
				Set(LVar4, 0)
                PlayEffect(EFFECT_LIGHTNING_BOLT, 0, LVar0, LVar1, LVar2, LVar3, LVar4, LVar5, Float(0.5), 12, 0)
				Call(N(SetBoltColors), LVarF)
				Wait(12)
                Call(SetGoalToTarget, LVarA)
				Call(EnemyTestTarget, LVarA, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
                Call(GetActorPos, LVarA, LVar0, LVar1, LVar2)
                Add(LVar1, 20)
                //Call(SetAnimation, LVarA, PRT_MAIN, ANIM_KingBoo_Idle)
                Call(PlaySoundAtActor, LVarA, SOUND_EMOTE_IDEA)
                PlayEffect(EFFECT_EMOTE, EMOTE_EXCLAMATION, 0, LVar0, LVar1, LVar2, 24, 0, 25, 0, 0)
                Wait(10)
                Call(SetAnimation, LVarA, PRT_MAIN, ANIM_KingBoo_GetAngry)
                Wait(142)
			EndIf
			Goto(LBL_SKIP_DAMAGE)
	EndSwitch
	// Damage Mario
	Call(SetGoalToTarget, LVarA)
	Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
	Call(RandInt, 40, LVar1)
	Add(LVar0, LVar1)
	Add(LVar0, 50)
	Set(LVar1, 200)
	Call(GetGoalPos, LVarA, LVar3, LVar4, LVar5)
	Set(LVar4, 0)
    PlayEffect(EFFECT_LIGHTNING_BOLT, 0, LVar0, LVar1, LVar2, LVar3, LVar4, LVar5, Float(1.5), 12, 0)
	Call(N(SetBoltColors), LVarF)
	Wait(12)
	Call(SetGoalToTarget, LVarA)
	Call(EnemyDamageTarget, LVarA, LVar0, (DAMAGE_TYPE_MAGIC | DAMAGE_TYPE_SHOCK | DAMAGE_TYPE_NO_CONTACT), 0, SUPPRESS_EVENT_FLAG_10000, DMG_LIGHTNING, BS_FLAGS1_TRIGGER_EVENTS) // 5 Damage
	// Skip damage.
	Label(LBL_SKIP_DAMAGE)
    Call(MakeLerp, 200, 0, 60, EASING_LINEAR)
    Label(2)
        Call(UpdateLerp)
        Call(N(SetBackgroundAlpha), LVar0)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(2)
        EndIf
	Call(SetAnimation, LVarA, PRT_MAIN, ANIM_KingBoo_Idle)
	Call(UseIdleAnimation, LVarA, true)
	Call(EnableIdleScript, LVarA, IDLE_SCRIPT_ENABLE)
	Return
	End
};

// Vec3i N(CloneSummonPos) = { NPC_DISPOSE_LOCATION };

// Formation N(CloneFormation) = {
//     ACTOR_BY_POS(A(clone), N(CloneSummonPos), 0),
// };

EvtScript N(EVS_BooFlood) = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(GetActorVar, ACTOR_SELF, AVAR_KB_Phase, LVar6)
	IfEq(LVar6, AVAL_KB_FirstPhase)
		// // Boo 1
		// Call(SetNpcPos, 1, 0, 0, -125)
        // Call(PlaySoundAtNpc, 1, SOUND_BOO_APPEAR_A, SOUND_SPACE_DEFAULT)
        // SetF(LVar0, 0) // ALPHA
        // Call(SetNpcRenderMode, 1, RENDER_MODE_SURFACE_XLU_LAYER1)
        // Loop(8)
        //     Add(LVar0, 16)
        //     Call(SetNpcImgFXParams, 1, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        //     Wait(1)
        // EndLoop
		// Call(SetNpcAnimation, 1, ANIM_KingBoo_BooLaugh)
        // Call(NpcFlyTo, 1, 0, 300, -125)
        // // Boo 2
        // Call(SetNpcPos, 2, 5, -200, -125)
        // Call(PlaySoundAtNpc, 2, SOUND_BOO_APPEAR_A, SOUND_SPACE_DEFAULT)
        // SetF(LVar0, 0) // ALPHA
        // Call(SetNpcRenderMode, 2, RENDER_MODE_SURFACE_XLU_LAYER1)
        // Loop(8)
        //     Add(LVar0, 16)
        //     Call(SetNpcImgFXParams, 2, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        //     Wait(1)
        // EndLoop
        // Call(SetNpcAnimation, 2, ANIM_KingBoo_BooLaugh)
        // Call(NpcFlyTo, 2, 5, 300, -125, 20, 0, EASING_COS_IN_OUT)
        // // Boo 3
        // Call(SetNpcPos, 3, -10, -200, -125)
        // Call(PlaySoundAtNpc, 3, SOUND_BOO_APPEAR_A, SOUND_SPACE_DEFAULT)
        // SetF(LVar0, 0) // ALPHA
        // Call(SetNpcRenderMode, 3, RENDER_MODE_SURFACE_XLU_LAYER1)
        // Loop(8)
        //     Add(LVar0, 16)
        //     Call(SetNpcImgFXParams, 3, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        //     Wait(1)
        // EndLoop
        // Call(SetNpcAnimation, 3, ANIM_KingBoo_BooLaugh)
        // Call(NpcFlyTo, 3, -10, 300, -125, 20, 0, EASING_COS_IN_OUT)
        // // Boo 4
        // Call(SetNpcPos, 4, 10, -200, -125)
        // Call(PlaySoundAtNpc, 4, SOUND_BOO_APPEAR_A, SOUND_SPACE_DEFAULT)
        // SetF(LVar0, 0) // ALPHA
        // Call(SetNpcRenderMode, 4, RENDER_MODE_SURFACE_XLU_LAYER1)
        // Loop(8)
        //     Add(LVar0, 16)
        //     Call(SetNpcImgFXParams, 4, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        //     Wait(1)
        // EndLoop
        // Call(SetNpcAnimation, 4, ANIM_KingBoo_BooLaugh)
        // Call(NpcFlyTo, 4, 10, 300, -125, 20, 0, EASING_COS_IN_OUT)
        // // Boo 5
        // Call(SetNpcPos, 5, -5, -200, -125)
        // Call(PlaySoundAtNpc, 5, SOUND_BOO_APPEAR_A, SOUND_SPACE_DEFAULT)
        // SetF(LVar0, 0) // ALPHA
        // Call(SetNpcRenderMode, 5, RENDER_MODE_SURFACE_XLU_LAYER1)
        // Loop(8)
        //     Add(LVar0, 16)
        //     Call(SetNpcImgFXParams, 5, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        //     Wait(1)
        // EndLoop
        // Call(SetNpcAnimation, 5, ANIM_KingBoo_BooLaugh)
        // Call(NpcFlyTo, 5, -5, 300, -125, 20, 0, EASING_COS_IN_OUT)
        // // Boo 6
        // Call(SetNpcPos, 6, 0, -200, -125)
        // Call(PlaySoundAtNpc, 6, SOUND_BOO_APPEAR_A, SOUND_SPACE_DEFAULT)
        // SetF(LVar0, 0) // ALPHA
        // Call(SetNpcRenderMode, 6, RENDER_MODE_SURFACE_XLU_LAYER1)
        // Loop(8)
        //     Add(LVar0, 16)
        //     Call(SetNpcImgFXParams, 6, IMGFX_SET_ALPHA, LVar0, 0, 0, 0)
        //     Wait(1)
        // EndLoop
        // Call(SetNpcAnimation, 6, ANIM_KingBoo_BooLaugh)
        // Call(NpcFlyTo, 6, 0, 300, -125, 20, 0, EASING_COS_IN_OUT)
	EndIf
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar1, -5)
    Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_BooAbsorb)
	Wait(50)
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_BooAbsorbEnd)
	Wait(20)
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOO_VANISH_A)
	SetF(LVar3, 256)
	Loop(16)
		Sub(LVar3, 16)
		Call(SetPartAlpha, ACTOR_SELF, PRT_MAIN, LVar3)
		Wait(1)
	EndLoop
	Wait(5)
    Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
    Add(LVar1, 5)
    Call(SetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Idle)
	Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, true)
	//Call(SetNpcAnimation, 1, ANIM_KingBoo_BooWellSingle)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript N(EVS_IllusoryClones) = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_CrownAttack)
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SPELL_CAST4)
	Wait(80)
	ExecWait(N(EVS_BooFlood))
	Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(UseIdleAnimation, ACTOR_SELF, false)

    Call(SetActorVar, ACTOR_CLONE_1, AVAR_Clone_Type, AVAL_Type_1)
	Call(SetEnemyHP, ACTOR_CLONE_1, 10)
    Call(SetActorFlagBits, ACTOR_CLONE_1, ACTOR_FLAG_NO_ATTACK | ACTOR_FLAG_SKIP_TURN, false)
    Call(SetPartFlagBits, ACTOR_CLONE_1, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, false)
    Call(SetActorVar, ACTOR_CLONE_2, AVAR_Clone_Type, AVAL_Type_2)
	Call(SetEnemyHP, ACTOR_CLONE_2, 10)
    Call(SetActorFlagBits, ACTOR_CLONE_2, ACTOR_FLAG_NO_ATTACK | ACTOR_FLAG_SKIP_TURN, false)
    Call(SetPartFlagBits, ACTOR_CLONE_2, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, false)
    Call(SetActorVar, ACTOR_CLONE_3, AVAR_Clone_Type, AVAL_Type_3)
	Call(SetEnemyHP, ACTOR_CLONE_3, 10)
    Call(SetActorFlagBits, ACTOR_CLONE_3, ACTOR_FLAG_NO_ATTACK | ACTOR_FLAG_SKIP_TURN, false)
    Call(SetPartFlagBits, ACTOR_CLONE_3, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, false)

	// King Boo
	Call(SetActorFlagBits, ACTOR_SELF, ACTOR_FLAG_NO_SHADOW, true)

	// Clone 1 Vanish
	Call(GetActorPos, ACTOR_KING_BOO, LVar0, LVar1, LVar2)
	SetF(LVar3, 0)
	Call(SetPartAlpha, ACTOR_CLONE_1, PRT_MAIN, LVar3)
	Call(SetActorPos, ACTOR_CLONE_1, LVar0, LVar1, LVar2)
	Call(SetActorFlagBits, ACTOR_CLONE_1, ACTOR_FLAG_NO_SHADOW, true)

    // Clone 2 Vanish
	Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
	SetF(LVar3, 0)
	Call(SetPartAlpha, ACTOR_CLONE_2, PRT_MAIN, LVar3)
	Call(SetActorPos, ACTOR_CLONE_2, LVar0, LVar1, LVar2)
	Wait(1)
	Call(SetActorFlagBits, ACTOR_CLONE_2, ACTOR_FLAG_NO_SHADOW, true)

    // Clone 3 Vanish
	Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
	SetF(LVar3, 0)
	Call(SetPartAlpha, ACTOR_CLONE_3, PRT_MAIN, LVar3)
	Call(SetActorPos, ACTOR_CLONE_3, LVar0, LVar1, LVar2)
	Wait(1)
	Call(SetActorFlagBits, ACTOR_CLONE_3, ACTOR_FLAG_NO_SHADOW, true)

	ExecWait(N(EVS_ClonePosition))

	// King Boo
	Call(GetActorPos, ACTOR_KING_BOO, LVar0, LVar1, LVar2)
    Set(LVar1, -1000)
	Call(SetActorPos, ACTOR_KING_BOO, LVar0, LVar1, LVar2)
	Call(ForceHomePos, ACTOR_KING_BOO, LVar0, LVar1, LVar2)
    // Call(SetActorFlagBits, ACTOR_KING_BOO, ACTOR_FLAG_NO_ATTACK, true)
    Call(SetActorFlagBits, ACTOR_KING_BOO, ACTOR_FLAG_INVISIBLE | ACTOR_FLAG_NO_HEALTH_BAR, true)
    Call(SetPartFlagBits, ACTOR_KING_BOO, PRT_MAIN, ACTOR_PART_FLAG_NO_TARGET, true)
	Call(HPBarToHome, ACTOR_KING_BOO)

	// Clone 1
	Call(GetActorPos, ACTOR_CLONE_1, LVar0, LVar1, LVar2)
	Call(ForceHomePos, ACTOR_CLONE_1, LVar0, LVar1, LVar2)
	Call(HPBarToHome, ACTOR_CLONE_1)

	// Clone 2
	Call(GetActorPos, ACTOR_CLONE_2, LVar0, LVar1, LVar2)
	Call(ForceHomePos, ACTOR_CLONE_2, LVar0, LVar1, LVar2)
	Call(HPBarToHome, ACTOR_CLONE_2)

	// Clone 3
	Call(GetActorPos, ACTOR_CLONE_3, LVar0, LVar1, LVar2)
	Call(ForceHomePos, ACTOR_CLONE_3, LVar0, LVar1, LVar2)
	Call(HPBarToHome, ACTOR_CLONE_3)

	Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
	Call(MoveBattleCamOver, 20)
	// Clone 1 Appears
    Thread
        Call(UseIdleAnimation, ACTOR_CLONE_1, false)
        Call(SetAnimation, ACTOR_CLONE_1, PRT_MAIN, ANIM_KingBoo_Laugh)
        Call(PlaySoundAtActor, ACTOR_CLONE_1, SOUND_BOO_APPEAR_A)
        SetF(LVar0, 0)
        Loop(16)
            Add(LVar0, 16)
            Call(SetPartAlpha, ACTOR_CLONE_1, PRT_MAIN, LVar0)
            Wait(1)
        EndLoop
    EndThread

	// Clone 2 Appears
    Thread
        Call(UseIdleAnimation, ACTOR_CLONE_2, false)
        Call(SetAnimation, ACTOR_CLONE_2, PRT_MAIN, ANIM_KingBoo_Laugh)
        Call(PlaySoundAtActor, ACTOR_CLONE_2, SOUND_BOO_APPEAR_A)
        SetF(LVar0, 0)
        Loop(16)
            Add(LVar0, 16)
            Call(SetPartAlpha, ACTOR_CLONE_2, PRT_MAIN, LVar0)
            Wait(1)
        EndLoop
    EndThread

	// Clone 3 Appears
	Call(UseIdleAnimation, ACTOR_CLONE_3, false)
	Call(SetAnimation, ACTOR_CLONE_3, PRT_MAIN, ANIM_KingBoo_Laugh)
	Call(PlaySoundAtActor, ACTOR_CLONE_3, SOUND_BOO_APPEAR_A)
	SetF(LVar0, 0)
	Loop(16)
		Add(LVar0, 16)
		Call(SetPartAlpha, ACTOR_CLONE_3, PRT_MAIN, LVar0)
		Wait(1)
	EndLoop

	Wait(15)
	Call(UseIdleAnimation, ACTOR_CLONE_1, true)
	Call(UseIdleAnimation, ACTOR_CLONE_2, true)
	Call(UseIdleAnimation, ACTOR_CLONE_3, true)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

EvtScript N(EVS_ClonePosition) = {
    Set(LVarA, ACTOR_CLONE_2)
    Set(LVarB, ACTOR_CLONE_3)
    Set(LVarC, ACTOR_CLONE_1)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 150)

	// LVarA
	Call(GetActorPos, LVarA, LVar0, LVar1, LVar2)
	Set(LVar0, 20)
	Set(LVar2, 5)
	Call(SetActorPos, LVarA, LVar0, LVar1, LVar2)
    // DebugPrintf("LVarA Pos: %d %d %d\n", LVar0, LVar1, LVar2)

	// LVarB
	Call(GetActorPos, LVarB, LVar0, LVar1, LVar2)
	Set(LVar0, 70)
	Set(LVar2, 5)
	Call(SetActorPos, LVarB, LVar0, LVar1, LVar2)
    // DebugPrintf("LVarB Pos: %d %d %d\n", LVar0, LVar1, LVar2)

	// LVarC
	Call(GetActorPos, LVarC, LVar0, LVar1, LVar2)
	Set(LVar0, 120)
	Set(LVar2, 5)
	Call(SetActorPos, LVarC, LVar0, LVar1, LVar2)
    // DebugPrintf("LVarC Pos: %d %d %d\n", LVar0, LVar1, LVar2)

    Wait(15)
    Return
    End
};

EvtScript N(EVS_Attack_FakeoutScareA) = {
    Call(UseIdleAnimation, LVarA, false)
    Call(EnableIdleScript, LVarA, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, LVarA, ACTOR_PLAYER)
    Call(PlaySoundAtActor, LVarA, SOUND_BOO_VANISH_A)
    SetF(LVar0, 256)
    Loop(16)
        Sub(LVar0, 16)
        Call(SetPartAlpha, LVarA, PRT_MAIN, LVar0)
        Wait(1)
    EndLoop
    Call(SetAnimation, LVarA, PRT_MAIN, ANIM_KingBoo_Hide)
    Wait(20)
    Call(UseBattleCamPreset, BTL_CAM_REPOSITION)
    Call(SetGoalToTarget, LVarA)
    Call(GetGoalPos, LVarA, LVar0, LVar1, LVar2)
    Add(LVar0, 20)
    Set(LVar1, 0)
    Call(SetBattleCamTarget, LVar0, LVar1, LVar2)
	Call(SetBattleCamDist, 350)
	Call(SetBattleCamOffsetY, 45)
	Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
	Call(MoveBattleCamOver, 20)
    Call(SetGoalToTarget, LVarA)
    Call(GetGoalPos, LVarA, LVar0, LVar1, LVar2)
    Add(LVar0, 40)
    Set(LVar1, 10)
    Call(SetGoalPos, LVarA, LVar0, LVar1, LVar2)
    Call(SetActorSpeed, LVarA, Float(10.0))
    Call(FlyToGoal, LVarA, 0, 0, EASING_COS_IN_OUT)
    Call(PlaySoundAtActor, LVarA, SOUND_BOO_APPEAR_A)
    SetF(LVar0, 0)
    Loop(16)
        Add(LVar0, 16)
        Call(SetPartAlpha, LVarA, PRT_MAIN, LVar0)
        Wait(1)
    EndLoop
    // randomized delay
    Call(RandInt, 20, LVar0)
    Wait(LVar0)
    Call(SetAnimation, LVarA, PRT_MAIN, ANIM_KingBoo_DoubleScareFakeoutA)
    Wait(55)
    Call(PlaySoundAtActor, LVarA, SOUND_BOO_APPEAR_A)
    Wait(60)
    Call(EnemyTestTarget, LVarA, LVar0, 0, 0, 0, 0)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            IfEq(LVar0, HIT_RESULT_LUCKY)
                Call(SetGoalToTarget, LVarA)
                Call(EnemyTestTarget, LVarA, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Goto(LBL_SKIP_DAMAGE)
        EndCaseGroup
    EndSwitch
    Call(PlaySoundAtActor, LVarA, SOUND_BOW_SPOOK)
    Wait(5)
    Call(SetGoalToTarget, LVarA)
    Call(EnemyDamageTarget, LVarA, LVar0, DAMAGE_TYPE_NO_CONTACT, 0, 0, DMG_FAKEOUT_SCARE_A, BS_FLAGS1_TRIGGER_EVENTS) // 4 Damage
    // Call(PlaySoundAtPart, LVarA PRT_BOO SOUND_BOO_VANISH_A)
    Label(LBL_SKIP_DAMAGE)
    Wait(45)
    Call(PlaySoundAtActor, LVarA, SOUND_BOO_VANISH_A)
    SetF(LVar0, 256)
    Loop(16)
        Sub(LVar0, 16)
        Call(SetPartAlpha, LVarA, PRT_MAIN, LVar0)
        Wait(1)
    EndLoop
    Wait(10)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 20)
    Call(SetActorSpeed, LVarA, Float(10.0))
    Call(SetGoalToHome, LVarA)
    Call(FlyToGoal, LVarA, 30, 0, EASING_COS_IN_OUT)
    Call(HPBarToHome, LVarA)
    Call(SetAnimation, LVarA, PRT_MAIN, ANIM_KingBoo_Idle)
    Call(PlaySoundAtActor, LVarA, SOUND_BOO_APPEAR_A)
    SetF(LVar0, 0)
    Loop(16)
        Add(LVar0, 16)
        Call(SetPartAlpha, LVarA, PRT_MAIN, LVar0)
        Wait(1)
    EndLoop
    Call(UseIdleAnimation, LVarA, true)
    Call(EnableIdleScript, LVarA, IDLE_SCRIPT_ENABLE)
    Return
    End
};

EvtScript N(EVS_Attack_FakeoutScareB) = {
    Call(UseIdleAnimation, LVarA, false)
    Call(EnableIdleScript, LVarA, IDLE_SCRIPT_DISABLE)
    Call(SetTargetActor, LVarA, ACTOR_PLAYER)
    Call(PlaySoundAtActor, LVarA, SOUND_BOO_VANISH_A)
    SetF(LVar0, 256)
    Loop(16)
        Sub(LVar0, 16)
        Call(SetPartAlpha, LVarA, PRT_MAIN, LVar0)
        Wait(1)
    EndLoop
    Call(SetAnimation, LVarA, PRT_MAIN, ANIM_KingBoo_Hide)
    Wait(20)
    Call(UseBattleCamPreset, BTL_CAM_REPOSITION)
    Call(SetGoalToTarget, LVarA)
    Call(GetGoalPos, LVarA, LVar0, LVar1, LVar2)
    Add(LVar0, 20)
    Set(LVar1, 0)
    Call(SetBattleCamTarget, LVar0, LVar1, LVar2)
	Call(SetBattleCamDist, 350)
	Call(SetBattleCamOffsetY, 45)
	Call(SetBattleCamTargetingModes, BTL_CAM_YADJ_TARGET, BTL_CAM_XADJ_AVG, false)
	Call(MoveBattleCamOver, 20)
    Call(SetGoalToTarget, LVarA)
    Call(GetGoalPos, LVarA, LVar0, LVar1, LVar2)
    Add(LVar0, 45)
    Set(LVar1, 10)
    Call(SetGoalPos, LVarA, LVar0, LVar1, LVar2)
    Call(SetActorSpeed, LVarA, Float(10.0))
    Call(FlyToGoal, LVarA, 0, 0, EASING_COS_IN_OUT)
    Call(PlaySoundAtActor, LVarA, SOUND_BOO_APPEAR_A)
    SetF(LVar0, 0)
    Loop(16)
        Add(LVar0, 16)
        Call(SetPartAlpha, LVarA, PRT_MAIN, LVar0)
        Wait(1)
    EndLoop
    // randomized delay
    Call(RandInt, 20, LVar0)
    Wait(LVar0)
    Call(SetAnimation, LVarA, PRT_MAIN, ANIM_KingBoo_DoubleScareFakeoutB)
    Wait(50)
    Call(PlaySoundAtActor, LVarA, SOUND_BOO_APPEAR_A)
    Wait(30)
    Call(EnemyTestTarget, LVarA, LVar0, 0, 0, 0, 0)
    Switch(LVar0)
        CaseOrEq(HIT_RESULT_MISS)
        CaseOrEq(HIT_RESULT_LUCKY)
            IfEq(LVar0, HIT_RESULT_LUCKY)
                Call(SetGoalToTarget, LVarA)
                Call(EnemyTestTarget, LVarA, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
            EndIf
            Goto(LBL_SKIP_DAMAGE)
        EndCaseGroup
    EndSwitch
    Call(GetActorPos, LVarA, LVar0, LVar1, LVar2)
    Set(LVar0, -130)
    Set(LVar1, -2)
    Call(SetPartPos, ACTOR_SELF, PRT_BOO, LVar0, LVar1, LVar2)
    Call(SetTargetActor, LVarA, ACTOR_PLAYER)
    Call(SetGoalToTarget, LVarA)
    Call(GetGoalPos, LVarA, LVar0, LVar1, LVar2)
    Call(FlyPartTo, ACTOR_SELF, PRT_BOO, LVar0, LVar1, LVar2, 0, 0, EASING_COS_IN_OUT)
    Call(PlaySoundAtActor, LVarA, SOUND_BOW_SPOOK)
    Wait(5)
    Call(SetGoalToTarget, LVarA)
    Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_NO_CONTACT | DAMAGE_TYPE_STATUS_ALWAYS_HITS, SUPPRESS_EVENT_ALL, DMG_STATUS_IGNORE_RES(STATUS_FLAG_SHRINK, 2), DMG_FAKEOUT_SCARE_B, BS_FLAGS1_TRIGGER_EVENTS) // 2 Damage
    // Call(PlaySoundAtPart, ACTOR_SELF, PRT_BOO SOUND_BOO_VANISH_A)
    Set(LVar0, 0)
    Set(LVar1, 0)
    Set(LVar2, 0)
    Call(SetPartPos, ACTOR_SELF, PRT_BOO, LVar0, LVar1, LVar2)
    Label(LBL_SKIP_DAMAGE)
    Wait(45)
    Call(PlaySoundAtActor, LVarA, SOUND_BOO_VANISH_A)
    SetF(LVar0, 256)
    Loop(16)
        Sub(LVar0, 16)
        Call(SetPartAlpha, LVarA, PRT_MAIN, LVar0)
        Wait(1)
    EndLoop
    Wait(10)
    Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
    Call(MoveBattleCamOver, 20)
    Call(SetActorSpeed, LVarA, Float(10.0))
    Call(SetGoalToHome, LVarA)
    Call(FlyToGoal, LVarA, 30, 0, EASING_COS_IN_OUT)
    Call(HPBarToHome, LVarA)
    Call(SetAnimation, LVarA, PRT_MAIN, ANIM_KingBoo_Idle)
    Call(PlaySoundAtActor, LVarA, SOUND_BOO_APPEAR_A)
    SetF(LVar0, 0)
    Loop(16)
        Add(LVar0, 16)
        Call(SetPartAlpha, LVarA, PRT_MAIN, LVar0)
        Wait(1)
    EndLoop
    Call(EnableIdleScript, LVarA, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, LVarA, true)
    Return
    End
};

EvtScript N(EVS_Attack_BoostedLightning) = {
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    // ExecWait(N(EVS_Move_Buff))
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOO_VANISH_A)
	Wait(10)
	Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
	Call(MoveBattleCamOver, 20)
	Call(SetActorSpeed, ACTOR_SELF, Float(10.0))
	Call(SetGoalPos, ACTOR_SELF, 70, 50, 5)
	Call(FlyToGoal, ACTOR_SELF, 30, 0, EASING_COS_IN_OUT)
	Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
	Call(ForceHomePos, ACTOR_SELF, LVar0, LVar1, LVar2)
	Call(HPBarToHome, ACTOR_SELF)
    Call(UseIdleAnimation, ACTOR_SELF, false)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
    Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Idle)
    Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
	Call(BattleCamTargetActor, ACTOR_SELF)
	Call(N(UnkBackgroundFunc3))
    Call(MakeLerp, 0, 200, 60, EASING_COS_IN_OUT)
    Label(0)
        Call(UpdateLerp)
        Call(N(SetBackgroundAlpha), LVar0)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(0)
        EndIf
	Wait(10)
	// Animation here
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_CrownAttack)
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_SPELL_CAST4)
	Wait(55)
	Call(StartRumble, BTL_RUMBLE_PLAYER_MAX)
	Call(ShakeCam, CAM_BATTLE, 0, 5, Float(2.0))
	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_LIGHTNING_STRIKE)
	Call(EnemyTestTarget, ACTOR_SELF, LVar0, 0, 0, 0, 0)
	Switch(LVar0)
		CaseOrEq(HIT_RESULT_MISS)
		CaseOrEq(HIT_RESULT_LUCKY)
			IfEq(LVar0, HIT_RESULT_LUCKY)
                Call(SetGoalToTarget, ACTOR_SELF)
				Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
				Call(RandInt, 40, LVar1)
				Sub(LVar0, LVar1)
				Set(LVar1, 200)
				Call(GetActorPos, ACTOR_PLAYER, LVar3, LVar4, LVar5)
				Add(LVar3, 30)
				Set(LVar4, 0)
                PlayEffect(EFFECT_LIGHTNING_BOLT, 0, LVar0, LVar1, LVar2, LVar3, LVar4, LVar5, Float(1.0), 12, 0)
				Call(N(SetBoltColors), LVarF)
				Wait(12)
                Call(SetGoalToTarget, ACTOR_SELF)
				Call(EnemyTestTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_TRIGGER_LUCKY, 0, 0, 0)
                Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
                Add(LVar1, 20)
                //Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Idle)
                Call(PlaySoundAtActor, ACTOR_SELF, SOUND_EMOTE_IDEA)
                PlayEffect(EFFECT_EMOTE, EMOTE_EXCLAMATION, 0, LVar0, LVar1, LVar2, 24, 0, 25, 0, 0)
                Wait(10)
                Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_GetAngry)
                Wait(35)
			EndIf
			Goto(LBL_SKIP_DAMAGE)
	EndSwitch
	// Damage Mario
    Call(SetGoalToTarget, ACTOR_SELF)
	Call(GetActorPos, ACTOR_PLAYER, LVar0, LVar1, LVar2)
	Call(RandInt, 40, LVar1)
	Add(LVar0, LVar1)
	Add(LVar0, 50)
	Set(LVar1, 200)
	Call(GetGoalPos, ACTOR_SELF, LVar3, LVar4, LVar5)
	Set(LVar4, 0)
    PlayEffect(EFFECT_LIGHTNING_BOLT, 0, LVar0, LVar1, LVar2, LVar3, LVar4, LVar5, Float(2.5), 12, 0)
	Call(N(SetBoltColors), LVarF)
	Wait(12)
    Call(SetGoalToTarget, ACTOR_SELF)
	Call(EnemyDamageTarget, ACTOR_SELF, LVar0, (DAMAGE_TYPE_MAGIC | DAMAGE_TYPE_SHOCK | DAMAGE_TYPE_NO_CONTACT), 0, SUPPRESS_EVENT_FLAG_10000, DMG_BOOSTED_LIGHTNING, BS_FLAGS1_TRIGGER_EVENTS) // 5 Damage
	// Skip damage.
	Label(LBL_SKIP_DAMAGE)
    Call(SetActorSpeed, ACTOR_SELF, Float(10.0))
	Call(SetGoalPos, ACTOR_SELF, 70, 50, 5)
	Call(FlyToGoal, ACTOR_SELF, 30, 0, EASING_COS_IN_OUT)
	Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
	Call(ForceHomePos, ACTOR_SELF, LVar0, LVar1, LVar2)
	Call(HPBarToHome, ACTOR_SELF)
    Call(MakeLerp, 200, 0, 60, EASING_LINEAR)
    Label(2)
        Call(UpdateLerp)
        Call(N(SetBackgroundAlpha), LVar0)
        Wait(1)
        IfEq(LVar1, 1)
            Goto(2)
        EndIf
	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Idle)
    Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
    Call(UseIdleAnimation, ACTOR_SELF, true)
    Return
    End
};

// EvtScript N(EVS_Move_Heal) = {
//     Call(UseIdleAnimation, ACTOR_SELF, false)
//     Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
//     ExecWait(N(EVS_Move_Buff))
// 	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOO_VANISH_A)
// 	SetF(LVar0, 256)
// 	Loop(16)
// 		Sub(LVar0, 16)
// 		Call(SetPartAlpha, ACTOR_SELF, PRT_MAIN, LVar0)
// 		Wait(1)
// 	EndLoop
// 	Wait(10)
// 	Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
// 	Call(MoveBattleCamOver, 20)
// 	Call(SetActorSpeed, ACTOR_SELF, Float(10.0))
// 	Call(SetGoalPos, ACTOR_SELF, 70, 50, 5)
// 	Call(FlyToGoal, ACTOR_SELF, 30, 0, EASING_COS_IN_OUT)
// 	Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
// 	Call(ForceHomePos, ACTOR_SELF, LVar0, LVar1, LVar2)
// 	Call(HPBarToHome, ACTOR_SELF)
// 	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_BOO_APPEAR_A)
// 	SetF(LVar0, 0)
// 	Loop(16)
// 		Add(LVar0, 16)
// 		Call(SetPartAlpha, ACTOR_SELF, PRT_MAIN, LVar0)
// 		Wait(1)
// 	EndLoop
// 	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Idle)
// 	Thread
// 		Wait(5)
// 		Call(PlaySoundAtActor, ACTOR_SELF, SOUND_RECOVER_HEART)
// 		Call(PlaySoundAtActor, ACTOR_SELF, SOUND_HEART_BOUNCE)
// 		Wait(30)
// 		Call(PlaySoundAtActor, ACTOR_SELF, SOUND_STAR_BOUNCE_A)
// 	EndThread
// 	Thread
// 		Call(FreezeBattleState, true)
// 		Call(HealActor, ACTOR_SELF, 10, false)
// 		Call(FreezeBattleState, false)
// 	EndThread
//     Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
//     Call(UseIdleAnimation, ACTOR_SELF, true)
//     Return
//     End
// };

// EvtScript N(EVS_Init_Removal) = {
//     // LVar9 ActorID
//     // LVarA ActorExists
//     // LVarB CountRemoved
//     Set(LVarB, 0)
//     // Clone 1
//     Call(GetActorVar, ACTOR_SELF, AVAR_Clone1_ID, LVar9)
//     Call(ActorExists, LVar9, LVarA)
//     IfEq(LVarA, true)
//         Exec(N(EVS_RemoveClone))
//         Add(LVarB, 1)
//     EndIf
//     // Clone 2
//     Call(GetActorVar, ACTOR_SELF, AVAR_Clone2_ID, LVar9)
//     Call(ActorExists, LVar9, LVarA)
//     IfEq(LVarA, true)
//         Exec(N(EVS_RemoveClone))
//         Add(LVarB, 1)
//     EndIf
//     // Clone 3
//     Call(GetActorVar, ACTOR_SELF, AVAR_Clone3_ID, LVar9)
//     Call(ActorExists, LVar9, LVarA)
//     IfEq(LVarA, true)
//         Exec(N(EVS_RemoveClone))
//         Add(LVarB, 1)
//     EndIf
//     // Wait if executing removal
//     IfGt(LVarB, 0)
//         Wait(20)
//     EndIf
//     Return
//     End
// };

// EvtScript N(EVS_Move_Buff) = {
//     Call(UseIdleAnimation, ACTOR_SELF, false)
//     Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_DISABLE)
//     Call(GetActorVar, ACTOR_SELF, AVAR_Clone1_ID, LVar0)
// 	Call(UseIdleAnimation, LVar0, false)
// 	Call(SetAnimation, LVar0, PRT_MAIN, ANIM_KingBoo_CrownAttack)
// 	Call(PlaySoundAtActor, LVar0, SOUND_SPELL_CAST4)
// 	Call(GetActorVar, ACTOR_SELF, AVAR_Clone2_ID, LVar0)
// 	Call(UseIdleAnimation, LVar0, false)
// 	Call(SetAnimation, LVar0, PRT_MAIN, ANIM_KingBoo_CrownAttack)
// 	Call(PlaySoundAtActor, LVar0, SOUND_SPELL_CAST4)
// 	Wait(80)
// 	Call(GetActorVar, ACTOR_SELF, AVAR_Clone1_ID, LVar0)
// 	Call(UseIdleAnimation, LVar0, true)
// 	Call(GetActorVar, ACTOR_SELF, AVAR_Clone2_ID, LVar0)
// 	Call(UseIdleAnimation, LVar0, true)
// 	Call(PlaySoundAtActor, ACTOR_SELF, SOUND_POWER_UP)
// 	Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
// 	Add(LVar1, 20)
// 	PlayEffect(EFFECT_ENERGY_IN_OUT, 6, LVar0, LVar1, LVar2, Float(1.0), 45, 0)
// 	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Laugh)
// 	Wait(30)

//     // LVarA TID
//     // LVarB ScriptComplete
//     ExecGetTID(N(EVS_Init_Removal), LVarA)
//     Label(0)
//     IsThreadRunning(LVarA, LVarB)
//     IfEq(LVar1, true)
//         Wait(1)
//         Goto(0)
//     EndIf
// 	Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_KingBoo_Hide)
//     Call(EnableIdleScript, ACTOR_SELF, IDLE_SCRIPT_ENABLE)
//     Call(UseIdleAnimation, ACTOR_SELF, true)
//     Return
//     End
// };
