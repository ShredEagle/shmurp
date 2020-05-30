#pragma once

#include <Utils/Periodic.h>

#include <aunteater/System.h>


namespace ad {


class EnemySpawn : public aunteater::System
{
public:
    EnemySpawn(aunteater::Engine &aEngine);

    void update(const aunteater::Timer aTimer) override;

private:
    aunteater::Engine & mEngine;
    Periodic mSpawnPeriod{0.1f};
};


} // namespace ad
