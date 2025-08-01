#include "common.h"
#include "../mim.h"
#include "effects.h"

EvtScript EVS_ManageSnowfall = {
    Set(AF_MIM_Snowing, true)
    Set(AF_MIM_LastSnowing, false)
    Label(10)
        IfNe(AF_MIM_Snowing, AF_MIM_LastSnowing)
            IfEq(AF_MIM_Snowing, false)
                Call(RemoveEffect, LVarF)
            Else
                PlayEffect(EFFECT_SNOWFALL, 0, 50)
            EndIf
            Set(AF_MIM_LastSnowing, AF_MIM_Snowing)
        EndIf
        Wait(1)
        Goto(10)
    Return
    End
};

EvtScript EVS_ManageMansionSnowfall = {
    Set(AF_MIM_Snowing, true)
    Set(AF_MIM_LastSnowing, false)
    Label(10)
        IfNe(AF_MIM_Snowing, AF_MIM_LastSnowing)
            IfEq(AF_MIM_Snowing, false)
                Call(RemoveEffect, LVarF)
            Else
                PlayEffect(EFFECT_SNOWFALL, 0, 100)
            EndIf
            Set(AF_MIM_LastSnowing, AF_MIM_Snowing)
        EndIf
        Wait(1)
        Goto(10)
    Return
    End
};
