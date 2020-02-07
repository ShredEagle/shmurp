#pragma once

#include "../commons.h"

#include <aunteater/Component.h>

namespace ad {

struct Speed : public aunteater::Component<Speed>
{
    Speed(Vec<2, GLfloat> aSpeed) :
        speed(aSpeed)
    {}

    Speed(GLfloat x, GLfloat y) :
        speed(x, y)
    {}

    Vec<2, GLfloat> speed;
};



} // namespace ad
