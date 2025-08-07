#include "sff_05.h"
#include "entity.h"

namespace sff_05 {

// EvtScript N(EVS_UseBluePipe) = {
//     Call(GotoMap, Ref("tik_09"), tik_09_ENTRY_2)
//     Wait(100)
//     Return
//     End
// };

EvtScript EVS_MakeEntities = {
    // IfEq(GF_MIM11_WarpPipe, false)
    //     IfEq(GF_TIK09_WarpPipe, true)
    //         Call(GetEntryID, LVar0)
    //         IfNe(LVar0, sff_05_ENTRY_3)
    //             Set(GF_KMR02_WarpPipe, true)
    //         EndIf
    //     EndIf
    // EndIf
    // Call(MakeEntity, Ref(Entity_BlueWarpPipe), -275, 0, -225, 270, sff_05_ENTRY_3, Ref(N(EVS_UseBluePipe)), EVT_INDEX_OF_GAME_FLAG(GF_MIM11_WarpPipe), MAKE_ENTITY_END)
    // Call(MakeEntity, Ref(Entity_HeartBlock), 0, 60, -300, 45, MAKE_ENTITY_END)
    // Call(MakeEntity, Ref(Entity_SavePoint), 220, 60, -80, 45, MAKE_ENTITY_END)
    // Call(MakeEntity, Ref(Entity_YellowBlock), -282, 60, 522, 0, ITEM_VOLT_SHROOM, MAKE_ENTITY_END)
    // Call(AssignBlockFlag, GF_MIM11_ItemBlock_VoltShroom)
    Return
    End
};

}; // namespace sff_05
