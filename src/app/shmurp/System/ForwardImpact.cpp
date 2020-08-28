#include "ForwardImpact.h"


namespace ad {


ForwardImpact::ForwardImpact(aunteater::Engine &aEngine,
                             EventQueue<shmurp::event::Impact> & aImpactEvents) :
    mImpactReplicators{aEngine},
    mImpactEvents{aImpactEvents}
{}


void ForwardImpact::update(const aunteater::Timer aTimer)
{
    for (auto & [health, impactRef] : mImpactReplicators)
    {
        for (auto & impact : mImpactEvents)
        {
            if (std::find(impactRef.sources.begin(), impactRef.sources.end(), impact.first)
                != impactRef.sources.end())
            {
                health.points -= impact.second.damage;
            }
        }
    }
}


} // namespace ad
