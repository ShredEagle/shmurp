#pragma once

#include <aunteater/Component.h>

namespace ad {

struct Faction : public aunteater::Component<Faction>
{
    enum Bit : short
    {
        SpaceForce = (1 << 0),
        Democrats  = (1 << 1),
        TruthBullet  = (1 << 2),
        LibLies  = (1 << 3),
    };

    Faction(Bit aSide, short aTargets = 0, int aAttackValue = 0) :
        side(aSide),
        targets(aTargets),
        attackValue(aAttackValue)
    {}

    bool isAttacking(Faction aOther) const
    {
        return aOther.side & targets;
    }

    Bit side{0};
    short targets{0};
    int attackValue{0};
};



} // namespace ad
