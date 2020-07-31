#pragma once

#include <aunteater/Component.h>

namespace ad {

struct Shape : public aunteater::Component<Shape>
{
    enum Value
    {
        Bullet,
        Laser,
        Circle,
        Pyramid,
        RocketShip,
        Square,
    };

    Shape(Value aEnumerator) :
        enumerator(aEnumerator)
    {}

    Value enumerator;
};



} // namespace ad

