#pragma once

#include <aunteater/Component.h>


namespace ad {


struct Health : public aunteater::Component<Health>
{
    Health(int aPoints) :
        points{aPoints}
    {}

    int points{0};
};



} // namespace ad
