#pragma once

#include <aunteater/Component.h>

namespace ad {

struct Shape : public aunteater::Component<Shape>
{
    enum Value
    {
        RocketShip,
        Square,
    };

    Shape(Value aEnumerator) :
        enumerator(aEnumerator)
    {}

    Value enumerator;
};



} // namespace ad

