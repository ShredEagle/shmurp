#pragma once

#include "../commons.h"

#include <aunteater/Component.h>


namespace ad {


struct Boundaries : public aunteater::Component<Boundaries>
{
    Boundaries(Rectangle<> aBoundingRect) :
        boundingRect(aBoundingRect)
    {}

    Rectangle<> boundingRect;
};


} // namespace ad
