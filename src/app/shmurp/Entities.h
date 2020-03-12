#pragma once

#include "configuration.h"

#include <Components/Faction.h>
#include <Components/Geometry.h>
#include <Components/Shape.h>
#include <Components/Speed.h>
#include <Components/HitPoints.h>

#include <aunteater/Entity.h>

namespace ad {
namespace entities {

inline aunteater::Entity makeEnemyBullet(Vec<2, GLfloat> aPosition, Vec<4, GLfloat> aSpeed)
{
    return aunteater::Entity().add<Faction>(Faction::LibLies, Faction::SpaceForce)
                              .add<Geometry>(aPosition, conf::gBulletRadius)
                              .add<Shape>(Shape::Circle)
                              .add<HitPoints>(1)
                              .add<Speed>(aSpeed.x(), aSpeed.y());
}

inline aunteater::Entity makeFriendBullet(Vec<2, GLfloat> aPosition, Vec<4, GLfloat> aSpeed)
{
    return aunteater::Entity().add<Faction>(Faction::TruthBullet, Faction::Democrats)
                              .add<Geometry>(aPosition, conf::gBulletRadius)
                              .add<Shape>(Shape::Circle)
                              .add<HitPoints>(1)
                              .add<Speed>(aSpeed.x(), aSpeed.y());
}

inline aunteater::Entity makeSquare(Vec<2, GLfloat> aPosition,
                                    Vec<2, GLfloat> aTranslationSpeed,
                                    Vec<3, GLfloat> aRotationSpeed)
{
    return aunteater::Entity().add<Faction>(Faction::Democrats, 0)
                              .add<Geometry>(aPosition, conf::squareRadius)
                              .add<Shape>(Shape::Square)
                              .add<HitPoints>(10)
                              .add<Speed>(aTranslationSpeed, aRotationSpeed); 
}

} // namespace entities
} // namespace ad
