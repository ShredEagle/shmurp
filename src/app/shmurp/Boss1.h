#include "configuration.h"

#include "Entities/Ships.h"

#include "Components/Tweening.h"

#include "System/Interpolate.h"


namespace ad {


static constexpr Vec<2> gBossPosition{conf::gWindowWorldWidth/2.0f, conf::gWindowWorldHeight-5.0f};
static constexpr float gBoss1BallRadius = 3.5f;
static constexpr float gBoss1SatelliteRadius = 0.7f * gBoss1BallRadius;
static constexpr float gBoss1Padding = 0.2f;

static constexpr float gBoss1RotationAccelerationDuration = 5.0f;
static constexpr Radian<> gBoss1RotationTopSpeed = 2*pi<Radian<>>;


namespace detail {


    aunteater::Entity makeSatellite(aunteater::weak_entity aParent, Vec<2> aLocalPosition)
    {
        aunteater::Entity satellite;
        satellite
            .add<Geometry>(Vec<2>::Zero(), gBoss1SatelliteRadius)
            .add<SceneGraphComposite>(aLocalPosition,
                                      Vec<3, Radian<>>::Zero())
            .add<SceneGraphParent>(aParent)
            .add<Shape>(Shape::Circle);
        return satellite;
    }


} // namespace detail


void boss1(aunteater::Engine & aEntityEngine, Application & aApplication)
{
    using namespace math::angle_literals;

    /*
     * Systems
     */
    aEntityEngine.addSystem<Interpolate<Speed, Radian<>>>();

    /*
     * Entities
     */
    aEntityEngine.addEntity(entities::makeHero(Vec<2>{conf::shipInitialX, conf::shipInitialY}));

    aunteater::Entity boss;
    boss
        //.add<Faction>(Faction::Democrats, Faction::SpaceForce)
        .add<Geometry>(Vec<2>{0.f, 0.f}, gBoss1BallRadius)
        .add<SceneGraphComposite>(gBossPosition,
                                  Vec<3, Radian<>>{0._radf, 0._radf, -pi<Radian<>>/2.f})
        .add<SceneGraphParent>(/*root*/)
        .add<Shape>(Shape::Circle)
        .add<Speed>(Vec<2>::Zero(), Vec<3, Radian<>>::Zero());
    aunteater::weak_entity liveBoss = aEntityEngine.addEntity(boss);

    {
        aunteater::Entity rotationAxis;
        rotationAxis
            .add<Geometry>()
            .add<SceneGraphComposite>(Vec<2>::Zero(),
                                      Vec<3, Radian<>>::Zero())
            .add<SceneGraphParent>(liveBoss)
            .add<Speed>(Vec<2>::Zero(), Vec<3, Radian<>>{0._radf, 0._radf, 0.5_radf});
        aunteater::weak_entity liveRotationAxis = aEntityEngine.addEntity(rotationAxis);
        {
            constexpr float gSatelliteYOffset =
                gBoss1BallRadius + gBoss1SatelliteRadius + gBoss1Padding;

            aunteater::weak_entity liveLeftSatellite =
                aEntityEngine.addEntity(detail::makeSatellite(liveRotationAxis,
                                                           Vec<2>{0.f, -gSatelliteYOffset}));
            aunteater::weak_entity liveRightSatellite =
                aEntityEngine.addEntity(detail::makeSatellite(liveRotationAxis,
                                                           Vec<2>{0.f, gSatelliteYOffset}));
        }

        setupTweening<Speed>(*liveRotationAxis,
                             [](Speed & speed) -> Radian<> & {return speed.rotation.z();},
                             gBoss1RotationTopSpeed,
                             gBoss1RotationAccelerationDuration);
    }
}


} // namespace ad
