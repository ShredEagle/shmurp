#include "EnemySpawn.h"

#include "../configuration.h"

#include <Components/Position.h>
#include <Components/Shape.h>
#include <Components/Speed.h>

#include <handy/random.h>

namespace ad {

typedef aunteater::Archetype<Position, Shape, Speed> Enemy;

void spawn(aunteater::Engine & aEngine)
{
    static Randomizer randomX(0, conf::gWindowWorldWidth);

    using aunteater::Entity;

    aEngine.addEntity(Entity().add<Position>(randomX(),
                                             conf::gWindowWorldHeight + conf::gViewportOffset)
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

    // TODO Cannot use auto for loop atm, removal invalidate the current iterator (so ++it fails)
    for (auto it = mEnemies.begin(); it != mEnemies.end();)
    {
        auto enemy = *(it++);
        if (enemy->get<Position>().position.y() < -conf::gViewportOffset)
        {
            mEngine.removeEntity(enemy);
        }
    }
}

} // namespace ad
