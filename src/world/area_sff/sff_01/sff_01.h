/// @file sff_01.h
/// @brief Snowy Forever Forest - Flower Sounds

#include "common.h"
#include "generated.h"
#include "message_ids.h"
#include "map.h"

#include "../sff.h"
#include "mapfs/mim_01_shape.h"
#include "mapfs/mim_01_hit.h"

#include "sprite/npc/WorldBow.h"
#include "sprite/npc/Fuzzy.h"

namespace sff_01 {

enum {
    NPC_FreezyFuzzy                = 0,
};

extern EvtScript EVS_Main;
extern EvtScript EVS_SetupMusic;
extern EvtScript EVS_MakeEntities;
extern EvtScript EVS_SetupExitHint;
extern EvtScript EVS_SetupGates;
extern EvtScript EVS_BindExitTriggers;
extern NpcGroupList DefaultNPCs;
extern EvtScript EVS_UseGate_West;
extern EvtScript EVS_DisabledGate_West;
extern EvtScript EVS_Cutscene1;

}; // namespace sff_01
