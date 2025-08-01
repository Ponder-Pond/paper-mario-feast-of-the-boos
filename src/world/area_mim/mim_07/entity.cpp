#include "mim_07.h"
#include "entity.h"

namespace mim_07 {

#include "world/common/entity/Chest.inc.c"

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

EvtScript EVS_OpenChest_BooBullyFormation = {
    Call(DisablePlayerInput, true)
    Set(GF_MIM07_Chest_BooBullyFormation, true)
    Exec(EVS_BooBullyChestScene)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_OpenChest = EVT_OPEN_CHEST(ITEM_SUPER_SHROOM, GF_MIM07_Chest_SuperShroom);


EvtScript EVS_ReadSign = {
    Call(IsStartingConversation, LVar0)
    IfEq(LVar0, true)
        Return
    EndIf
    SetGroup(EVT_GROUP_NEVER_PAUSE)
    Call(SetTimeFreezeMode, TIME_FREEZE_PARTIAL)
    Call(DisablePlayerInput, true)
    Call(ShowMessageAtScreenPos, MSG_Menus_017F, 160, 40)
    Call(DisablePlayerInput, false)
    Call(SetTimeFreezeMode, TIME_FREEZE_NONE)
    Return
    End
};

EvtScript EVS_MakeEntities = {
    Call(MakeEntity, Ref(Entity_Chest), GEN_CHEST_FORMATION2_VEC, GEN_CHEST_FORMATION2_DIR, 0, MAKE_ENTITY_END)
    Call(AssignChestFlag, GF_MIM07_Chest_BooBullyFormation)
    Call(AssignScript, Ref(EVS_OpenChest_BooBullyFormation))
    Call(MakeEntity, Ref(Entity_Chest), GEN_CHEST_ITEM1_VEC, GEN_CHEST_ITEM1_DIR, 0, MAKE_ENTITY_END)
    Call(AssignChestFlag, GF_MIM07_Chest_SuperShroom)
    Call(AssignScript, Ref(EVS_OpenChest))
    Call(MakeEntity, Ref(Entity_Signpost), 280, 0, 0, 90, MAKE_ENTITY_END)
    Call(AssignScript, Ref(EVS_ReadSign))
    BindTrigger(Ref(EVS_CheckBlockade),  TRIGGER_WALL_PRESS_A, COLLIDER_Rock1Col, 1, 0)
    BindTrigger(Ref(EVS_CheckBlockade),  TRIGGER_WALL_PRESS_A, COLLIDER_Rock2Col, 1, 0)
    Return
    End
};

}; // namespace mim_07
