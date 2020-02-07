#pragma once

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
    const float mSpawnPeriod{0.2};
    double mTimeSinceLastSpawn{0.};
};

} // namespace ad
