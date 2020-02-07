#pragma once

#include <Utils/Periodic.h>

#include <aunteater/System.h>

namespace ad {

class EnemySpawn : public aunteater::System
{

public:
    EnemySpawn(aunteater::Engine &aEngine);

    void update(double aDelta) override;

private:
    aunteater::Engine & mEngine;
    const aunteater::Family & mEnemies;
    Periodic mSpawnPeriod{0.1f};
};

} // namespace ad
