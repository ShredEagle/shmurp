#pragma once

#include <aunteater/Component.h>

namespace ad {

struct Shape : public aunteater::Component<Shape>
{
    enum Value
    {
        Circle,
        RocketShip,
        Square,
        FilledSquare,
    };

    Shape(Value aEnumerator) :
        enumerator(aEnumerator)
    {}

    Value enumerator;
};



} // namespace ad

