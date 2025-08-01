#include "mim_03.h"
#include "entity.h"

namespace mim_03 {

EvtScript EVS_CheckBlockade = {
    IfLt(GB_StoryProgress, STORY_MOD_CUTSCENE_5)
        Call(DisablePlayerInput, true)
        Call(ShowMessageAtScreenPos, MSG_FotB_0004, 160, 40)
        Call(DisablePlayerInput, false)
    Else
        Call(DisablePlayerInput, true)
        Call(ShowMessageAtScreenPos, MSG_FotB_0027, 160, 40)
        Call(DisablePlayerInput, false)
    EndIf
    Return
    End
};

EvtScript EVS_MakeEntities = {
    Call(MakeEntity, Ref(Entity_YellowBlock), GEN_ITEM_BOX_ITEM3_VEC, GEN_ITEM_BOX_ITEM3_DIR, ITEM_LIFE_SHROOM, MAKE_ENTITY_END)
    Call(AssignBlockFlag, GF_MIM03_ItemBlock_LifeShroom)
    BindTrigger(Ref(EVS_CheckBlockade),  TRIGGER_WALL_PRESS_A, COLLIDER_Rock1Col, 1, 0)
    Return
    End
};

}; // namespace mim_03
