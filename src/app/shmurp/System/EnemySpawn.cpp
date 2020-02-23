#include "EnemySpawn.h"

#include "../configuration.h"

#include <Components/Faction.h>
#include <Components/Shape.h>

#include <handy/random.h>

#include <math/Rectangle.h>

namespace ad {

void spawn(aunteater::Engine & aEngine)
{
    static Randomizer randomX(0, conf::gWindowWorldWidth);

    using aunteater::Entity;

    aEngine.addEntity(Entity().add<Faction>(Faction::Democrats, Faction::TruthBullet)
                              .add<Geometry>(randomX(),
                                             conf::gWindowWorldHeight + conf::gViewportOffset,
                                             conf::squareRadius)
                              .add<Shape>(Shape::Square)
                              .add<Speed>(0.f, -5.f));
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

        if (!gGameArea.contains(static_cast<math::Position<2, GLfloat>>(candidate->get<Geometry>().position)))
        {
            mEngine.markToRemove(candidate);
        }
    }
}

} // namespace ad
