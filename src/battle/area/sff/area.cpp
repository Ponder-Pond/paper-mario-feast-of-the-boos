#include "area.hpp"
#include "stage/sff_stages.hpp"

#include "battle/common/newactor/sff/sff_actors.hpp"

#include "battle/common/newactor/sff/freezy_fuzzy.inc.cpp"
#include "battle/common/newactor/sff/pink_panser.inc.cpp"
#include "battle/common/newactor/sff/red_panser.inc.cpp"
#include "battle/common/newactor/sff/piranha_plant.inc.cpp"
#include "battle/common/newactor/sff/boo_bully.inc.cpp"
#include "battle/common/newactor/sff/frost_piranha.inc.cpp"
#include "battle/common/newactor/sff/crystal_cleft.inc.cpp"
#include "battle/common/newactor/sff/purple_panser.inc.cpp"
#include "battle/common/newactor/sff/spirit_bubble.inc.cpp"
#include "battle/common/newactor/sff/duplighost.inc.cpp"
#include "battle/common/newactor/sff/beehive.inc.cpp"
#include "battle/common/newactor/sff/jr_troopa_gang.inc.cpp"

using namespace battle::actor;

namespace battle::area::sff {

Vec3i Beehive1Pos = { 85, 120, -25 };
Vec3i Beehive2Pos = { 120, 120, -25 };
Vec3i PiranhaPlantPos = { 42, 25, 0 };
Vec3i BooBullyPosB = { 45, 30, -5 };
Vec3i BooBullyPosC = { 85, 30, 10 };

// [BTL_POS_GROUND_A] { 5, 0, -20 },
// [BTL_POS_GROUND_B] { 45, 0, -5 },
// [BTL_POS_GROUND_C] { 85, 0, 10 },
// [BTL_POS_GROUND_D] { 125, 0, 25 },

Formation Formation_DuoFreezyFuzzy = {
    ACTOR_BY_IDX(FreezyFuzzy, BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(FreezyFuzzy, BTL_POS_GROUND_C, 9),
};

Formation Formation_SoloBooBully = {
    ACTOR_BY_POS(BooBully, BooBullyPosB, 10),
};

Formation Formation_SoloFreezyFuzzySoloPiranhaPlant = {
    ACTOR_BY_IDX(FreezyFuzzy, BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(PiranhaPlant, BTL_POS_GROUND_C, 9),
};

Formation Formation_SoloPiranhaPlantSoloPurplePanser = {
    ACTOR_BY_IDX(PiranhaPlant, BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(PurplePanser, BTL_POS_GROUND_C, 9),
};

Formation Formation_DuoFreezyFuzzySoloPurplePanser = {
    ACTOR_BY_IDX(FreezyFuzzy, BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(FreezyFuzzy, BTL_POS_GROUND_C, 9),
    ACTOR_BY_IDX(PurplePanser, BTL_POS_GROUND_D, 8),
};

Formation Formation_PurplePanserMiniboss = {
    ACTOR_BY_IDX(PurplePanser, BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(RedPanser, BTL_POS_GROUND_C, 9),
    ACTOR_BY_IDX(PurplePanser, BTL_POS_GROUND_D, 8),
};

Formation Formation_DuoPiranhaPlant = {
    ACTOR_BY_IDX(PiranhaPlant, BTL_POS_GROUND_B, 10),
    ACTOR_BY_IDX(PiranhaPlant, BTL_POS_GROUND_C, 9),
};

Formation Formation_BeehiveMiniboss = {
    ACTOR_BY_POS(Beehive, Beehive1Pos, 10),
    ACTOR_BY_POS(Beehive, Beehive2Pos, 9),
};

Formation Formation_DuplighostMiniboss = {
    ACTOR_BY_IDX(Duplighost, BTL_POS_GROUND_B, 10),
    ACTOR_BY_POS(BooBully, BooBullyPosC, 9),
};

BattleList Battles = {
    BATTLE(Formation_DuoFreezyFuzzy, SnowyForeverForest1, "2 Freezy Fuzzies"),
    BATTLE(Formation_SoloBooBully, SnowyForeverForest1, "Boo Bully"),
    BATTLE(Formation_SoloFreezyFuzzySoloPiranhaPlant, SnowyForeverForest1, "Freezy Fuzzy Piranha Plant"),
    BATTLE(Formation_SoloPiranhaPlantSoloPurplePanser, SnowyForeverForest1, "Piranha Plant Purple Panser"),
    BATTLE(Formation_DuoFreezyFuzzySoloPurplePanser, SnowyForeverForest1, "2 Freezy Fuzzies Purple Panser"),
    BATTLE(Formation_PurplePanserMiniboss, SnowyForeverForest1, "Purple Panser Miniboss"),
    BATTLE(Formation_DuoPiranhaPlant, SnowyForeverForest1, "2 Piranha Plants"),
    BATTLE(Formation_BeehiveMiniboss, SnowyForeverForest2, "Beehive Miniboss"),
    BATTLE(Formation_DuplighostMiniboss, SnowyForeverForest1, "Duplighost Miniboss"),
    {},
};

StageList Stages = {
    STAGE("Snowy Forever Forest 1", SnowyForeverForest1),
    STAGE("Snowy Forever Forest 2", SnowyForeverForest2),
    {},
};

}; // namespace battle::area::sff
