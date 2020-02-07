#include "Displace.h"

#include <Components/Geometry.h>
#include <Components/Speed.h>

namespace ad {

typedef aunteater::Archetype<Geometry, Speed> Movable;

Displace::Displace(aunteater::Engine &aEngine) :
    mMovables(aEngine.getFamily<Movable>())
{}

void Displace::update(double aDelta)
{
    for (auto & movable : mMovables)
    {
        movable->get<Geometry>().position +=
            static_cast<GLfloat>(aDelta) * movable->get<Speed>().speed;
    }
}

} // namespace ad
