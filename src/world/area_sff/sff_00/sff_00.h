/// @file sff_00.h
/// @brief Snowy Forever Forest - Exit to Toad Town

#include "common.h"
#include "generated.h"
#include "message_ids.h"
#include "map.h"

#include "../sff.h"
#include "mapfs/mim_10_shape.h"
#include "mapfs/mim_10_hit.h"

#include "sprite/npc/Bootler.h"
#include "sprite/npc/JrTroopa.h"

namespace sff_00 {

enum {
    NPC_Bootler         = 0,
    NPC_JrTroopa        = 1,
};

enum {
    MV_Unk_00           = MapVar(0),
};

extern EvtScript EVS_Main;
extern EvtScript EVS_SetupBootlerTrigger;
extern EvtScript EVS_SetupMusic;
extern EvtScript EVS_MakeEntities;
extern NpcGroupList DefaultNPCs;

}; // namespace sff_00
