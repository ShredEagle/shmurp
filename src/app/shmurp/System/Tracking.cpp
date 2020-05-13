#include "Tracking.h"


namespace ad {


Tracking::Tracking(aunteater::Engine &aEngine) :
    mTrackers{aEngine},
    mTargets{aEngine}
{}


void Tracking::update(const aunteater::Timer aTimer)
{
    // Note:
    //   At the moment, hardcoded to track first target
    if (mTargets.size())
    {
        const auto targetPosition = mTargets.begin()->get<Geometry>().position;

        for(auto & [geometry, ignored] : mTrackers)
        {
            geometry.orientations.z() =
                transform::angleBetween({0.f, -1.f}, targetPosition-geometry.position).value();
        }
    }
}


} // namespace ad
