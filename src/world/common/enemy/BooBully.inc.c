#include "BooBully.h"

#include "world/common/enemy/ai/FlyingAI.inc.c"

MobileAISettings N(AISettings_BooBully) = {
    .moveSpeed = 1.0f,
    .moveTime = 30,
    .waitTime = 5,
    .alertRadius = 80.0f,
    .alertOffsetDist = 30.0f,
    .playerSearchInterval = 2,
    .chaseSpeed = 4.5f,
    .chaseTurnRate = 6,
    .chaseUpdateInterval = 1,
    .chaseRadius = 90.0f,
    .chaseOffsetDist = 30.0f,
    .unk_AI_2C = 1,
};

EvtScript N(EVS_NpcAI_BooBully) = {
    Call(SetSelfVar, 0, 0)
    Call(SetSelfVar, 5, -630)
    Call(SetSelfVar, 6, 50)
    Call(SetSelfVar, 1, 200)
    Call(N(FlyingAI_Main), Ref(N(AISettings_BooBully)))
    Return
    End
};

NpcSettings N(NpcSettings_BooBully) = {
    .height = 24,
    .radius = 24,
    // .ai = &N(EVS_NpcAI_BooBully),
    .onHit = &EnemyNpcHit,
    .onDefeat = &EnemyNpcDefeat,
    .level = ACTOR_LEVEL_BOO_BULLY,
};
