#include "EnemySpawn.h"

#include "../configuration.h"

#include <Components/Faction.h>
#include <Components/Geometry.h>
#include <Components/Shape.h>
#include <Components/Speed.h>

#include <handy/random.h>

namespace ad {

typedef aunteater::Archetype<Geometry, Shape, Speed> Enemy;

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
    mEnemies(aEngine.getFamily<Enemy>())
{}

void EnemySpawn::update(double time)
{
    mTimeSinceLastSpawn += time;

    while((mTimeSinceLastSpawn - mSpawnPeriod) >= 0.)
    {
        mTimeSinceLastSpawn -= mSpawnPeriod;
        spawn(mEngine);
    }

    for (const auto enemy : mEnemies)
    {
        if (enemy->get<Geometry>().position.y() < -conf::gViewportOffset)
        {
            mEngine.markToRemove(enemy);
        }
    }
}

} // namespace ad
