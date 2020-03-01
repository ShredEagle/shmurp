#include "Displace.h"

#include "../transformations.h"

namespace ad {

Displace::Displace(aunteater::Engine &aEngine) :
    mMovables(aEngine)
{}

void Displace::update(double aDelta)
{
    for (auto & [geometry, speed] : mMovables)
    {
        geometry.position += static_cast<GLfloat>(aDelta) * speed.translation;
        geometry.orientation *= transform::rotateMatrix_X(aDelta * speed.rotation.x())
                                * transform::rotateMatrix_Y(aDelta * speed.rotation.y());
    }
}

} // namespace ad
