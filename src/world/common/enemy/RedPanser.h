#include "common.h"
#include "sprite/npc/Panser.h"

#define RED_PANSER_DROPS \
{ \
    .dropFlags = NPC_DROP_FLAG_80, \
    .itemDropChance = 100, \
    .itemDrops = { \
        { ITEM_FIRE_FLOWER, 10, 0 }, \
    }, \
    .heartDrops  = STANDARD_HEART_DROPS(4), \
    .flowerDrops = STANDARD_FLOWER_DROPS(3), \
    .minCoinBonus = 0, \
    .maxCoinBonus = 3, \
}

#define RED_PANSER_ANIMS \
{ \
    .idle   = ANIM_Panser_Red_Idle, \
    .walk   = ANIM_Panser_Red_Idle, \
    .run    = ANIM_Panser_Red_Idle, \
    .chase  = ANIM_Panser_Red_Idle, \
    .anim_4 = ANIM_Panser_Red_Idle, \
    .anim_5 = ANIM_Panser_Red_Idle, \
    .death  = ANIM_Panser_Red_Hurt, \
    .hit    = ANIM_Panser_Red_Hurt, \
    .anim_8 = ANIM_Panser_Red_Idle, \
    .anim_9 = ANIM_Panser_Red_Shot, \
    .anim_A = ANIM_Panser_Red_Idle, \
    .anim_B = ANIM_Panser_Red_Idle, \
    .anim_C = ANIM_Panser_Red_Idle, \
    .anim_D = ANIM_Panser_Red_Idle, \
    .anim_E = ANIM_Panser_Red_Idle, \
    .anim_F = ANIM_Panser_Red_Idle, \
}

#define RED_PANSER_FIREBALL_HITBOX(npcID) \
{ \
    .id = npcID, \
    .settings = &N(NpcSettings_RedPanser_Fireball), \
    .pos = { NPC_DISPOSE_LOCATION }, \
    .yaw = 90, \
    .flags = ENEMY_FLAG_IGNORE_ENTITY_COLLISION | ENEMY_FLAG_FLYING | ENEMY_FLAG_NO_DROPS, \
    .drops = NO_DROPS, \
    .territory = { \
        .wander = { \
            .isFlying = TRUE, \
            .moveSpeedOverride = NO_OVERRIDE_MOVEMENT_SPEED, \
            .wanderShape = SHAPE_CYLINDER, \
            .centerPos  = { NPC_DISPOSE_LOCATION }, \
            .wanderSize = { 0 }, \
            .detectShape = SHAPE_CYLINDER, \
            .detectPos  = { NPC_DISPOSE_LOCATION }, \
            .detectSize = { 0 }, \
        } \
    }, \
    .animations = { \
        .idle   = ANIM_Panser_Red_Fireball, \
        .walk   = ANIM_Panser_Red_Fireball, \
        .run    = ANIM_Panser_Red_Fireball, \
        .chase  = ANIM_Panser_Red_Fireball, \
        .anim_4 = ANIM_Panser_Red_Fireball, \
        .anim_5 = ANIM_Panser_Red_Fireball, \
        .death  = ANIM_Panser_Red_Fireball, \
        .hit    = ANIM_Panser_Red_Fireball, \
        .anim_8 = ANIM_Panser_Red_Fireball, \
        .anim_9 = ANIM_Panser_Red_Fireball, \
        .anim_A = ANIM_Panser_Red_Fireball, \
        .anim_B = ANIM_Panser_Red_Fireball, \
        .anim_C = ANIM_Panser_Red_Fireball, \
        .anim_D = ANIM_Panser_Red_Fireball, \
        .anim_E = ANIM_Panser_Red_Fireball, \
        .anim_F = ANIM_Panser_Red_Fireball, \
    }, \
    .extraAnimations = N(ExtraAnims_RedPanser_Fireball), \
}
