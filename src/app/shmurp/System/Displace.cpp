#include "Displace.h"


namespace ad {

Displace::Displace(aunteater::Engine &aEngine) :
    mMovables(aEngine)
{}

void Displace::update(const aunteater::Timer aTimer)
{
    const GLfloat delta = aTimer.delta();
    for (auto & [geometry, speed] : mMovables)
    {
        geometry.position += static_cast<GLfloat>(delta) * speed.translation;
        // Note: limitation in math::matrix, only accepting scalar multiplication with matching type
        geometry.orientations += (delta * speed.rotation);

    }
}

} // namespace ad
