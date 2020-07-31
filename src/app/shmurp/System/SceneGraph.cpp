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


void SceneGraph::traverse(aunteater::LiveEntity & aNode,
                          const Matrix<4> & aParentTransform,
                          const Vec<3, Radian<>> aParentOrientation)
{
    SceneGraphComposite & node = aNode.get<SceneGraphComposite>();
    Geometry & geometry = aNode.get<Geometry>();

    Vec<4> transPos{node.position.x(), node.position.y(), 0.f, 1.f};
    transPos *= aParentTransform;
    geometry.position = Vec<2>{transPos.x(), transPos.y()};

    geometry.orientations = node.orientations;
    auto forwardedOrientationMatrix = transform::makeOrientationMatrix(node.orientations);
    if (node.relation == SceneGraphComposite::InheritOrientation)
    {
        geometry.orientations += aParentOrientation;
    }
    else if (node.relation == SceneGraphComposite::ResetOrientation)
    {
        forwardedOrientationMatrix *= transform::makeOrientationMatrix(-aParentOrientation);
    }

    for (aunteater::weak_entity child : node.mChildren)
    {
        // IMPORTANT: the separation between local/world transformation has to be deeply refactored
        traverse(*child,
                 forwardedOrientationMatrix
                    * transform::translateMatrix(node.position.x(), node.position.y())
                    * aParentTransform,
                 geometry.orientations);
    }
}


void SceneGraph::update(const aunteater::Timer aTimer)
{
    for (auto & root : mRootNodes)
    {
        traverse(*root, Matrix<4>::Identity(), Vec<3, Radian<>>::Zero());
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


// A similar function exist in stdlib, starting with C++20
template <class T>
void eraseElement(std::vector<T> & aVector, const T & aValue)
{
    aVector.erase(std::remove(aVector.begin(), aVector.end(), aValue), aVector.end());
}


void SceneGraph::removedEntity(aunteater::LiveEntity &aEntity)
{
    // Remove the node from its parent
    const aunteater::weak_entity parent = aEntity.get<SceneGraphParent>().parent;
    if (parent == nullptr)
    {
        eraseElement(mRootNodes, entityRefFrom(aEntity));
    }
    else
    {
        auto foundParent = std::find(mNodesFamily.begin(), mNodesFamily.end(), parent);
        if (foundParent != mNodesFamily.end())
        {
            auto & children = (*foundParent)->get<SceneGraphComposite>().mChildren;
            eraseElement(children, entityRefFrom(aEntity));
        }
        // Else, the parent might have been removed first
        // (causing the current node to be recursively removed)
    }

    // Mark all children for removal
    // Note: this does not need to recusively traverse all the children:
    // removing a child will have removedEntity() called for said child, thus implicitly recurring
    for (aunteater::weak_entity child : aEntity.get<SceneGraphComposite>().mChildren)
    {
        mEngine.markToRemove(child);
    }
}


} // namespace ad
