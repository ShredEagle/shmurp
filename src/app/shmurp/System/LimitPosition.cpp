#include "LimitPosition.h"

namespace ad {


LimitPosition::LimitPosition(aunteater::Engine &aEngine) :
    mPositionBounds{aEngine},
    mSceneGraphNodes{aEngine}
{}


void limitPosition(const Rectangle<> & aBoundingRect, Vec<2> & aPosition)
{
    if (!aBoundingRect.contains(aPosition.as<Position>()))
    {
        aPosition = aBoundingRect.closestPoint(aPosition.as<Position>()).as<Vec>();
    }
}


void LimitPosition::update(const aunteater::Timer aTimer)
{
    for (auto wrapPositionBound : mPositionBounds)
    {
        auto & [boundaries, geometry] = wrapPositionBound;

        // Similar unsatisfying branching appraoch than with Displace implementation:
        // We try to uniformly handle both SceneGraph entities, and "canonical" entities
        if (auto nodeWrap = mSceneGraphNodes.find(entityIdFrom(wrapPositionBound));
            nodeWrap != mSceneGraphNodes.end())
        {
            auto & [discard_1, sceneGraphComposite, discard_2] = nodeWrap;
            // TODO: this currently works only for SceneGraph roots
            // otherwise, the bounding rectangle has to be first transformed to local coordinates!
            limitPosition(boundaries.boundingRect, sceneGraphComposite.position);
        }
        else
        {
            limitPosition(boundaries.boundingRect, geometry.position);
        }
    }
}


} // namespace ad
