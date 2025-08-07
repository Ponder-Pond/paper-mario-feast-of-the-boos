#include "sff_05.h"
#include "foliage.hpp"

namespace sff_05 {

// #define BUSH1_VEC 357,16,315

DEFINE_BUSH_WITH_DROP(Bush1, MODEL_o182, GEN_BUSH1_VEC, ITEM_STRANGE_LEAF, ITEM_SPAWN_MODE_TOSS_NEVER_VANISH, AF_JAN01_TreeDrop_StarPiece, AF_JAN01_TreeDrop_StarPiece)
DEFINE_BUSH(Bush2, MODEL_o183, GEN_BUSH2_VEC)
DEFINE_BUSH(Bush3, MODEL_o184, GEN_BUSH3_VEC)
DEFINE_BUSH(Bush4, MODEL_o185, GEN_BUSH4_VEC)

EvtScript EVS_SetupFoliage = {
    BIND_BUSH(Bush1, COLLIDER_o207)
    BIND_BUSH(Bush2, COLLIDER_o208)
    BIND_BUSH(Bush3, COLLIDER_o209)
    BIND_BUSH(Bush4, COLLIDER_o210)
    Return
    End
};

}; // namespace sff_05
