#pragma once

#include "configuration.h"

#include <Components/Faction.h>
#include <Components/Geometry.h>
#include <Components/Shape.h>
#include <Components/Speed.h>

#include <aunteater/Entity.h>


namespace ad {
namespace entities {


inline aunteater::Entity makeBullet(Faction aFaction,
                                    Vec<2, GLfloat> aPosition,
                                    Vec<4, GLfloat> aSpeed,
                                    Floating aRadius)
{
    return aunteater::Entity().add<Faction>(aFaction)
                              .add<Geometry>(aPosition, aRadius)
                              .add<Shape>(Shape::Bullet)
                              .add<Speed>(aSpeed.x(), aSpeed.y());
}


} // namespace entities
} // namespace ad
