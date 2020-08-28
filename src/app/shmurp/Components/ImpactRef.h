#pragma once

#include <aunteater/Component.h>

#include <vector>


namespace ad {


struct ImpactRef : public aunteater::Component<ImpactRef>
{
    ImpactRef(std::initializer_list<aunteater::entity_id> aSources) :
        sources{std::move(aSources)}
    {}

    /// \brief The entities whose impacts will be replicated
    std::vector<aunteater::entity_id> sources;
};



} // namespace ad
