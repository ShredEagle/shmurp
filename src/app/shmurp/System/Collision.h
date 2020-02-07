#pragma once

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
    Collision(aunteater::Engine &aEngine);

    void update(double aDelta) override;

private:
    aunteater::Engine & mEngine;
    const aunteater::FamilyHelp<Colliding> mColliders;
};

} // namespace ad
