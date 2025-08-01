/// @file mim_10.h
/// @brief Forever Forest - Exit to Toad Town

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../mim.h"
#include "mapfs/mim_10_shape.h"
#include "mapfs/mim_10_hit.h"

#include "sprite/npc/Bootler.h"
#include "sprite/npc/JrTroopa.h"

namespace mim_10 {

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

}; // namespace mim_10
