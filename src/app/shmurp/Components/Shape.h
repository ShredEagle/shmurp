#pragma once

#include <aunteater/Component.h>

namespace ad {

struct Shape : public aunteater::Component<Shape>
{
    enum Value
    {
        Bullet,
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

