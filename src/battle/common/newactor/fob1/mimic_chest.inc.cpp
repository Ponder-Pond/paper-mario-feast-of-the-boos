// #include "../area.h"
// #include "entity.h"
// #include "sprite/npc/BattleMimicChest.h"

// #define NAMESPACE A(mimic_chest)

// extern EvtScript EVS_Init;
// extern EvtScript EVS_Idle;
// extern EvtScript EVS_TakeTurn;
// extern EvtScript EVS_HandleEvent;

// enum N(ActorPartIDs) {
//     PRT_MAIN = 1,
// };

// enum N(ActorVars) {
//     AVAR_PlayerTurnCount = 0,
//     AVAR_Speaking = 1,
// };

// enum N(ActorParams) {
//     DMG_CHEST_ATTACK = 5,
// };

// s32 DefaultAnims[] = {
//     STATUS_KEY_NORMAL, ANIM_BattleMimicChest_Idle,
//     STATUS_END,
// };

// s32 DefenseTable[] = {
//     ELEMENT_NORMAL, 0,
//     ELEMENT_END,
// };

// s32 StatusTable[] = {
//     STATUS_KEY_NORMAL, 0,
//     STATUS_KEY_DEFAULT, 0,
//     STATUS_END,
// };

// ActorPartBlueprint ActorParts[] = {
//     {
//         .flags = ACTOR_PART_FLAG_PRIMARY_TARGET,
//         .index = PRT_MAIN,
//         .posOffset = { 0, 0, 0 },
//         .targetOffset = { -10, 35 },
//         .opacity = 255,
//         .idleAnimations = DefaultAnims,
//         .defenseTable = DefenseTable,
//         .eventFlags = 0,
//         .elementImmunityFlags = 0,
//         .projectileTargetOffset = { 0, -8 },
//     },
// };

// ActorBlueprint NAMESPACE = {
//     .flags = 0,
//     .type = ACTOR_TYPE_MIMIC_CHEST,
//     .level = ACTOR_LEVEL_MIMIC_CHEST,
//     .maxHP = 20,
//     .partCount = ARRAY_COUNT(ActorParts),
//     .partsData = ActorParts,
//     .initScript = &EVS_Init,
//     .statusTable = StatusTable,
//     .escapeChance = 0,
//     .airLiftChance = 0,
//     .hurricaneChance = 0,
//     .spookChance = 0,
//     .upAndAwayChance = 0,
//     .spinSmashReq = 0,
//     .powerBounceChance = 80,
//     .coinReward = 0,
//     .size = { 32, 32 },
//     .healthBarOffset = { 0, 0 },
//     .statusIconOffset = { -10, 20 },
//     .statusTextOffset = { 10, 20 },
// };

// EvtScript EVS_Init = {
//     Call(BindTakeTurn, ACTOR_SELF, Ref(EVS_TakeTurn))
//     Call(BindIdle, ACTOR_SELF, Ref(EVS_Idle))
//     Call(BindHandleEvent, ACTOR_SELF, Ref(EVS_HandleEvent))
//     Call(SetActorVar, ACTOR_SELF, AVAR_PlayerTurnCount, 0)
//     Call(SetActorVar, ACTOR_SELF, AVAR_Speaking, false)
//     Return
//     End
// };

// API_CALLABLE(N(AnimateChestTransformation)) {
//     Entity* entity = get_entity_by_index(script->varTable[9]);

//     if (isInitialCall) {
//         script->functionTemp[0] = 0;
//     }

//     entity->scale.x = 1.0f + (script->functionTemp[0] / 30.0f);
//     entity->scale.y = 1.0f + (script->functionTemp[0] / 30.0f);
//     entity->scale.z = 1.0f + (script->functionTemp[0] / 30.0f);

//     script->functionTemp[0]++;
//     if (script->functionTemp[0] >= 30) {
//         return ApiStatus_DONE2;
//     }
//     return ApiStatus_BLOCK;
// }

// EvtScript EVS_Idle = {
//     Loop(0)
//         Wait(1)
//     EndLoop
//     Return
//     End
// };

// EvtScript EVS_HandleEvent = {
//     Call(UseIdleAnimation, ACTOR_SELF, false)
//     Call(GetLastEvent, ACTOR_SELF, LVar0)
//     Switch(LVar0)
//         CaseEq(EVENT_BEGIN_FIRST_STRIKE)
//         CaseOrEq(EVENT_HIT_COMBO)
//         CaseOrEq(EVENT_HIT)
//             SetConst(LVar0, PRT_MAIN)
//             SetConst(LVar1, ANIM_BattleMimicChest_Hurt)
//             ExecWait(EVS_Enemy_Hit)
//         EndCaseGroup
//         CaseEq(EVENT_DEATH)
//             SetConst(LVar0, PRT_MAIN)
//             SetConst(LVar1, ANIM_BattleMimicChest_Hurt)
//             ExecWait(EVS_Enemy_Hit)
//             Wait(10)
//             Return
//         CaseDefault
//     EndSwitch
//     Call(UseIdleAnimation, ACTOR_SELF, true)
//     Return
//     End
// };

// EvtScript EVS_TakeTurn = {
//     Call(UseIdleAnimation, ACTOR_SELF, false)
//     Call(SetTargetActor, ACTOR_SELF, ACTOR_PLAYER)
//     Call(UseBattleCamPreset, BTL_CAM_ACTOR)
//     Call(BattleCamTargetActor, ACTOR_SELF)
//     Wait(30)
//     Call(SetAnimation, ACTOR_SELF, PRT_MAIN, ANIM_BattleMimicChest_Attack)
//     Wait(5)
//     Call(PlaySoundAtActor, ACTOR_SELF, SOUND_MIMIC_CHEST_ATTACK)
//     Call(GetActorPos, ACTOR_SELF, LVar0, LVar1, LVar2)
//     Sub(LVar0, 25)
//     Add(LVar1, 40)
//     PlayEffect(EFFECT_GATHER_ENERGY, 0, LVar0, LVar1, LVar2, 1, 100, 0)
//     Wait(20)
//     Call(UseBattleCamPreset, BTL_CAM_REPOSITION)
//     Call(SetBattleCamTarget, 80, 95, 15)
//     Call(SetBattleCamDist, 340)
//     Call(MoveBattleCamOver, 60)
//     Call(PlaySoundAtActor, ACTOR_SELF, SOUND_MIMIC_CHEST_IMPACT)
//     Wait(20)
//     Call(EnemyDamageTarget, ACTOR_SELF, LVar0, DAMAGE_TYPE_MAGIC | DAMAGE_TYPE_NO_CONTACT, 0, 0, DMG_CHEST_ATTACK, BS_FLAGS1_TRIGGER_EVENTS)
//     Set(LVarF, LVar0)
//     Switch(LVarF)
//         CaseOrEq(HIT_RESULT_HIT)
//         CaseOrEq(HIT_RESULT_NO_DAMAGE)
//             Wait(19)
//             Call(UseBattleCamPreset, BTL_CAM_DEFAULT)
//         EndCaseGroup
//     EndSwitch
//     Wait(10)
//     Call(UseIdleAnimation, ACTOR_SELF, true)
//     Return
//     End
// };
