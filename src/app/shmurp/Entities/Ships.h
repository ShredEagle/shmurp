#pragma once

#include "configuration.h"

#include <Components/CustomCallback.h>
#include <Components/Faction.h>
#include <Components/FirePattern.h>
#include <Components/Geometry.h>
#include <Components/SceneGraphComposite.h>
#include <Components/SceneGraphParent.h>
#include <Components/Shape.h>
#include <Components/Speed.h>
#include <Components/TrackerPlayer.h>

#include <Utils/Rythm.h>

#include <aunteater/Entity.h>


namespace ad {
namespace entities {


inline aunteater::Entity makeSquare(Vec<2, GLfloat> aPosition,
                                    Vec<2, GLfloat> aTranslationSpeed,
                                    Vec<3, Radian<>> aRotationSpeed)
{
    return aunteater::Entity().add<Faction>(Faction::Democrats, Faction::TruthBullet)
                              .add<Geometry>(aPosition, conf::squareRadius)
                              .add<Shape>(Shape::Square)
                              .add<Speed>(aTranslationSpeed, aRotationSpeed);
}


inline void addTrackingPyramid(aunteater::Engine & aEngine,
                               Vec<2, GLfloat> aPosition,
                               Vec<2, GLfloat> aTranslationSpeed,
                               Vec<3, Radian<>> aRotationSpeed)
{
    static constexpr float firePeriod = 0.15f;
    static constexpr int burstSize = 4;

    Rythm fourFour{firePeriod, 2*burstSize};
    for (int i = 0; i != burstSize; ++i)
    {
        fourFour.note(i);
    }

    auto customBehaviour = [firing = false, alternator = Periodic{burstSize*firePeriod}]
        (aunteater::LiveEntity & aSelf, const aunteater::Timer aTimer) mutable
        {
            if (alternator.countEvents(aTimer.delta()))
            {
                firing = !firing;
                if (firing)
                {
                    aSelf.remove<TrackerPlayer>();
                }
                else
                {
                    aSelf.add<TrackerPlayer>();
                }
            }
        };

    using namespace math::angle_literals;

    aunteater::weak_entity ship = aEngine.addEntity(
        aunteater::Entity()
            .add<CustomCallback>(std::move(customBehaviour))
            .add<Faction>(Faction::Democrats, Faction::SpaceForce)
            .add<Geometry>(Vec<2>{0.f, 0.f}, conf::gPyramidRadius)
            .add<SceneGraphComposite>(aPosition,
                                      Vec<3, Radian<>>{0._radf, 0._radf, -pi<Radian<>>/2.f})
            .add<SceneGraphParent>(/*root*/)
            .add<Shape>(Shape::Pyramid)
            .add<Speed>(aTranslationSpeed, aRotationSpeed));

    aEngine.addEntity(
        aunteater::Entity()
            .add<FirePattern>(std::make_unique<Fire::Line<Rythm>>(fourFour))
            .add<Geometry>(Vec<2>{0.f, 0.f}, 0.f) // The position is handled by SceneGraph system
            .add<SceneGraphComposite>(Vec<2>{.5f, 0.f})
            .add<SceneGraphParent>(ship));
}


} // namespace entities
} // namespace ad

