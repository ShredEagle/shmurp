#pragma once

#include "../commons.h"

#include <aunteater/Component.h>

namespace ad {

struct Geometry : public aunteater::Component<Geometry>
{
    Geometry(Vec<2, GLfloat> aPosition, GLfloat aRadius = 0.) :
        position(aPosition),
        radius(aRadius)
    {}

    Geometry(GLfloat x, GLfloat y, GLfloat aRadius = 0.) :
        position(x, y),
        radius(aRadius)
    {}

    bool isColliding(const Geometry & aOther)
    {
        return (position-aOther.position).getNormSquared() <= std::pow(radius+aOther.radius, 2);
    }

    Vec<2, GLfloat> position;
    GLfloat radius;
    Matrix<4, GLfloat> orientation{Matrix<4, GLfloat>::Identity()};
};

} // namespace ad
