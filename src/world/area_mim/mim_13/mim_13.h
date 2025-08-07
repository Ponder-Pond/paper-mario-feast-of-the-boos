/// @file mim_13.h
/// @brief Forever Forest - Courtyard

#include "common.h"
#include "generated.h"
#include "message_ids.h"
#include "map.h"

#include "../mim.h"
#include "mapfs/mim_13_shape.h"
#include "mapfs/mim_13_hit.h"

// #include "sprite/npc/Bootler.h"
// #include "sprite/npc/JrTroopa.h"

namespace sff_07 {

// enum {
//     NPC_Bootler         = 0,
//     NPC_JrTroopa        = 1,
// };

enum {
    MV_Unk_00           = MapVar(0),
};

extern EvtScript EVS_Main;
extern EvtScript EVS_SetupMusic;
extern EvtScript EVS_MakeEntities;
extern NpcGroupList DefaultNPCs;

}; // namespace sff_07
