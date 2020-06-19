#pragma once

#include "configuration.h"

#include <Components/Faction.h>
#include <Components/Geometry.h>
#include <Components/Shape.h>
#include <Components/Speed.h>

#include <aunteater/Entity.h>


namespace ad {


// Might make a good component, then fire() virtual method would also take a reference to this component
// Yet, it would either mean a component whose member is a component (here faction)
// Or we could use the faction of the composed entity, but then the bullet would always have the same faction as the entity
struct BulletConfig
{
    Faction faction{Faction::LibLies, Faction::SpaceForce};
    Floating radius{conf::gBulletRadius};
    Floating velocity{conf::gEnemyBulletSpeed};
    Shape shape{Shape::Bullet};
};


namespace entities {


inline aunteater::Entity makeBullet(const Vec<2, GLfloat> aPosition,
                                    const Vec<3, Radian<>> aOrientations,
                                    const Vec<4, GLfloat> aSpeed,
                                    const BulletConfig & aConfig)
{
    return aunteater::Entity().add<Faction>(aConfig.faction)
                              .add<Geometry>(aConfig.radius, aPosition, aOrientations)
                              .add<Shape>(aConfig.shape)
                              .add<Speed>(aSpeed.x(), aSpeed.y());
}

inline aunteater::Entity makeBullet(const Vec<2, GLfloat> aPosition,
                                    const Radian<> aDirection,
                                    const Vec<4, GLfloat> aSpeed,
                                    const BulletConfig & aConfig)
{
    using namespace math::angle_literals;
    return makeBullet(aPosition, {0._radf, 0._radf, aDirection}, aSpeed, aConfig);
}

} // namespace entities
} // namespace ad
