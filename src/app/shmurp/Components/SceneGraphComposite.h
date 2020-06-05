#pragma once

#include "../commons.h"

#include <aunteater/Component.h>

#include <vector>


namespace ad {


class SceneGraphComposite : public aunteater::Component<SceneGraphComposite>
{
    friend class SceneGraph;

public:
    enum RelationControl
    {
        InheritOrientation,
        ResetOrientation,
    };

    SceneGraphComposite(Vec<2> aPosition,
                        Vec<3, Radian<>> aOrientations=Vec<3, Radian<>>::Zero(),
                        RelationControl aRelation = InheritOrientation) :
        position{aPosition},
        orientations{aOrientations},
        relation{aRelation}
    {}

    Vec<2> position;
    Vec<3, Radian<>> orientations;
    RelationControl relation;

private:
    std::vector<aunteater::weak_entity> mChildren;
};



} // namespace ad
