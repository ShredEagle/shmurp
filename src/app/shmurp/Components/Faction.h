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

    Faction(Bit aSide, short aTargets = 0) :
        side(aSide),
        targets(aTargets)
    {}

    bool isAttacking(Faction aOther) const
    {
        return aOther.side & targets;
    }

    Bit side{0};
    short targets{0};
};



} // namespace ad
