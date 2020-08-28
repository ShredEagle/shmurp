#pragma once

#include "../commons.h"

#include <aunteater/Component.h>


namespace ad {


struct HealthFollower : public aunteater::Component<HealthFollower>
{
    HealthFollower(aunteater::entity_id aFollowed, Vec<2> aSize) :
        followed{aFollowed},
        size{std::move(aSize)}
    {}

    aunteater::entity_id followed;
    Vec<2> size;
};



} // namespace ad
