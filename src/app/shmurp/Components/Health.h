#pragma once

#include <aunteater/Component.h>


namespace ad {


struct Health : public aunteater::Component<Health>
{
    Health(int aPoints) :
        points{aPoints},
        total{aPoints}
    {}

    int points;
    const int total;
};



} // namespace ad
