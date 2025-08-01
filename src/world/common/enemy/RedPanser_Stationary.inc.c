#include "RedPanser.h"

NpcSettings N(NpcSettings_RedPanser_Stationary) = {
    .height = 24,
    .radius = 28,
    .onHit = &EnemyNpcHit,
    .onDefeat = &EnemyNpcDefeat,
    .level = ACTOR_LEVEL_RED_PANSER,
    .actionFlags = AI_ACTION_JUMP_WHEN_SEE_PLAYER,
};
