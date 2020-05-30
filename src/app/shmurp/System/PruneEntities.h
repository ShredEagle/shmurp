#pragma once

#include <Components/Geometry.h>
#include <Components/Speed.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {


using Prunable = aunteater::Archetype<Geometry, Speed>;


class PruneEntities : public aunteater::System
{
public:
    PruneEntities(aunteater::Engine &aEngine);

    void update(const aunteater::Timer aTimer) override;

private:
    aunteater::Engine & mEngine;
    aunteater::FamilyHelp<Prunable> mPrunables;
};


} // namespace ad
