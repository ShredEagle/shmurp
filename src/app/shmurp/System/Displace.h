#pragma once

#include <Components/Geometry.h>
#include <Components/Speed.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>

namespace ad {

typedef aunteater::Archetype<Geometry, Speed> Movable;

class Displace : public aunteater::System
{

public:
    Displace(aunteater::Engine &aEngine);

    void update(const aunteater::Timer aTimer) override;

private:
    aunteater::FamilyHelp<Movable> mMovables;
};

} // namespace ad
