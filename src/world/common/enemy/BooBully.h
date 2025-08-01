#include "common.h"
#include "sprite/npc/BooBully.h"

#define BOO_BULLY_DROPS \
{ \
    .dropFlags = NPC_DROP_FLAG_80, \
    .itemDropChance = 15, \
    .itemDrops = { \
        { ITEM_MAPLE_SYRUP, 6, 0 }, \
        { ITEM_HONEY_SYRUP, 4, 0 }, \
    }, \
    .heartDrops  = STANDARD_HEART_DROPS(3), \
    .flowerDrops = STANDARD_FLOWER_DROPS(2), \
    .minCoinBonus = 1, \
    .maxCoinBonus = 4, \
}

#define BOO_BULLY_ANIMS \
{ \
    .idle   = ANIM_BooBully_Idle, \
    .walk   = ANIM_BooBully_Walk, \
    .run    = ANIM_BooBully_Run, \
    .chase  = ANIM_BooBully_Run, \
    .anim_4 = ANIM_BooBully_Idle, \
    .anim_5 = ANIM_BooBully_Idle, \
    .death  = ANIM_BooBully_Flail, \
    .hit    = ANIM_BooBully_Flail, \
    .anim_8 = ANIM_BooBully_Run, \
    .anim_9 = ANIM_BooBully_Run, \
    .anim_A = ANIM_BooBully_Run, \
    .anim_B = ANIM_BooBully_Run, \
    .anim_C = ANIM_BooBully_Run, \
    .anim_D = ANIM_BooBully_Run, \
    .anim_E = ANIM_BooBully_Run, \
    .anim_F = ANIM_BooBully_Run, \
}
