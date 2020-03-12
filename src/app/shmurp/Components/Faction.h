#pragma once

#include <aunteater/Component.h>

#include <iostream>
namespace ad {

struct Faction : public aunteater::Component<Faction>
{
    enum Bit : unsigned short
    {
        SpaceForce = (1 << 0),
        Democrats  = (1 << 1),
        TruthBullet  = (1 << 2),
        LibLies  = (1 << 3),
    };

    Faction(Bit aSide, unsigned short aTargets = 0) :
        side(aSide),
        targets(aTargets)
    {}

    bool isAttacking(Faction aOther) const
    {
        //std::cerr << __func__ << " "<< aOther.side << " " << targets << "\n";

        return aOther.side & targets;
    }

    Bit side{0};
    unsigned short targets{0};
};



} // namespace ad
