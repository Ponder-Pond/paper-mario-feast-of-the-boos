/// @file mim_12.h
/// @brief Forever Forest - Exit to Gusty Gulch

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../mim.h"
#include "mapfs/mim_12_shape.h"
#include "mapfs/mim_12_hit.h"

#include "sprite/npc/Boo.h"
#include "sprite/npc/WorldBow.h"

namespace mim_12 {

enum {
    NPC_GateBoo_01      = 0,
    NPC_GateBoo_02      = 1,
};

extern EvtScript EVS_Main;
extern EvtScript EVS_StartForestMusic;
extern EvtScript EVS_StartGustyMusic;
extern EvtScript EVS_SetupMusic;
extern EvtScript EVS_SetupForestGate;
extern EvtScript EVS_MakeEntities;
extern NpcGroupList DefaultNPCs;

};
