#include "sff_06.h"

namespace sff_06 {

#include "world/common/npc/Boo.inc.c"

EvtScript EVS_NpcInit_GateBoo_01 = {
    Return
    End
};

EvtScript EVS_NpcInit_GateBoo_02 = {
    Return
    End
};

NpcData NpcData_GateBoos[] = {
    {
        .id = NPC_GateBoo_01,
        .settings = &N(NpcSettings_Boo),
        .pos = { -68.0f, 65.0f, -56.0f },
        .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_FLYING,
        .init = &EVS_NpcInit_GateBoo_01,
        .yaw = 270,
        .drops = NO_DROPS,
        .animations = NORMAL_BOO_ANIMS,
    },
    {
        .id = NPC_GateBoo_02,
        .settings = &N(NpcSettings_Boo),
        .pos = { -125.0f, 65.0f, 60.0f },
        .flags = ENEMY_FLAG_PASSIVE | ENEMY_FLAG_FLYING,
        .init = &EVS_NpcInit_GateBoo_02,
        .yaw = 270,
        .drops = NO_DROPS,
        .animations = NORMAL_BOO_ANIMS,
    },
};

NpcGroupList DefaultNPCs = {
    NPC_GROUP(NpcData_GateBoos),
    {}
};

}; // namespace sff_06
