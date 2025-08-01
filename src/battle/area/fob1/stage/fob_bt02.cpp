#include "../area.hpp"
// #include "entity.h"
// #include "battle/common/stage/area_mim/mim_01.inc.c"


namespace battle::area::fob1 {

namespace fob_02 {

#include "mapfs/mim_bt02_shape.h"

#include "battle/common/stage/lib/Snowflakes.inc.c"

API_CALLABLE(SetupFog) {
    enable_world_fog();
    set_world_fog_dist(950, 1010);
    set_world_fog_color(75, 120, 170, 255);
    gCameras[CAM_BATTLE].bgColor[0] = 60;
    gCameras[CAM_BATTLE].bgColor[1] = 70;
    gCameras[CAM_BATTLE].bgColor[2] = 90;

    return ApiStatus_DONE2;
}

EvtScript EVS_PreBattle = {
    Call(SetSpriteShading, SHADING_NONE)
    Call(SetupFog)
    Exec(N(EVS_SpawnSnowfall))
    Return
    End
};

EvtScript EVS_PostBattle = {
    Return
    End
};

}; // namespace fob_02

Stage SnowyForeverForest2 = {
    .texture = "mim_tex",
    .shape = "mim_bt02_shape",
    .hit = "mim_bt02_hit",
    .preBattle = &fob_02::EVS_PreBattle,
    .postBattle = &fob_02::EVS_PostBattle,
    .bg = "fob_bg",
};

}; // namespace battle::area::fob1
