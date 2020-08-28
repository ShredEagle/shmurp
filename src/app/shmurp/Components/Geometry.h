#pragma once

#include "../commons.h"

#include "../transformations.h"

#include <aunteater/Component.h>


namespace ad {


struct Geometry : public aunteater::Component<Geometry>
{
    explicit Geometry(GLfloat aRadius = 0.,
                      Vec<2, GLfloat> aPosition = Vec<2>::Zero(),
                      Vec<3, Radian<>> aOrientations = Vec<3, Radian<>>::Zero()) :
        radius{aRadius},
        orientations{std::move(aOrientations)},
        position{std::move(aPosition)}
    {}


    bool isColliding(const Geometry & aOther)
    {
        return (position-aOther.position).getNormSquared() <= std::pow(radius+aOther.radius, 2);
    }


    GLfloat radius;
    Vec<2> position{Vec<2>::Zero()};
    Vec<3, Radian<>> orientations{Vec<3, Radian<>>::Zero()};
    Vec<3> scale{1.f, 1.f, 1.f};
};


} // namespace ad
