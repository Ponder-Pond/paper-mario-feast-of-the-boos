#include "sff_00.h"
#include "entity.h"

namespace sff_00 {

EvtScript EVS_MakeEntities = {
    Call(MakeEntity, Ref(Entity_SavePoint), -175, 60, -50, 0, MAKE_ENTITY_END)
    Return
    End
};

}; // namespace sff_00
