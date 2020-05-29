#include "Displace.h"

#include <Components/SceneGraphComposite.h>


namespace ad {


Displace::Displace(aunteater::Engine &aEngine) :
    mMovables(aEngine)
{}


template <class T_geometry>
void displace(T_geometry & aGeometry, const Speed & aSpeed, GLfloat aDelta)
{
    aGeometry.position += static_cast<GLfloat>(aDelta) * aSpeed.translation;
    aGeometry.orientations += aDelta * aSpeed.rotation;
}


void Displace::update(const aunteater::Timer aTimer)
{
    const GLfloat delta = aTimer.delta();
    //for (auto & [geometry, speed] : mMovables)
    //{
    //    geometry.position += static_cast<GLfloat>(delta) * speed.translation;
    //    geometry.orientations += delta * speed.rotation;
    //}

    for (aunteater::EntityWrap wrap : mMovables)
    {
        // Note: this is an unsatisfying branching approach, due to the fact that
        // the game supports both entities with only absolute geometry (which are "implicitly" root),
        // and entities with relative geometry (which are part of a scenegraph)
        auto & [geometry, speed] = wrap;
        if(wrap->has<SceneGraphComposite>())
        {
            displace(wrap->get<SceneGraphComposite>(), speed, delta);
        }
        else
        {
            displace(geometry, speed, delta);
        }
    }
}


} // namespace ad
