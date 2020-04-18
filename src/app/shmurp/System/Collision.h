#pragma once

#include <Components/Faction.h>
#include <Components/Geometry.h>
#include <Components/HitPoints.h>

#include <aunteater/Archetype.h>
#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>

namespace ad {

typedef aunteater::Archetype<FactionSpaceForce, Geometry, HitPoints> SpaceForce;
typedef aunteater::Archetype<FactionDemocrats, Geometry, HitPoints> Democrats;
typedef aunteater::Archetype<FactionLibLies, Geometry> LibLies;
typedef aunteater::Archetype<FactionTruthBullet, Geometry> TruthBullet;

class Collision : public aunteater::System
{

public:
    Collision(aunteater::Engine &aEngine);

    void update(double aDelta) override;

private:
    aunteater::Engine & mEngine;
    const aunteater::FamilyHelp<SpaceForce> mSpaceForces;
    const aunteater::FamilyHelp<Democrats> mDemocrats;
    const aunteater::FamilyHelp<LibLies> mLibLies;
    const aunteater::FamilyHelp<TruthBullet> mTruthBullets;
};

} // namespace ad
