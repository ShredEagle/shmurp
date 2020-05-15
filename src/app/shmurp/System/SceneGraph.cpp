#include "SceneGraph.h"

#include "../transformations.h"


namespace ad {


SceneGraph::SceneGraph(aunteater::Engine &aEngine) :
    mEngine{aEngine},
    mNodesFamily{aEngine.getFamily<SceneGraphNode>()}
{
    // TODO aunteater should make it safer to observe (already captured in AE todo list)
    mNodesFamily.registerObserver(this);
}


SceneGraph::~SceneGraph()
{
    mNodesFamily.cancelObserver(this);
}


void SceneGraph::traverse(aunteater::LiveEntity & aNode, const Matrix<4> & aTransform)
{
    SceneGraphComposite & node = aNode.get<SceneGraphComposite>();
    Geometry & geometry = aNode.get<Geometry>();

    Vec<4> transPos = node.position * aTransform;
    geometry.position = Vec<2>{transPos.x(), transPos.y()};

    for (aunteater::weak_entity child : aNode.get<SceneGraphComposite>().mChildren)
    {
        // IMPORTANT: should be node.orientations instead, but the separation between local/world
        // transformation has to be deeply refactored
        traverse(*child, transform::makeOrientationMatrix(geometry.orientations)
                         * transform::translateMatrix(node.position.x(), node.position.y())
                         * aTransform);
    }
}


void SceneGraph::update(const aunteater::Timer aTimer)
{
    for (auto & root : mRootNodes)
    {
        traverse(*root, Matrix<4>::Identity());
    }
}


void SceneGraph::addedEntity(aunteater::LiveEntity &aEntity)
{
    const aunteater::weak_entity parent = aEntity.get<SceneGraphParent>().parent;
    if (parent == nullptr)
    {
        mRootNodes.push_back(entityRefFrom(aEntity));
    }
    else
    {
        auto found = std::find(mNodesFamily.begin(), mNodesFamily.end(), parent);
        if (found != mNodesFamily.end())
        {
            (*found)->get<SceneGraphComposite>().mChildren.push_back(entityRefFrom(aEntity));
        }
        else
        {
            throw std::logic_error("scene graph node parent is not a scene graph node");
        }
    }
}


void SceneGraph::removedEntity(aunteater::LiveEntity &aEntity)
{
    // Remove the node from its parent
    const aunteater::weak_entity parent = aEntity.get<SceneGraphParent>().parent;
    if (parent == nullptr)
    {
        std::remove(mRootNodes.begin(), mRootNodes.end(), entityRefFrom(aEntity));
    }
    else
    {
        auto found = std::find(mNodesFamily.begin(), mNodesFamily.end(), parent);
        if (found != mNodesFamily.end())
        {
            auto & children = (*found)->get<SceneGraphComposite>().mChildren;
            std::remove(children.begin(), children.end(), entityRefFrom(aEntity));
        }
        else
        {
            throw std::logic_error("scene graph node parent is not a scene graph node");
        }
    }

    // Recursively mark all children for removal
    auto markRemove = [this](aunteater::LiveEntity & entity, auto & recursion) -> void
        {
            for (aunteater::weak_entity child : entity.get<SceneGraphComposite>().mChildren)
            {
                mEngine.markToRemove(child);
                recursion(*child, recursion);
            }
        };
    markRemove(aEntity, markRemove);
}


} // namespace ad
