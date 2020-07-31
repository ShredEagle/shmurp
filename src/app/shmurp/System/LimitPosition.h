#pragma once

#include "SceneGraph.h"

#include <Components/Boundaries.h>
#include <Components/Geometry.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {


using PositionBound = aunteater::Archetype<Boundaries, Geometry>;


class LimitPosition : public aunteater::System
{
public:
    LimitPosition(aunteater::Engine &aEngine);

    void update(const aunteater::Timer aTimer) override;

private:
    const aunteater::FamilyHelp<PositionBound> mPositionBounds;
    const aunteater::FamilyHelp<SceneGraphNode> mSceneGraphNodes;
};


} // namespace ad
