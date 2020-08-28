#pragma once

#include "../Game.h"

#include <Components/Health.h>
#include <Components/ImpactRef.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {


using ImpactReplicator = aunteater::Archetype<Health, ImpactRef>;


class ForwardImpact : public aunteater::System
{
public:
    ForwardImpact(aunteater::Engine &aEngine,
                  EventQueue<shmurp::event::Impact> & aImpactEvents);

    void update(const aunteater::Timer aTimer) override;

private:
    const aunteater::FamilyHelp<ImpactReplicator> mImpactReplicators;
    EventQueue<shmurp::event::Impact> & mImpactEvents;
};


} // namespace ad
