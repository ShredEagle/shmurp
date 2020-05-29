#pragma once

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>

#include <Components/Geometry.h>
#include <Components/SceneGraphComposite.h>
#include <Components/SceneGraphParent.h>


namespace ad {


using SceneGraphNode = aunteater::Archetype<Geometry, SceneGraphComposite, SceneGraphParent>;


class SceneGraph : public aunteater::System, public aunteater::FamilyObserver
{
public:
    SceneGraph(aunteater::Engine &aEngine);

    ~SceneGraph();

    // Non-copyable, non-movable due to registration as FamilyObserver
    SceneGraph(const SceneGraph &) = delete;
    SceneGraph & operator=(const SceneGraph &) = delete;

    void update(const aunteater::Timer aTimer) override;


    //
    // FamilyObserver
    //
    void addedEntity(aunteater::LiveEntity &aEntity) override;
    void removedEntity(aunteater::LiveEntity &aEntity) override;

private:
    void traverse(aunteater::LiveEntity & aNode,
                  const Matrix<4> & aTransform,
                  const Vec<3, Radian<>> aParentOrientation);

private:
    aunteater::Engine & mEngine;
    aunteater::Family & mNodesFamily;

    std::vector<aunteater::weak_entity> mRootNodes;
};


} // namespace ad
