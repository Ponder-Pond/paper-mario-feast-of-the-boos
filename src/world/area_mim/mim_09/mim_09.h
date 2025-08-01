/// @file mim_09.h
/// @brief Forever Forest - Flowers Appear (FP Plus)

#include "common.h"
#include "message_ids.h"
#include "map.h"

#include "../mim.h"
#include "mapfs/mim_09_shape.h"
#include "mapfs/mim_09_hit.h"

enum {
    NPC_Fuzzy           = 0,
};

enum {
    MV_FoundHint        = MapVar(0),
};

#define NAMESPACE mim_09

extern EvtScript ::EVS_Main;
extern EvtScript EVS_SetupGates;
extern EvtScript EVS_SetupMusic;
extern EvtScript EVS_SetupExitHint;
extern EvtScript EVS_MakeEntities;
extern NpcGroupList DefaultNPCs;
