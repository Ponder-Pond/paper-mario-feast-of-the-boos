/// @file mim_11.h
/// @brief Forever Forest - Outside Boo's Mansion

#include "common.h"
#include "generated.h"
#include "message_ids.h"
#include "map.h"

#include "../mim.h"
#include "mapfs/mim_11_shape.h"
#include "mapfs/mim_11_hit.h"

#include "sprite/npc/WorldGoombario.h"
#include "sprite/npc/WorldKooper.h"
#include "sprite/npc/WorldBombette.h"
#include "sprite/npc/WorldParakarry.h"
#include "sprite/npc/Bootler.h"
#include "sprite/npc/WorldSkolar.h"

namespace mim_11 {

enum {
    NPC_Bootler         = 0,
    NPC_Skolar          = 2,
};

enum {
    AF_JAN01_TreeDrop_StarPiece           = MapFlag(10),
};

extern EvtScript EVS_Main;
extern EvtScript EVS_MakeEntities;
// extern EvtScript N(D_802430E0_BBA150);
extern EvtScript EVS_SetupFoliage;
extern EvtScript EVS_SetupMusic;

extern NpcGroupList DefaultNPCs;

}; // namespace mim_11
