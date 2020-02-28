#pragma once

#include <Components/FirePattern.h>
#include <Components/Geometry.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>

namespace ad {

using Canon = aunteater::Archetype<FirePattern, Geometry>;

class BulletSpawn : public aunteater::System
{

public:
    BulletSpawn(aunteater::Engine &aEngine);

    void update(double aDelta) override;

private:
    aunteater::Engine & mEngine;
    aunteater::FamilyHelp<Canon> mCanons;
};

} // namespace ad
