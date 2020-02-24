#include "BulletSpawn.h"

#include "../configuration.h"
#include "../transformations.h"

namespace ad {

BulletSpawn::BulletSpawn(aunteater::Engine &aEngine) :
    mEngine(aEngine),
    mCanons(aEngine)
{}


void BulletSpawn::update(double aDelta)
{
    for (const auto & canon : mCanons)
    {
        canon->get<FirePattern>().fire(aDelta, mEngine, canon->get<Geometry>().position);
    }
}

} // namespace ad
