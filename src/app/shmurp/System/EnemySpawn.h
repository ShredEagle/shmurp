#pragma once

#include <Utils/Periodic.h>

#include <Components/Geometry.h>
#include <Components/Speed.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>

namespace ad {

typedef aunteater::Archetype<Geometry, Speed> Prunable;

class EnemySpawn : public aunteater::System
{

public:
    EnemySpawn(aunteater::Engine &aEngine);

    void update(const aunteater::Timer aTimer) override;

private:
    aunteater::Engine & mEngine;
    aunteater::FamilyHelp<Prunable> mPrunables;
    Periodic mSpawnPeriod{0.1f};
};

} // namespace ad
