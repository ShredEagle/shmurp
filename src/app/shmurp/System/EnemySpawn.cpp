#include "EnemySpawn.h"

#include "../configuration.h"

#include <Components/Faction.h>
#include <Components/Shape.h>

#include <handy/random.h>

namespace ad {

void spawn(aunteater::Engine & aEngine)
{
    static Randomizer randomX(0, conf::gWindowWorldWidth);

    using aunteater::Entity;

    aEngine.addEntity(Entity().add<Faction>(Faction::Democrats)
                              .add<Geometry>(randomX(),
                                             conf::gWindowWorldHeight + conf::gViewportOffset,
                                             conf::squareRadius)
                              .add<Shape>(Shape::Square)
                              .add<Speed>(0.f, -5.f));
}

EnemySpawn::EnemySpawn(aunteater::Engine &aEngine) :
    mEngine(aEngine),
    mPrunables(aEngine)
{}

void EnemySpawn::update(double time)
{
    for(auto count = mSpawnPeriod.countEvents(time); count; --count)
    {
        spawn(mEngine);
    }

    for (const auto candidate : mPrunables)
    {
        if (candidate->get<Geometry>().position.y() < -conf::gViewportOffset
            || candidate->get<Geometry>().position.y() > conf::gWindowWorldHeight+conf::gViewportOffset)
        {
            mEngine.markToRemove(candidate);
        }
    }
}

} // namespace ad
