#include "BulletSpawn.h"

#include "../configuration.h"
#include "../transformations.h"

namespace ad {

BulletSpawn::BulletSpawn(aunteater::Engine &aEngine) :
    mEngine(aEngine),
    mCanons(aEngine)
{}


void BulletSpawn::update(const aunteater::Timer aTimer)
{
    for (auto & [firePattern, geometry] : mCanons)
    {
        firePattern.fire(aTimer.delta(), mEngine, geometry.position, geometry.orientations);
    }
}

} // namespace ad
