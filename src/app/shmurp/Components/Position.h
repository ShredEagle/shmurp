#pragma once

#include "../commons.h"

#include <aunteater/Component.h>

namespace ad {

struct Position : public aunteater::Component<Position>
{
    Position(GLfloat x, GLfloat y) :
        position(x, y)
    {}

    Vec<2, GLfloat> position;
};

} // namespace ad
