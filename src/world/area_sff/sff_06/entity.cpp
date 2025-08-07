#include "sff_06.h"
#include "entity.h"

namespace sff_06 {

EvtScript EVS_MakeEntities = {
    Call(MakeEntity, Ref(Entity_HiddenPanel), 100, 0, -40, 0, MODEL_o8, MAKE_ENTITY_END)
    Call(AssignPanelFlag, GF_MIM12_HiddenPanel)
    Return
    End
};

}; // namespace mim_12
