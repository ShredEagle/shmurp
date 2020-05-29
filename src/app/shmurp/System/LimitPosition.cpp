#include "LimitPosition.h"

namespace ad {


LimitPosition::LimitPosition(aunteater::Engine &aEngine) :
    mPositionBounds(aEngine)
{}


void LimitPosition::update(const aunteater::Timer aTimer)
{
    for (auto & [boundaries, geometry] : mPositionBounds)
    {
        if (!boundaries.boundingRect.contains(geometry.position.as<Position>()))
        {
            geometry.position =
                boundaries.boundingRect.closestPoint(geometry.position.as<Position>())
                    .as<Vec>();
        }
    }
}


} // namespace ad
