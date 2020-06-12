#pragma once

#include "../commons.h"

#include "../transformations.h"

#include <aunteater/Component.h>


namespace ad {


struct Geometry : public aunteater::Component<Geometry>
{
    explicit Geometry(GLfloat aRadius) :
        radius(aRadius)
    {}

    explicit Geometry(Vec<2, GLfloat> aPosition = Vec<2>::Zero(), GLfloat aRadius = 0.) :
        radius(aRadius),
        position(aPosition)
    {}


    Geometry(GLfloat x, GLfloat y, GLfloat aRadius = 0.) :
        radius(aRadius),
        position(x, y)
    {}


    bool isColliding(const Geometry & aOther)
    {
        return (position-aOther.position).getNormSquared() <= std::pow(radius+aOther.radius, 2);
    }


    GLfloat radius;
    Vec<2> position{Vec<2>::Zero()};
    Vec<3, Radian<>> orientations{Vec<3, Radian<>>::Zero()};
};


} // namespace ad
