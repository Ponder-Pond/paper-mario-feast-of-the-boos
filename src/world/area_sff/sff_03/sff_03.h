/// @file sff_03.h
/// @brief Snowy Forever Forest - Flowers Vanish

#include "common.h"
#include "generated.h"
#include "message_ids.h"
#include "map.h"

#include "../sff.h"
#include "mapfs/mim_07_shape.h"
#include "mapfs/mim_07_hit.h"

#include "sprite/player.h"
#include "sprite/npc/JrTroopa.h"
#include "sprite/npc/WorldGoombario.h"
#include "sprite/npc/WorldKooper.h"
#include "sprite/npc/WorldBombette.h"
#include "sprite/npc/WorldParakarry.h"
#include "sprite/npc/WorldBow.h"
#include "sprite/npc/Fuzzy.h"

namespace sff_03 {

enum {
    NPC_BooBully,
};

extern EvtScript EVS_Main;
extern EvtScript EVS_PlayForestMusic;
extern EvtScript EVS_JrTroopaMusic;
extern EvtScript EVS_SetupGates;
extern EvtScript EVS_SetupExitHint;
extern EvtScript EVS_MakeEntities;
extern NpcGroupList DefaultNPCs;
extern EvtScript EVS_BooBullyChestScene;

}; // namespace sff_03
