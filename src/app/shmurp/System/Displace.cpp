#include "Displace.h"

#include "../transformations.h"

namespace ad {

Displace::Displace(aunteater::Engine &aEngine) :
    mMovables(aEngine)
{}

void Displace::update(const aunteater::Timer aTimer)
{
    const auto delta = aTimer.delta();
    for (auto & [geometry, speed] : mMovables)
    {
        geometry.position += static_cast<GLfloat>(delta) * speed.translation;
        geometry.orientation *= transform::rotateMatrix_X(delta * speed.rotation.x())
                                * transform::rotateMatrix_Y(delta * speed.rotation.y());
    }
}

} // namespace ad
