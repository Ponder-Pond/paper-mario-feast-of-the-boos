#pragma once

#include "common.h"

enum {
    AB_SFF_Unused_0     = AreaByte(0),
    AB_SFF_1            = AreaByte(1),
    AB_SFF_2            = AreaByte(2),
    AB_SFF_Unused_3     = AreaByte(3),
    AB_SFF_Unused_4     = AreaByte(4),
    AB_SFF_Unused_5     = AreaByte(5),
    AB_SFF_Unused_6     = AreaByte(6),
    AB_SFF_Unused_7     = AreaByte(7),
    AB_SFF_Unused_8     = AreaByte(8),
    AB_SFF_Unused_9     = AreaByte(9),
    AB_SFF_Unused_A     = AreaByte(10),
    AB_SFF_Unused_B     = AreaByte(11),
    AB_SFF_Unused_C     = AreaByte(12),
    AB_SFF_Unused_D     = AreaByte(13),
    AB_SFF_Unused_E     = AreaByte(14),
    AB_SFF_Unused_F     = AreaByte(15),
};

enum {
    AF_SFF_01                       = AreaFlag(1),
    AF_SFF02_Oaklie_DialogueToggle  = AreaFlag(2),
    AF_SFF06_BackgroundLitUp        = AreaFlag(3),
    AF_SFF_Snowing                  = AreaFlag(4),
    AF_SFF_LastSnowing              = AreaFlag(5),
};

EXTERN_C MapSettings sff_00_settings;
EXTERN_C MapSettings sff_01_settings;
EXTERN_C MapSettings sff_02_settings;
EXTERN_C MapSettings sff_03_settings;
EXTERN_C MapSettings sff_04_settings;
EXTERN_C MapSettings sff_05_settings;
EXTERN_C MapSettings sff_06_settings;
EXTERN_C MapSettings sff_07_settings;
