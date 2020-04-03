#pragma once

#include <Components/Boundaries.h>
#include <Components/Geometry.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {


using PositionBound = aunteater::Archetype<Boundaries, Geometry>;


class LimitPosition : public aunteater::System
{
public:
    LimitPosition(aunteater::Engine &aEngine);

    void update(double aDelta) override;

private:
    const aunteater::FamilyHelp<PositionBound> mPositionBounds;
};


} // namespace ad
