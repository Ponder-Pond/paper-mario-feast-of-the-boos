
#include "mim_01.h"
#include "entity.h"

EvtScript EVS_ReadSign = {
    Call(IsStartingConversation, LVar0)
    IfEq(LVar0, true)
        Return
    EndIf
    SetGroup(EVT_GROUP_NEVER_PAUSE)
    Call(SetTimeFreezeMode, TIME_FREEZE_PARTIAL)
    Call(DisablePlayerInput, true)
    Call(ShowMessageAtScreenPos, MSG_Menus_017E, 160, 40)
    Call(DisablePlayerInput, false)
    Call(SetTimeFreezeMode, TIME_FREEZE_NONE)
    Return
    End
};

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
    Call(MakeEntity, Ref(Entity_Signpost), -300, 0, 0, 270, MAKE_ENTITY_END)
    Call(AssignScript, Ref(EVS_ReadSign))
    BindTrigger(Ref(EVS_CheckBlockade),  TRIGGER_WALL_PRESS_A, COLLIDER_Rock1Col, 1, 0)
    BindTrigger(Ref(EVS_CheckBlockade),  TRIGGER_WALL_PRESS_A, COLLIDER_Rock2Col, 1, 0)
    Return
    End
};
