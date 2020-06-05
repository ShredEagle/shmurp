#include "configuration.h"

#include "Entities/Ships.h"

#include "Components/Tweening.h"

#include "System/Interpolate.h"

#include "Utils/Periodic.h"
#include "Utils/TimedSequence.h"


namespace ad {


constexpr Vec<2> gBossPosition{conf::gWindowWorldWidth/2.0f, conf::gWindowWorldHeight-5.0f};
constexpr float gBoss1BallRadius = 3.5f;
constexpr float gBoss1SatelliteRadius = 0.7f * gBoss1BallRadius;
constexpr float gBoss1TurretRadius = conf::gPyramidRadius;
constexpr float gBoss1Padding = 0.2f;

constexpr duration_t gBoss1RotationAccelerationDuration = 4.0; //Integral of f(x)=x from 0..4 is 2
constexpr Radian<> gBoss1RotationTopSpeed = pi<Radian<>>;  // so 2*pi rotation during acceleration and deceleration, i.e. 1 revolution
constexpr duration_t gBoss1PhaseDuration = 6.0;

constexpr duration_t gBoss1TurretPeriod = 0.1;
constexpr Floating gBoss1TurretBulletRadius = 3*conf::gBulletRadius;
constexpr Floating gBoss1TurretBulletVelocity = conf::gBulletSpeed/5.0;

namespace detail {


    aunteater::Entity makeSatellite(aunteater::weak_entity aParent, Vec<2> aLocalPosition)
    {
        aunteater::Entity satellite;
        satellite
            .add<Geometry>(Vec<2>::Zero(), gBoss1SatelliteRadius)
            .add<SceneGraphComposite>(aLocalPosition,
                                      Vec<3, Radian<>>::Zero())
            .add<SceneGraphParent>(aParent)
            .add<Shape>(Shape::Circle)
            ;
        return satellite;
    }


    aunteater::Entity makeTurret(aunteater::weak_entity aParent,
                                 Vec<2> aLocalPosition=Vec<2>::Zero())
    {
        using namespace math::angle_literals;

        aunteater::Entity turret;
        turret
            .add<Geometry>(Vec<2>{0.f, 0.f}, gBoss1TurretRadius)
            .add<SceneGraphComposite>(aLocalPosition,
                                      Vec<3, Radian<>>{0._radf, 0._radf, -pi<Radian<>>/2.f},
                                      SceneGraphComposite::ResetOrientation)
            .add<SceneGraphParent>(aParent)
            .add<Shape>(Shape::Pyramid)
            .add<Speed>(Vec<2>::Zero(),
                        Vec<3, Radian<>>{1._radf, 0._radf, 0._radf})
            .add<TrackerPlayer>()
            ;
        return turret;
    }


    aunteater::Entity makeCanon(aunteater::weak_entity aParent,
                                 Vec<2> aLocalPosition=Vec<2>::Zero())
    {
        using namespace math::angle_literals;

        BulletConfig bulletConfig;
        bulletConfig.radius = gBoss1TurretBulletRadius;
        bulletConfig.velocity = gBoss1TurretBulletVelocity;

        TimedSequence<aunteater::LiveEntity &> canonSequence{
        { // The map
            { // A pair element of the map
                2*gBoss1PhaseDuration,
                [bulletConfig](timepoint_t /*ignored*/, aunteater::LiveEntity &aEntity)
                {
                    aEntity.add<FirePattern>(std::make_unique<Fire::Line<Periodic>>(
                                Periodic{gBoss1TurretPeriod},
                                std::move(bulletConfig)));
                }
            },
            {
                2*gBoss1PhaseDuration + gBoss1RotationAccelerationDuration,
                [](timepoint_t /*ignored*/, aunteater::LiveEntity &aEntity)
                {
                    aEntity.remove<FirePattern>();
                }
            },
        }};

        aunteater::Entity canon;
        canon
            .add<CustomCallback>(
                [sequence = std::move(canonSequence)]
                (aunteater::LiveEntity & aEntity, const aunteater::Timer & aTimer) mutable
                {
                    sequence.elapse(aTimer.delta(), aEntity);
                })
            .add<Geometry>(Vec<2>{0.f, 0.f})
            .add<SceneGraphComposite>(aLocalPosition)
            .add<SceneGraphParent>(aParent)
            ;
        return canon;
    }



    aunteater::Entity makeBoss1(aunteater::Engine & aEntityEngine)
    {
        using namespace math::angle_literals;

        //
        // Boss core
        //
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
            //
            // Central Rotation Axis
            //
            aunteater::Entity rotationAxis;
            rotationAxis
                .add<Geometry>()
                .add<SceneGraphComposite>(Vec<2>::Zero(),
                                          Vec<3, Radian<>>::Zero())
                .add<SceneGraphParent>(liveBoss)
                .add<Speed>(Vec<2>::Zero(),
                            Vec<3, Radian<>>{0._radf, 0._radf, 0._radf})
                .add<CustomCallback>(
                    [rotating = true, alternator = Periodic{gBoss1PhaseDuration}]
                    (aunteater::LiveEntity & aEntity, const aunteater::Timer & aTimer) mutable
                    {
                        if(alternator.countEvents(aTimer.delta()))
                        {
                            rotating = !rotating;
                            if (rotating)
                            {
                                aEntity.get<Tweening<Speed, Radian<>>>()
                                    .interpolation.redirect(gBoss1RotationTopSpeed);
                            }
                            else
                            {
                                aEntity.get<Tweening<Speed, Radian<>>>()
                                    .interpolation.redirect(0._radf);
                            }
                        }
                    })
                ;

            setupTweening<Speed>(rotationAxis,
                                 [](Speed & speed) -> Radian<> & {return speed.rotation.z();},
                                 0._radf,
                                 gBoss1RotationAccelerationDuration);

            aunteater::weak_entity liveRotationAxis = aEntityEngine.addEntity(rotationAxis);

            {
                constexpr float gSatelliteYOffset =
                    gBoss1BallRadius + gBoss1SatelliteRadius + gBoss1Padding;

                //
                // Left Satellite
                //
                aunteater::weak_entity liveLeftSatellite =
                    aEntityEngine.addEntity(detail::makeSatellite(liveRotationAxis,
                                                               Vec<2>{0.f, -gSatelliteYOffset}));
                {
                    //
                    // Left Turret
                    //
                    aunteater::weak_entity liveLeftTurret =
                        aEntityEngine.addEntity(detail::makeTurret(liveLeftSatellite));

                    {
                        aunteater::weak_entity liveLeftCanon =
                            aEntityEngine.addEntity(detail::makeCanon(
                                        liveLeftTurret,
                                        {gBoss1TurretRadius/2.f, 0.f}));
                    }
                }

                //
                // Right Satellite
                //
                aunteater::weak_entity liveRightSatellite =
                    aEntityEngine.addEntity(detail::makeSatellite(liveRotationAxis,
                                                               Vec<2>{0.f, gSatelliteYOffset}));
                {
                    //
                    // Right Turret
                    //
                    aunteater::weak_entity liveRightTurret =
                        aEntityEngine.addEntity(detail::makeTurret(liveRightSatellite));

                    {
                        aunteater::weak_entity liveRightCanon =
                            aEntityEngine.addEntity(detail::makeCanon(
                                        liveRightTurret,
                                        {gBoss1TurretRadius/2.f, 0.f}));
                    }

                    //aunteater::Entity bullet;
                    //bullet
                    //    .add<Geometry>(Vec<2>{0.f, 0.f}, conf::gBulletRadius)
                    //    .add<SceneGraphComposite>(Vec<2>{2.f, 0.f})
                    //    .add<SceneGraphParent>(liveRightTurret)
                    //    .add<Shape>(Shape::Pyramid)
                    //;
                    //aEntityEngine.addEntity(std::move(bullet));
                }
            }
        }
        return boss;
    }


} // namespace detail


void boss1(aunteater::Engine & aEntityEngine, Application & aApplication)
{
    /*
     * Systems
     */
    aEntityEngine.addSystem<Interpolate<Speed, Radian<>>>();

    /*
     * Entities
     */
    aEntityEngine.addEntity(entities::makeHero(Vec<2>{conf::shipInitialX, conf::shipInitialY}));
    aEntityEngine.addEntity(detail::makeBoss1(aEntityEngine));
}


} // namespace ad
