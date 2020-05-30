#include "PruneEntities.h"

#include "../configuration.h"

#include <math/Rectangle.h>


namespace ad {


PruneEntities::PruneEntities(aunteater::Engine &aEngine) :
    mEngine(aEngine),
    mPrunables(aEngine)
{}


void PruneEntities::update(const aunteater::Timer aTimer)
{
    for (const auto candidate : mPrunables)
    {
        static const Rectangle<GLfloat> gGameArea{
            {-conf::gViewportOffset, -conf::gViewportOffset},
            {conf::gWindowWorldWidth+2*conf::gViewportOffset, conf::gWindowWorldHeight+2*conf::gViewportOffset}};

        if (!gGameArea.contains(candidate->get<Geometry>().position.as<math::Position>()))
        {
            mEngine.markToRemove(candidate);
        }
    }
}

} // namespace ad
