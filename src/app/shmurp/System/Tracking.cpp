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

        for(auto & [geometry, sceneGraphComposite, ignored] : mTrackers)
        {
            // Note: this approaches introduces 1 step lag: we use the "previous frame" geometry and target positions
            //  This is hard to work around with a generic solution when a scene graph is involved,
            //  since there is a circular dependency:
            //  * orientation depends on absolute positions only known after traversing the scene graph
            //  * scene graph outputs absolute positions depending on orientations (which this system may change)
            //  This approach thus favors scene graph consistency at the cost of 1 step of lag

            // TODO: On problem is we set the relative orientation from the absolute angle...
            // I.e. This system will only work for root nodes
            sceneGraphComposite.orientations.z() =
                transform::angleBetween({1.f, 0.f}, targetPosition-geometry.position);
        }
    }
}


} // namespace ad
