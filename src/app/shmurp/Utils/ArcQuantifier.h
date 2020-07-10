#pragma once


#include "../commons.h"


namespace ad {

struct ArcQuantifier
{
    ArcQuantifier(Radian<> aCoverage, int aCount) :
        startingAngle{aCount > 1 ? -aCoverage/2. : Radian<>{0}},
        angleIncrement{aCoverage / std::max(1, aCount-1)} // There are count-1 intervals
    {}


    Radian<> getAngleFor(int aIndex) const
    {
        return startingAngle + aIndex*angleIncrement;
    }


    Radian<> startingAngle;
    Radian<> angleIncrement;
};


} // namespace ad
