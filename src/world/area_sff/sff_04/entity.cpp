#include "sff_04.h"
#include "entity.h"

namespace sff_04 {

#include "world/common/entity/Chest.inc.c"
#include "world/common/todo/RemovePadlock.inc.c"
#include "world/common/todo/GetEntityPosition.inc.c"

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

EvtScript EVS_DropShrinkStomp = {
    Call(DisablePlayerInput, true)
    Loop(12)
        Set(LVar0, 2)
        Call(TranslateModel, MODEL_Beehive1, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_Beehive2, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_o443, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_o442, LVar0, 0, LVar0)
        Wait(1)
        Set(LVar0, -2)
        Call(TranslateModel, MODEL_Beehive1, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_Beehive2, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_o443, LVar0, 0, LVar0)
        Call(TranslateModel, MODEL_o442, LVar0, 0, LVar0)
        Wait(1)
    EndLoop
    Set(LVar0, 0)
    Call(TranslateModel, MODEL_Beehive1, LVar0, 0, LVar0)
    Call(TranslateModel, MODEL_Beehive2, LVar0, 0, LVar0)
    Call(TranslateModel, MODEL_o443, LVar0, 0, LVar0)
    Call(TranslateModel, MODEL_o442, LVar0, 0, LVar0)
    Wait(1)
    Call(GetPlayerPos, LVar0, LVar1, LVar2)
    Sub(LVar0, 20)
    Sub(LVar2, 40)
    Call(DropItemEntity, ITEM_SHRINK_STOMP, LVar0, LVar1, LVar2, ITEM_SPAWN_MODE_FALL_NEVER_VANISH, GF_MIM08_Badge_ShrinkStomp)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_OpenChest_Oaklie = {
    Call(DisablePlayerInput, true)
    Set(GF_MIM08_Chest_Oaklie, true)
    Set(GB_StoryProgress, STORY_MOD_CUTSCENE_6)
    Exec(EVS_Cutscene6)
    Call(DisablePlayerInput, false)
    Return
    End
};

EvtScript EVS_UnlockChest = {
    SetGroup(EVT_GROUP_NEVER_PAUSE)
    SuspendGroup(EVT_GROUP_FLAG_INTERACT)
    Call(ShowKeyChoicePopup)
    IfEq(LVar0, 0)
        Call(ShowMessageAtScreenPos, MSG_Menus_00D8, 160, 40)
        Call(CloseChoicePopup)
        ResumeGroup(EVT_GROUP_FLAG_INTERACT)
        Return
    EndIf
    IfEq(LVar0, -1)
        Call(CloseChoicePopup)
        ResumeGroup(EVT_GROUP_FLAG_INTERACT)
        Return
    EndIf
    Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_ChestLockCollider, COLLIDER_FLAGS_UPPER_MASK)
    Call(RemoveItem, ITEM_RUINS_KEY)
    Call(CloseChoicePopup)
    Set(GF_MIM08_UnlockedChest, true)
    Call(N(GetEntityPosition), MV_PadlockEntityID, LVar0, LVar1, LVar2)
    Call(PlaySoundAt, SOUND_USE_KEY, SOUND_SPACE_DEFAULT, LVar0, LVar1, LVar2)
    Set(LVar0, MV_PadlockEntityID)
    Call(N(RemovePadlock))
    ResumeGroup(EVT_GROUP_FLAG_INTERACT)
    Unbind
    Return
    End
};

EvtScript EVS_OpenLockedChest = {
    BindTrigger(Ref(EVS_OpenChest_Oaklie), TRIGGER_WALL_PRESS_A, COLLIDER_ChestLockCollider, 1, 0)
    Return
    End
};

EvtScript EVS_MakeEntities = {
    Call(MakeItemEntity, ITEM_MAPLE_SYRUP, GEN_ITEM2_VEC, ITEM_SPAWN_MODE_FIXED_NEVER_VANISH, GF_MIM08_Item_MapleSyrup)
    IfEq(GF_MIM08_UnlockedChest, false)
        Call(MakeEntity, Ref(Entity_Padlock), -74, 33, -25, 90, MAKE_ENTITY_END)
        Call(AssignScript, Ref(EVS_OpenLockedChest))
        Set(MV_PadlockEntityID, LVar0)
    Else
        Call(ModifyColliderFlags, MODIFY_COLLIDER_FLAGS_SET_BITS, COLLIDER_ChestLockCollider, COLLIDER_FLAGS_UPPER_MASK)
    EndIf
    Call(MakeEntity, Ref(Entity_Chest), GEN_CHEST_OAKLIE_VEC, GEN_CHEST_OAKLIE_DIR, 0, MAKE_ENTITY_END)
    Call(AssignChestFlag, GF_MIM08_Chest_Oaklie)
    Call(AssignScript, Ref(EVS_OpenChest_Oaklie))
    Call(MakeEntity, Ref(Entity_HeartBlock), GEN_HP_BLOCK_VEC, GEN_HP_BLOCK_DIR, MAKE_ENTITY_END)
    Call(MakeEntity, Ref(Entity_SavePoint), GEN_SAVE_BLOCK_VEC, GEN_SAVE_BLOCK_DIR, MAKE_ENTITY_END)
    BindTrigger(Ref(EVS_CheckBlockade),  TRIGGER_WALL_PRESS_A, COLLIDER_Rock1Col, 1, 0)
    BindTrigger(Ref(EVS_CheckBlockade),  TRIGGER_WALL_PRESS_A, COLLIDER_Rock2Col, 1, 0)
    BindTrigger(Ref(EVS_CheckBlockade),  TRIGGER_WALL_PRESS_A, COLLIDER_Rock3Col, 1, 0)
    Return
    End
};

}; // namespace sff_04
