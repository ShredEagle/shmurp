#include "EnemySpawn.h"

#include "../configuration.h"
#include "../Entities/Ships.h"

#include <handy/random.h>

#include <math/Rectangle.h>


using namespace ad::math::angle_literals;


namespace ad {

void spawn(aunteater::Engine & aEngine)
{
    static Randomizer randomX(0, conf::gWindowWorldWidth);

    aEngine.addEntity(
            entities::makeSquare({static_cast<float>(randomX()), static_cast<float>(conf::gWindowWorldHeight + conf::gViewportOffset)},
                                 {0.f, -5.f},
                                 {0.4_radf, 1.1_radf, 0._radf}));
}

EnemySpawn::EnemySpawn(aunteater::Engine &aEngine) :
    mEngine(aEngine),
    mPrunables(aEngine)
{}

void EnemySpawn::update(const aunteater::Timer aTimer)
{
    for(auto count = mSpawnPeriod.countEvents(aTimer.delta()); count; --count)
    {
        spawn(mEngine);
    }

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
