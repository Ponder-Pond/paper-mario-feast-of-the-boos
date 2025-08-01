#include "common.h"
#include "sprite/npc/InsectsDK3.h"

#define BEEHIVE_DROPS \
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

#define BEEHIVE_ANIMS \
{ \
    .idle   = ANIM_InsectsDK3_Beehive, \
    .walk   = ANIM_InsectsDK3_Beehive, \
    .run    = ANIM_InsectsDK3_Beehive, \
    .chase  = ANIM_InsectsDK3_Beehive, \
    .anim_4 = ANIM_InsectsDK3_Beehive, \
    .anim_5 = ANIM_InsectsDK3_Beehive, \
    .death  = ANIM_InsectsDK3_BeehiveSpawn, \
    .hit    = ANIM_InsectsDK3_BeehiveSpawn, \
    .anim_8 = ANIM_InsectsDK3_Beehive, \
    .anim_9 = ANIM_InsectsDK3_Beehive, \
    .anim_A = ANIM_InsectsDK3_Beehive, \
    .anim_B = ANIM_InsectsDK3_Beehive, \
    .anim_C = ANIM_InsectsDK3_Beehive, \
    .anim_D = ANIM_InsectsDK3_Beehive, \
    .anim_E = ANIM_InsectsDK3_Beehive, \
    .anim_F = ANIM_InsectsDK3_Beehive, \
}

#define BUZZBEE_ANIMS \
{ \
    .idle   = ANIM_InsectsDK3_Buzzbee, \
    .walk   = ANIM_InsectsDK3_Buzzbee, \
    .run    = ANIM_InsectsDK3_Buzzbee, \
    .chase  = ANIM_InsectsDK3_Buzzbee, \
    .anim_4 = ANIM_InsectsDK3_Buzzbee, \
    .anim_5 = ANIM_InsectsDK3_Buzzbee, \
    .death  = ANIM_InsectsDK3_BuzzbeeSpawn, \
    .hit    = ANIM_InsectsDK3_BuzzbeeSpawn, \
    .anim_8 = ANIM_InsectsDK3_Buzzbee, \
    .anim_9 = ANIM_InsectsDK3_Buzzbee, \
    .anim_A = ANIM_InsectsDK3_Buzzbee, \
    .anim_B = ANIM_InsectsDK3_Buzzbee, \
    .anim_C = ANIM_InsectsDK3_Buzzbee, \
    .anim_D = ANIM_InsectsDK3_Buzzbee, \
    .anim_E = ANIM_InsectsDK3_Buzzbee, \
    .anim_F = ANIM_InsectsDK3_Buzzbee, \
}
