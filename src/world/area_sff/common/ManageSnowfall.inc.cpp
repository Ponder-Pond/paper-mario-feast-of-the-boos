#include "common.h"
#include "../sff.h"
#include "effects.h"

EvtScript EVS_ManageSnowfall = {
    Set(AF_SFF_Snowing, true)
    Set(AF_SFF_LastSnowing, false)
    Label(10)
        IfNe(AF_SFF_Snowing, AF_SFF_LastSnowing)
            IfEq(AF_SFF_Snowing, false)
                Call(RemoveEffect, LVarF)
            Else
                PlayEffect(EFFECT_SNOWFALL, 0, 50)
            EndIf
            Set(AF_SFF_LastSnowing, AF_SFF_Snowing)
        EndIf
        Wait(1)
        Goto(10)
    Return
    End
};

EvtScript EVS_ManageMansionSnowfall = {
    Set(AF_SFF_Snowing, true)
    Set(AF_SFF_LastSnowing, false)
    Label(10)
        IfNe(AF_SFF_Snowing, AF_SFF_LastSnowing)
            IfEq(AF_SFF_Snowing, false)
                Call(RemoveEffect, LVarF)
            Else
                PlayEffect(EFFECT_SNOWFALL, 0, 100)
            EndIf
            Set(AF_SFF_LastSnowing, AF_SFF_Snowing)
        EndIf
        Wait(1)
        Goto(10)
    Return
    End
};
