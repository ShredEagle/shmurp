#pragma once

#include <aunteater/Component.h>


namespace ad {


/// \brief A parent set to nullptr indicates a root node of the scene graph
struct SceneGraphParent : public aunteater::Component<SceneGraphParent>
{
    SceneGraphParent() = default;

    SceneGraphParent(aunteater::weak_entity aParent) :
        parent{aParent}
    {}

    // Note: made const, because of the current logic to ensure the scene graph consistency:
    // the nodes are rooted when they enter the family (and removed when they live it), but no
    // check is made to see if the parent pointer stayed the same. We force it to stay the same.
    const aunteater::weak_entity parent{nullptr};
};



} // namespace ad
