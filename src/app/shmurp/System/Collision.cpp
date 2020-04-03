#include "Collision.h"

namespace ad {

Collision::Collision(aunteater::Engine &aEngine) :
    mEngine(aEngine),
    mSpaceForces(mEngine),
    mDemocrats(mEngine),
    mLibLies(mEngine),
    mTruthBullets(mEngine)
{}

void Collision::update(double aDelta)
{
    for(auto truthBullet : mTruthBullets)
    {
        for(auto democrat : mDemocrats)
        {
            if(truthBullet->get<Geometry>().isColliding(democrat->get<Geometry>()))
            {
                democrat->get<HitPoints>().hp -= 1;

                if (democrat->get<HitPoints>().hp == 0)
                {
                  mEngine.markToRemove(democrat);
                }

                mEngine.markToRemove(truthBullet);
            }
        }
    }
}

} // namespace ad
