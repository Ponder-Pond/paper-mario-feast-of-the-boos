#include "../area.hpp"
#include "battle/battle.h"

#include "mapfs/sff_bt01_shape.h"
// #include "entity.h"
// #include "battle/common/stage/area_mim/mim_01.inc.c"

namespace battle::area::sff {

namespace sff_01 {

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

}; // namespace sff_01

Stage SnowyForeverForest1 = {
    .texture = "sff_tex",
    .shape = "sff_bt01_shape",
    .hit = "sff_bt01_hit",
    .preBattle = &sff_01::EVS_PreBattle,
    .postBattle = &sff_01::EVS_PostBattle,
    .bg = "sff_bg",
};

}; // namespace battle::area::sff
