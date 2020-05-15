#pragma once

#include "../commons.h"

#include <aunteater/Component.h>

#include <vector>


namespace ad {


class SceneGraphComposite : public aunteater::Component<SceneGraphComposite>
{
    friend class SceneGraph;

public:
    SceneGraphComposite(Vec<2> aPosition) :
        position{aPosition.x(), aPosition.y(), 0.f, 1.f}
    {}

    Vec<4> position;
    Vec<3, Radian<>> orientations{Vec<3, Radian<>>::Zero()};

private:
    std::vector<aunteater::weak_entity> mChildren;
};



} // namespace ad
