/// @file mim_08.h
/// @brief Forever Forest - Laughing Rock

#include "common.h"
#include "generated.h"
#include "message_ids.h"
#include "map.h"

#include "../mim.h"
#include "mapfs/mim_08_shape.h"
#include "mapfs/mim_08_hit.h"

#include "sprite/player.h"
#include "sprite/npc/WorldBow.h"
#include "sprite/npc/Oaklie.h"
#include "sprite/npc/Duplighost.h"
#include "sprite/npc/BooBully.h"
#include "sprite/npc/Fuzzy.h"
#include "sprite/npc/SmallPiranha.h"
#include "sprite/npc/InsectsDK3.h"

namespace mim_08 {

enum {
    NPC_FreezyFuzzy,
    NPC_PiranhaPlant,
    NPC_PiranhaPlant_Hitbox,
    NPC_Beehive,
    NPC_Buzzbee,
    NPC_BooBully_1,
    NPC_BooBully_2,
    NPC_DupiOaklie,
    NPC_Duplighost,
    NPC_Oaklie,
};

enum {
    MV_ShakedTree           = MapVar(0),
    MV_PadlockEntityID      = MapVar(1),
};


extern EvtScript EVS_Main;
extern EvtScript EVS_SetupMusic;
extern EvtScript EVS_SetupGates;
extern EvtScript EVS_UnlockChest;
extern EvtScript EVS_MakeEntities;
extern NpcGroupList DefaultNPCs;
extern EvtScript EVS_DropShrinkStomp;
extern EvtScript EVS_UseGate_South;
extern EvtScript EVS_Cutscene2;
extern EvtScript EVS_Cutscene4;
extern EvtScript EVS_Cutscene5;
extern EvtScript EVS_Cutscene6;

}; // namespace mim_08
