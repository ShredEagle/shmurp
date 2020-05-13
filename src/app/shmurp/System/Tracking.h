#pragma once

#include <Components/ControlDevice.h>
#include <Components/Geometry.h>
#include <Components/TrackerPlayer.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {


using Trackers = aunteater::Archetype<Geometry, TrackerPlayer>;
using Targets = aunteater::Archetype<ControlDevice>;


class Tracking : public aunteater::System
{

public:
    Tracking(aunteater::Engine &aEngine);

    void update(const aunteater::Timer aTimer) override;

private:
    const aunteater::FamilyHelp<Trackers> mTrackers;
    const aunteater::FamilyHelp<Targets> mTargets;
};


} // namespace ad
