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
constexpr Floating gBoss1ReferenceBulletVelocity = conf::gBulletSpeed/8.0;

constexpr duration_t gBoss1RotationAccelerationDuration = 4.0; //Integral of f(x)=x from 0..4 is 2
constexpr Radian<> gBoss1RotationTopSpeed = pi<Radian<>>;  // so 2*pi rotation during acceleration and deceleration, i.e. 1 revolution
constexpr duration_t gBoss1PhaseDuration = 6.0;

constexpr duration_t gBoss1TurretPeriod = 0.1;
constexpr Floating gBoss1TurretBulletRadius = 3*conf::gBulletRadius;
constexpr Floating gBoss1TurretBulletVelocity = gBoss1ReferenceBulletVelocity;

constexpr duration_t gBoss1CoreCanonPeriod = 1.0;
constexpr Floating gBoss1CoreCanonBaseYPosition = gBoss1BallRadius/1.5f;
constexpr Floating gBoss1CoreCanonOffset = gBoss1BallRadius/5.f;

constexpr Floating gBoss1ArcCanonBulletCount = 15;
constexpr Floating gBoss1ArcCanonBulletRadius = 1.5*gBoss1TurretBulletRadius;
constexpr Floating gBoss1ArcCanonBulletVelocity = gBoss1ReferenceBulletVelocity * 1.5f;

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


    aunteater::Entity makeGenericCanon(aunteater::weak_entity aParent,
                                       TimedSequence<aunteater::LiveEntity &> aSequence,
                                       Vec<2> aLocalPosition=Vec<2>::Zero())
    {
        using namespace math::angle_literals;

        aunteater::Entity canon;
        canon
            .add<CustomCallback>(
                [sequence = std::move(aSequence)]
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


    aunteater::Entity makeTurretCanon(aunteater::weak_entity aParent,
                                      Vec<2> aLocalPosition=Vec<2>::Zero())
    {
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

        return makeGenericCanon(aParent, std::move(canonSequence), std::move(aLocalPosition));
    }


    aunteater::Entity makeArcCanon(aunteater::weak_entity aParent,
                                   Vec<2> aLocalPosition=Vec<2>::Zero())
    {
        BulletConfig bulletConfig;
        bulletConfig.radius = gBoss1ArcCanonBulletRadius;
        bulletConfig.velocity = gBoss1ArcCanonBulletVelocity;

        TimedSequence<aunteater::LiveEntity &> canonSequence{
        { // The map
            { // A pair element of the map
                gBoss1PhaseDuration,
                [bulletConfig](timepoint_t /*ignored*/, aunteater::LiveEntity &aEntity)
                {
                    aEntity.add<FirePattern>(std::make_unique<Fire::Arc>(
                                std::numeric_limits<duration_t>::infinity(),
                                3.f/8.f * 2*pi<Radian<>>,
                                gBoss1ArcCanonBulletCount,
                                std::move(bulletConfig)));
                }
            },
            //{
            //    gBoss1PhaseDuration,
            //    [](timepoint_t /*ignored*/, aunteater::LiveEntity &aEntity)
            //    {
            //        aEntity.remove<FirePattern>();
            //    }
            //},
        }};

        return makeGenericCanon(aParent, std::move(canonSequence), std::move(aLocalPosition));
    }


    aunteater::Entity makeCoreCanon(aunteater::weak_entity aParent,
                                    duration_t aPhase,
                                    Vec<2> aLocalPosition=Vec<2>::Zero())
    {
        BulletConfig bulletConfig;
        bulletConfig.radius = gBoss1TurretBulletRadius;
        bulletConfig.velocity = gBoss1TurretBulletVelocity;

        TimedSequence<aunteater::LiveEntity &> canonSequence{
        { // The map
            { // A pair element of the map
                aPhase,
                [bulletConfig](timepoint_t /*ignored*/, aunteater::LiveEntity &aEntity)
                {
                    aEntity.add<FirePattern>(std::make_unique<Fire::Burst<3>>(
                                gBoss1CoreCanonPeriod,
                                pi<Radian<>> / 2.f,
                                std::move(bulletConfig)));
                }
            },
            {
                gBoss1PhaseDuration,
                [](timepoint_t /*ignored*/, aunteater::LiveEntity &aEntity)
                {
                    aEntity.remove<FirePattern>();
                }
            },
        }};

        return makeGenericCanon(aParent, std::move(canonSequence), std::move(aLocalPosition));
    }



    void addCircleOfCoreCanons(aunteater::Engine & aEntityEngine,
                               aunteater::weak_entity aParent,
                               int aCanonCount,
                               Floating aCircleRadius,
                               Vec<2> aCirclePosition = Vec<2>::Zero())
    {
        for(int canonId = 0; canonId != aCanonCount; ++canonId)
        {
            Radian<> polarDirection = pi<Radian<>>/2.f + (canonId*(2*pi<Radian<>>)/aCanonCount);
            Vec<2> offsetFromCenter{cos(polarDirection), sin(polarDirection)};
            offsetFromCenter *= aCircleRadius;

            aunteater::Entity canon =
                makeCoreCanon(aParent,
                              canonId * gBoss1CoreCanonPeriod/aCanonCount,
                              aCirclePosition + offsetFromCenter);
            aEntityEngine.addEntity(canon);
        }
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
            // Core Canons
            //

            // Note: Firing those as a wall (no random, all in the same direction)
            // makes another cool pattern

            //
            // Left triplet
            //
            addCircleOfCoreCanons(aEntityEngine,
                                  liveBoss,
                                  3,
                                  gBoss1CoreCanonOffset,
                                  {0.f, -gBoss1CoreCanonBaseYPosition});

            //
            // Right triplet
            //
            addCircleOfCoreCanons(aEntityEngine,
                                  liveBoss,
                                  3,
                                  gBoss1CoreCanonOffset,
                                  {0.f, +gBoss1CoreCanonBaseYPosition});
            //
            // Top triplet
            //
            addCircleOfCoreCanons(aEntityEngine,
                                  liveBoss,
                                  3,
                                  gBoss1CoreCanonOffset,
                                  {-gBoss1CoreCanonBaseYPosition, 0.f});
        }

        {
            //
            // Arc Canon
            //
            aEntityEngine.addEntity(makeArcCanon(liveBoss, {-gBoss1BallRadius, 0.f}));
        }

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
                            aEntityEngine.addEntity(detail::makeTurretCanon(
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
                            aEntityEngine.addEntity(detail::makeTurretCanon(
                                        liveRightTurret,
                                        {gBoss1TurretRadius/2.f, 0.f}));
                    }
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
    entities::addHero(aEntityEngine,
                      Vec<2>{conf::shipInitialX, conf::shipInitialY});
    aEntityEngine.addEntity(detail::makeBoss1(aEntityEngine));
}


} // namespace ad
