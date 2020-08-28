#pragma once

#include "Game.h"

#include <Components/Faction.h>
#include <Components/Geometry.h>
#include <Components/Health.h>
#include <Components/HealthFollower.h>
#include <Components/ImpactRef.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {


using Impactable = aunteater::Archetype<Faction, Health>;
using ImpactReplicator = aunteater::Archetype<Health, ImpactRef>;
using HealthBar = aunteater::Archetype<Geometry, HealthFollower>;



class Damage : public aunteater::System
{
public:
    Damage(aunteater::Engine &aEngine,
           EventQueue<shmurp::event::Impact> & aImpactEvents);

    void update(const aunteater::Timer aTimer) override;

private:
    void updateDirectImpact();
    void updateForwardImpact();
    void updateHealthBars();

    void handleDamage(Health & aHealth, int aDamage, aunteater::weak_entity aImpacted);

    aunteater::Engine & mEngine;
    const aunteater::FamilyHelp<Impactable> mImpactables;
    const aunteater::FamilyHelp<ImpactReplicator> mImpactReplicators;
    const aunteater::FamilyHelp<HealthBar> mHealthBars;
    EventQueue<shmurp::event::Impact> & mImpactEvents;
};


} // namespace ad
