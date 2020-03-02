#include "EnemySpawn.h"

#include "../configuration.h"
#include "../Entities.h"

#include <handy/random.h>

#include <math/Rectangle.h>

namespace ad {

void spawn(aunteater::Engine & aEngine)
{
    static Randomizer randomX(0, conf::gWindowWorldWidth);
    static Randomizer randomRotation(-20, 20);
    static constexpr GLfloat rotationQuant = 2.5f/20.f;

    aEngine.addEntity(
            entities::makeSquare({randomX(), conf::gWindowWorldHeight + conf::gViewportOffset},
                                 {0.f, -5.f},
                                 {rotationQuant*randomRotation(), rotationQuant*randomRotation(), 0.f}));
}

EnemySpawn::EnemySpawn(aunteater::Engine &aEngine) :
    mEngine(aEngine),
    mPrunables(aEngine)
{}

void EnemySpawn::update(double time)
{
    for(auto count = mSpawnPeriod.countEvents(time); count; --count)
    {
        spawn(mEngine);
    }

    for (const auto candidate : mPrunables)
    {
        static const math::Rectangle<GLfloat> gGameArea{
            {-conf::gViewportOffset, -conf::gViewportOffset},
            {conf::gWindowWorldWidth+2*conf::gViewportOffset, conf::gWindowWorldHeight+2*conf::gViewportOffset}};

        if (!gGameArea.contains(candidate->get<Geometry>().position.as<math::Position>()))
        {
            mEngine.markToRemove(candidate);
        }
    }
}

} // namespace ad
