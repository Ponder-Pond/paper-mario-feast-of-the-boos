#include "common.h"
#include "sprite/npc/Fuzzy.h"

#define FREEZY_FUZZY_DROPS \
{ \
    .dropFlags = NPC_DROP_FLAG_80, \
    .itemDropChance = 5, \
    .itemDrops = { \
        { ITEM_THUNDER_BOLT, 10, 0 }, \
    }, \
    .heartDrops  = STANDARD_HEART_DROPS(2), \
    .flowerDrops = STANDARD_FLOWER_DROPS(2), \
    .minCoinBonus = 0, \
    .maxCoinBonus = 2, \
}

#define FREEZY_FUZZY_ANIMS \
{ \
    .idle   = ANIM_Fuzzy_Freezy_Walk, \
    .walk   = ANIM_Fuzzy_Freezy_Walk, \
    .run    = ANIM_Fuzzy_Freezy_Run, \
    .chase  = ANIM_Fuzzy_Freezy_Run, \
    .anim_4 = ANIM_Fuzzy_Freezy_Idle, \
    .anim_5 = ANIM_Fuzzy_Freezy_Idle, \
    .death  = ANIM_Fuzzy_Freezy_Hurt, \
    .hit    = ANIM_Fuzzy_Freezy_Hurt, \
    .anim_8 = ANIM_Fuzzy_Freezy_Run, \
    .anim_9 = ANIM_Fuzzy_Freezy_Run, \
    .anim_A = ANIM_Fuzzy_Freezy_Run, \
    .anim_B = ANIM_Fuzzy_Freezy_Run, \
    .anim_C = ANIM_Fuzzy_Freezy_Run, \
    .anim_D = ANIM_Fuzzy_Freezy_Run, \
    .anim_E = ANIM_Fuzzy_Freezy_Run, \
    .anim_F = ANIM_Fuzzy_Freezy_Run, \
}
