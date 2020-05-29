#pragma once

#include <Components/ControlDevice.h>
#include <Components/Geometry.h>
#include <Components/SceneGraphComposite.h>
#include <Components/TrackerPlayer.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {


using Trackers = aunteater::Archetype<Geometry, SceneGraphComposite, TrackerPlayer>;
using Targets = aunteater::Archetype<ControlDevice, Geometry>;


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
