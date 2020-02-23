#include "BulletSpawn.h"

#include "../configuration.h"
#include "../transformations.h"

#include <Components/Faction.h>
#include <Components/Shape.h>
#include <Components/Speed.h>

namespace ad {

BulletSpawn::BulletSpawn(aunteater::Engine &aEngine) :
    mEngine(aEngine),
    mCanons(aEngine)
{}


void spawnBullet(timet aRemainingTime, aunteater::Engine & aEngine, Vec<2, GLfloat> aBasePosition, FirePattern & aPattern)
{
    static constexpr Vec<4, GLfloat> gSpeed(0.f, -conf::gEnemyBulletSpeed, 0.f, 1.f);

    using aunteater::Entity;
    auto speed = gSpeed * transform::rotateMatrix(aPattern.nextAngle());
    Vec<2, GLfloat> startPosition = aBasePosition
                                    + static_cast<GLfloat>(aRemainingTime)*Vec<2, GLfloat>{speed.x(), speed.y()};
    aEngine.addEntity(Entity().add<Faction>(Faction::LibLies,
                                            Faction::SpaceForce)
                              .add<Geometry>(startPosition, conf::gBulletRadius)
                              .add<Shape>(Shape::Circle)
                              .add<Speed>(speed.x(), speed.y()));
}


void BulletSpawn::update(double aDelta)
{
    for (const auto & canon : mCanons)
    {
        canon->get<FirePattern>().period.forEachEvent(aDelta,
                                                      &spawnBullet,
                                                      mEngine,
                                                      canon->get<Geometry>().position,
                                                      canon->get<FirePattern>());
    }
}

} // namespace ad
