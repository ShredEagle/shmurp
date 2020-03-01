#pragma once

#include "../commons.h"

#include <aunteater/Component.h>

namespace ad {

struct Speed : public aunteater::Component<Speed>
{
    Speed(Vec<2, GLfloat> aTranslation) :
        translation{aTranslation}
    {}

    Speed(Vec<2, GLfloat> aTranslation, Vec<3, GLfloat> aRotation) :
        translation{aTranslation},
        rotation{aRotation}
    {}

    Speed(GLfloat x, GLfloat y) :
        translation(x, y)
    {}

    Vec<2, GLfloat> translation;
    Vec<3, GLfloat> rotation{Vec<3, GLfloat>::Zero()};
};



} // namespace ad
