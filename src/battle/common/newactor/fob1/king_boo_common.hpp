#pragma once

namespace battle::actor {

enum KingBooCommon {
    ACTOR_KING_BOO              = ACTOR_ENEMY0,
    ACTOR_CLONE_1               = ACTOR_ENEMY1,
    ACTOR_CLONE_2               = ACTOR_ENEMY2,
    ACTOR_CLONE_3               = ACTOR_ENEMY3,
    // actor vars for king boo
    AVAR_KB_Phase               = 0,
    AVAL_KB_CheckPhase          = 0,
    AVAL_KB_FirstPhase          = 1,
    AVAL_KB_SecondPhase         = 2,
    AVAL_KB_ThirdPhase          = 3,
    AVAR_KB_ClonesDefeated      = 1,
    AVAL_KB_1CloneDead          = 1,
    AVAL_KB_2ClonesDead         = 2,
    AVAL_KB_3ClonesDead         = 3,
    AVAL_KB_AllClonesDefeated   = 0,
    // actor vars for clone
    AVAR_Clone_Type             = 0,
    AVAL_Type_None              = 0,
    AVAL_Type_1                 = 1,
    AVAL_Type_2                 = 2,
    AVAL_Type_3                 = 3,
};

}; // namespace battle::actor
