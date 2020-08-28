#pragma once

#include "Game.h"

#include <Components/Faction.h>
#include <Components/Geometry.h>

#include <aunteater/Archetype.h>
#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>

namespace ad {

typedef aunteater::Archetype<Faction, Geometry> Colliding;

class Collision : public aunteater::System
{

public:
    Collision(aunteater::Engine &aEngine,
              EventQueue<shmurp::event::Impact> & aImpactEvents);

    void update(const aunteater::Timer aTimer) override;
    void handleCollision(aunteater::weak_entity aDefender, const Faction & aFaction);

private:
    aunteater::Engine & mEngine;
    EventQueue<shmurp::event::Impact> & mImpactEvents;
    const aunteater::FamilyHelp<Colliding> mColliders;
};

} // namespace ad
