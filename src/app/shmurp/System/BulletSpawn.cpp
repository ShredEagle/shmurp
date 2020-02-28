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
    for (auto & [firePattern, geometry] : mCanons)
    {
        firePattern.fire(aDelta, mEngine, geometry.position);
    }
}

} // namespace ad
