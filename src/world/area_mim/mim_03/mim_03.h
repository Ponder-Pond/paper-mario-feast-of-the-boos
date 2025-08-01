/// @file mim_03.h
/// @brief Forever Forest - Flowers (Oaklie)

#include "common.h"
#include "generated.h"
#include "message_ids.h"
#include "map.h"

#include "../mim.h"
#include "mapfs/mim_03_shape.h"
#include "mapfs/mim_03_hit.h"

#include "sprite/player.h"
#include "sprite/npc/WorldBow.h"
#include "sprite/npc/Oaklie.h"
#include "sprite/npc/Fuzzy.h"
#include "sprite/npc/Panser.h"
#include "sprite/npc/SmallPiranha.h"

namespace mim_03 {

enum {
    NPC_FreezyFuzzy,
    NPC_PiranhaPlant,
    NPC_PiranhaPlant_Hitbox,
    NPC_RedPanser,
    // NPC_RedPanser_Fireball,
    NPC_DupiOaklie,
    NPC_Oaklie,
    // NPC_HammerBros,
};

extern EvtScript EVS_Main;
extern EvtScript EVS_MakeEntities;
extern EvtScript EVS_SetupExitHint;
extern EvtScript EVS_SetupGates;
extern EvtScript EVS_SetupMusic;
extern NpcGroupList DefaultNPCs;
extern EvtScript EVS_UseGate_North;
extern EvtScript EVS_Cutscene3;
extern EvtScript EVS_Cutscene7;

}; // namespace mim_03
