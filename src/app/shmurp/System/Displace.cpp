#include "Displace.h"

#include <Components/Position.h>
#include <Components/Speed.h>

namespace ad {

typedef aunteater::Archetype<Position, Speed> Movable;

Displace::Displace(aunteater::Engine &aEngine) :
    mMovables(aEngine.getFamily<Movable>())
{}

void Displace::update(double aDelta)
{
    for (auto & movable : mMovables)
    {
        movable->get<Position>().position +=
            static_cast<GLfloat>(aDelta) * movable->get<Speed>().speed;
    }
}

} // namespace ad
