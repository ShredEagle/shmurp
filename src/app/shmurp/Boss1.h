#include "configuration.h"

#include "Entities/Ships.h"

#include "Components/BossEvent.h"
#include "Components/Tweening.h"

#include "System/EventQueue.h"
#include "System/Interpolate.h"

#include "Utils/Delay.h"
#include "Utils/Iterative.h"
#include "Utils/Periodic.h"
#include "Utils/TimedSequence.h"


namespace ad {

constexpr duration_t gInfinity = std::numeric_limits<duration_t>::infinity();

constexpr float gBoss1BallRadius = 3.5f;
constexpr float gBoss1SatelliteRadius = 0.7f * gBoss1BallRadius;
constexpr float gBoss1TurretRadius = conf::gPyramidRadius;
constexpr float gBoss1Padding = 0.2f;
constexpr Floating gBoss1ReferenceBulletVelocity = conf::gBulletSpeed/8.0;

constexpr duration_t gBoss1RotationAccelerationDuration = 4.0; //Integral of f(x)=x from 0..4 is 2
constexpr Radian<> gBoss1RotationTopSpeed = pi<Radian<>>;  // so 2*pi rotation during acceleration and deceleration, i.e. 1 revolution
constexpr duration_t gBoss1PhaseDuration = 6.0;
constexpr duration_t gBoss1CompoundFirePeriod = gBoss1PhaseDuration/3.f;

constexpr duration_t gBoss1TurretPeriod = 0.1;
constexpr Floating gBoss1TurretBulletRadius = 3*conf::gBulletRadius;
constexpr Floating gBoss1TurretBulletVelocity = gBoss1ReferenceBulletVelocity;

constexpr int gBoss1CompoundIterations = 4;

constexpr duration_t gBoss1CoreCanonPeriod = 1.0;
constexpr Floating gBoss1CoreCanonBaseYPosition = gBoss1BallRadius/1.5f;
constexpr Floating gBoss1CoreCanonOffset = gBoss1BallRadius/5.f;

constexpr Floating gBoss1ArcCanonBulletCount = 15.f;
constexpr Floating gBoss1ArcCanonBulletRadius = 1.5*gBoss1TurretBulletRadius;
constexpr Floating gBoss1ArcCanonBulletVelocity = gBoss1ReferenceBulletVelocity * 1.5f;

constexpr Floating gBoss1TurretLaserRadius = conf::gBulletRadius;
constexpr Floating gBoss1TurretLaserPeriod = 0.6f;
constexpr Floating gBoss1TurretLaserVelocity = gBoss1ReferenceBulletVelocity * 0.5f;

namespace Boss1 {

    constexpr Vec<2> gStartPosition = {conf::gWindowWorldWidth/2.0f, conf::gWindowWorldHeight-5.0f};

    constexpr std::array<Vec<2>, 4> gPositions = {
        gStartPosition + Vec<2>{-4.f, -3.f},
        gStartPosition,
        gStartPosition + Vec<2>{+4.f, -3.f},
        gStartPosition,
    };

    constexpr duration_t gDisplacementDuration = 7.f;

    constexpr Radian<> gCircleCoreCanonsCoverage = pi<Radian<>>/3.f;
    constexpr int gCircleCoreCanonsArcCount = 6;

} // namespace Boss1


namespace detail {


    aunteater::Entity makeSatellite(aunteater::weak_entity aParent, Vec<2> aLocalPosition)
    {
        using namespace math::angle_literals;

        aunteater::Entity satellite;
        satellite
            .add<Geometry>(gBoss1SatelliteRadius)
            .add<SceneGraphComposite>(aLocalPosition,
                                      Vec<3, Radian<>>{0._radf, 0._radf, -pi<Radian<>>/2.f},
                                      SceneGraphComposite::ResetOrientation)
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
            .add<Geometry>(gBoss1TurretRadius)
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


    class BossEventFunctor
    {
    public:
        using phase_fun = std::function<void(aunteater::LiveEntity &)>;

        BossEventFunctor(aunteater::LiveEntity & aSource) :
            mSource{entityIdFrom(aSource)}
        {}

        BossEventFunctor(aunteater::entity_id aSourceId) :
            mSource{aSourceId}
        {}

        template <BossEvent::Phase N_phase>
        BossEventFunctor & on(phase_fun aFunction)
        {
            mPhaseFunctions.emplace(N_phase, std::move(aFunction));
            return *this;
        }

        void operator()(aunteater::entity_id /*aEventEntityId*/,
                        const BossEvent & aEvent,
                        aunteater::LiveEntity & aObserver)
        {
            if (aEvent.source != mSource)
            {
                return;
            }

            auto found = mPhaseFunctions.find(aEvent.phase);
            if (found != mPhaseFunctions.end())
            {
                std::invoke(found->second, aObserver);
            }
        }

    private:
        aunteater::entity_id mSource;
        std::map<BossEvent::Phase, phase_fun> mPhaseFunctions;
    };

    aunteater::Entity makeLaserCanon(aunteater::entity_id aBossId,
                                     aunteater::weak_entity aParent,
                                     Vec<2> aLocalPosition=Vec<2>::Zero())
    {
        using namespace math::angle_literals;

        BulletConfig bulletConfig;
        bulletConfig.radius = gBoss1TurretLaserRadius;
        bulletConfig.velocity = gBoss1TurretLaserVelocity;
        bulletConfig.shape = Shape::Laser;

        auto handler = BossEventFunctor{aBossId}
            .on<BossEvent::LaserOn>([bulletConfig](aunteater::LiveEntity & aEntity)
                    {
                        aEntity.add<FirePattern>(std::make_unique<Fire::Line<Periodic>>(
                                    Periodic{gBoss1TurretLaserPeriod},
                                    std::move(bulletConfig)));
                    })
            .on<BossEvent::LaserOff>([](aunteater::LiveEntity & aEntity)
                    {
                        aEntity.remove<FirePattern>();
                    })
            ;

        aunteater::Entity canon;
        canon
            .add<EventObserver<BossEvent>>(std::move(handler))
            .add<Geometry>()
            .add<SceneGraphComposite>(aLocalPosition)
            .add<SceneGraphParent>(aParent)
            ;
        return canon;
    }


    aunteater::Entity makeGenericCanon(aunteater::weak_entity aParent,
                                       BossEventFunctor aBossEventHandler,
                                       Vec<2> aLocalPosition=Vec<2>::Zero(),
                                       Vec<3, Radian<>> aLocalOrientation=Vec<3, Radian<>>::Zero())
    {
        using namespace math::angle_literals;

        aunteater::Entity canon;
        canon
            .add<EventObserver<BossEvent>>(std::move(aBossEventHandler))
            .add<Geometry>()
            .add<SceneGraphComposite>(aLocalPosition, aLocalOrientation)
            .add<SceneGraphParent>(aParent)
            ;
        return canon;
    }


    aunteater::Entity makeTurretCanon(aunteater::entity_id aBossId,
                                      aunteater::weak_entity aParent,
                                      Vec<2> aLocalPosition=Vec<2>::Zero())
    {
        BulletConfig bulletConfig;
        bulletConfig.radius = gBoss1TurretBulletRadius;
        bulletConfig.velocity = gBoss1TurretBulletVelocity;

        auto handler = BossEventFunctor{aBossId}
            .on<BossEvent::Turret_1>([bulletConfig](aunteater::LiveEntity & aEntity)
                    {
                        aEntity.add<FirePattern>(std::make_unique<Fire::Line<Periodic>>(
                                    Periodic{gBoss1TurretPeriod},
                                    std::move(bulletConfig)));
                    })
            .on<BossEvent::TurretOff>([](aunteater::LiveEntity & aEntity)
                    {
                        aEntity.remove<FirePattern>();
                    })
            ;

        return makeGenericCanon(aParent, std::move(handler), std::move(aLocalPosition));
    }


    aunteater::Entity makeArcCanon(aunteater::entity_id aBossId,
                                   aunteater::weak_entity aParent,
                                   Vec<2> aLocalPosition=Vec<2>::Zero())
    {
        BulletConfig bulletConfig;
        bulletConfig.radius = gBoss1ArcCanonBulletRadius;
        bulletConfig.velocity = gBoss1ArcCanonBulletVelocity;

        auto handler = BossEventFunctor{aBossId}
            .on<BossEvent::Arc>([bulletConfig](aunteater::LiveEntity & aEntity)
                    {
                        aEntity.add<FirePattern>(std::make_unique<Fire::Arc>(
                                    std::numeric_limits<duration_t>::infinity(),
                                    3.f/8.f * 2*pi<Radian<>>,
                                    gBoss1ArcCanonBulletCount,
                                    std::move(bulletConfig)));
                    })
            // TODO would be better to actually remove the FirePattern
            // for the moment, its period is infinite, so it shoot only when added (or re-added)
            //.on<BossEvent::ArcOff>([](aunteater::LiveEntity & aEntity)
            //        {
            //            aEntity.remove<FirePattern>();
            //        })
            ;

        return makeGenericCanon(aParent, std::move(handler), std::move(aLocalPosition));
    }


    template <BossEvent::Phase N_compoundTrigger>
    aunteater::Entity makeCoreCanon(aunteater::entity_id aBossId,
                                    aunteater::weak_entity aParent,
                                    duration_t aDelay,
                                    Floating aArcBulletVelocity,
                                    Radian<> aArcCoverage,
                                    Vec<2> aLocalPosition=Vec<2>::Zero(),
                                    Vec<3, Radian<>> aLocalOrientation=Vec<3, Radian<>>::Zero())
    {
        BulletConfig bulletConfig;
        bulletConfig.radius = gBoss1TurretBulletRadius;
        bulletConfig.velocity = gBoss1TurretBulletVelocity;

        BulletConfig arcBulletConfig{bulletConfig};
        arcBulletConfig.velocity = aArcBulletVelocity;

        auto handler = BossEventFunctor{aBossId}
            .on<BossEvent::CoreCanon_1>([bulletConfig, aDelay](aunteater::LiveEntity & aEntity)
                    {
                        aEntity.add<FirePattern>(std::make_unique<Fire::Spray<3, Delay<Periodic>>>(
                                    pi<Radian<>> / 2.f,
                                    std::move(bulletConfig),
                                    aDelay,
                                    gBoss1CoreCanonPeriod));
                    })
            .template on<BossEvent::CoreCanonOff>([](aunteater::LiveEntity & aEntity)
                    {
                        aEntity.remove<FirePattern>();
                    })
            .template on<N_compoundTrigger>([bulletConfig, arcBulletConfig, aArcCoverage]
                    (aunteater::LiveEntity & aEntity)
                    {
                        auto compositeFire = std::make_unique<Fire::Composite>();

                        compositeFire->components.emplace_back(
                            std::make_unique<Fire::Line<Iterative<Periodic>>>(
                                    Iterative<Periodic>{gBoss1CompoundIterations, gInfinity},
                                    bulletConfig));

                        compositeFire->components.emplace_back(
                            std::make_unique<Fire::Arc>(
                                gInfinity,
                              aArcCoverage,
                              Boss1::gCircleCoreCanonsArcCount,
                              arcBulletConfig));

                        aEntity.add<FirePattern>(std::move(compositeFire));
                    })
            ;
        return makeGenericCanon(aParent, std::move(handler),
                                std::move(aLocalPosition), std::move(aLocalOrientation));
    }



    template <BossEvent::Phase N_compoundTrigger>
    void addCircleOfCoreCanons(aunteater::entity_id aBossId,
                               aunteater::Engine & aEntityEngine,
                               aunteater::weak_entity aParent,
                               int aCanonCount,
                               Radian<> aCoverage,
                               Floating aCircleRadius,
                               Radian<> aAimOffset,
                               Vec<2> aCirclePosition = Vec<2>::Zero())
    {
        const ArcQuantifier arc{aCoverage, aCanonCount};
        for(int canonId = 0; canonId != aCanonCount; ++canonId)
        {
            // We need to have the "rear" canon happen in the middle of the sequence
            // in order for the arc quantizer to apply the cenral value to it
            Radian<> polarDirection = - pi<Radian<>>/aCanonCount
                - canonId*(2*pi<Radian<>>)/aCanonCount;

            Vec<2> offsetFromCenter{cos(polarDirection), sin(polarDirection)};
            offsetFromCenter *= aCircleRadius;

            auto orientations = Vec<3, Radian<>>::Zero();
            orientations.z() = arc.getAngleFor(canonId) + aAimOffset;

            Floating arcBulletVelocity = (canonId == (aCanonCount/2)) // only works for odd canon count
                ? gBoss1TurretBulletVelocity * dampenFactor(gBoss1CompoundIterations/*-1*/) // slower than last line bullet
                : gBoss1TurretBulletVelocity;

            aunteater::Entity canon =
                makeCoreCanon<N_compoundTrigger>
                             (aBossId,
                              aParent,
                              canonId * gBoss1CoreCanonPeriod/aCanonCount,
                              arcBulletVelocity,
                              Boss1::gCircleCoreCanonsCoverage,
                              aCirclePosition + offsetFromCenter,
                              orientations);
            aEntityEngine.addEntity(canon);
        }
    }


    template <class T_event, class... VT_ctorArgs>
    void postEvent(aunteater::Engine & aEngine,
                   aunteater::LiveEntity & aSource,
                   VT_ctorArgs &&... vaCtorArgs)
    {
        aEngine.addEntity(aunteater::Entity().add<T_event>(entityIdFrom(aSource),
                                                           std::forward<VT_ctorArgs>(vaCtorArgs)...));
    }


    template <class T_entity>
    void recursePositionTween(T_entity & aEntity, std::size_t aPositionId=0)
    {
        aEntity
            .template add<LiveTweening<SceneGraphComposite, Vec<2>>>(
                [](SceneGraphComposite & sgc) -> Vec<2> & {return sgc.position;},
                Boss1::gPositions[aPositionId],
                Boss1::gDisplacementDuration,
                [aPositionId](AUNTEATER_CB_ARGS)
                    {
                        recursePositionTween(aEntity,
                                             (aPositionId+1) % Boss1::gPositions.size());
                    })
        ;
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
            .add<Geometry>(gBoss1BallRadius)
            .add<SceneGraphComposite>(Boss1::gStartPosition,
                                      Vec<3, Radian<>>{0._radf, 0._radf, -pi<Radian<>>/2.f})
            .add<SceneGraphParent>(/*root*/)
            .add<Shape>(Shape::Circle)
            .add<Speed>(Vec<2>::Zero(), Vec<3, Radian<>>::Zero());


        //
        // The boss perpatual displacement
        //
        recursePositionTween(boss);

        //
        // The main boss automata
        //
        TimedSequence<aunteater::LiveEntity &, aunteater::Engine &> bossSequence{
        { // The map
            { // A pair element of the map
                0,
                [](timepoint_t /*ignored*/, aunteater::LiveEntity & aEntity, aunteater::Engine & aEngine)
                {
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::CoreCanon_1);
                }
            },
            { // A pair element of the map
                gBoss1PhaseDuration,
                [](timepoint_t /*ignored*/, aunteater::LiveEntity & aEntity, aunteater::Engine & aEngine)
                {
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::CoreCanonOff);

                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::Arc);
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::Rotate);
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::LaserOn);
                }
            },
            {
                2*gBoss1PhaseDuration,
                [](timepoint_t /*ignored*/, aunteater::LiveEntity & aEntity, aunteater::Engine & aEngine)
                {
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::Stabilize);
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::Turret_1);
                }
            },
            {
                2*gBoss1PhaseDuration + gBoss1RotationAccelerationDuration,
                [](timepoint_t /*ignored*/, aunteater::LiveEntity & aEntity, aunteater::Engine & aEngine)
                {
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::LaserOff);
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::TurretOff);
                }
            },
            {
                3*gBoss1PhaseDuration,
                [](timepoint_t /*ignored*/, aunteater::LiveEntity & aEntity, aunteater::Engine & aEngine)
                {
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::LeftCompoundFire);
                }
            },
            {
                3*gBoss1PhaseDuration + 1*gBoss1CompoundFirePeriod,
                [](timepoint_t /*ignored*/, aunteater::LiveEntity & aEntity, aunteater::Engine & aEngine)
                {
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::RightCompoundFire);
                }
            },
            {
                3*gBoss1PhaseDuration + 2*gBoss1CompoundFirePeriod ,
                [](timepoint_t /*ignored*/, aunteater::LiveEntity & aEntity, aunteater::Engine & aEngine)
                {
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::LeftCompoundFire);
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::RightCompoundFire);
                    postEvent<BossEvent>(aEngine, aEntity, BossEvent::TopCompoundFire);
                }
            },
        }};

        boss
            .add<CustomCallback>(
                [sequence = std::move(bossSequence)]
                (aunteater::LiveEntity & aEntity, const aunteater::Timer & aTimer, aunteater::Engine & aEngine) mutable
                {
                    sequence.elapse(aTimer.delta(), aEntity, aEngine);
                })
            ;

        aunteater::weak_entity liveBoss = aEntityEngine.addEntity(boss);
        aunteater::entity_id liveBossId = entityIdFrom(*liveBoss);
        {
            //
            // Core Canons
            //

            // Note: Firing those as a wall (no random, all in the same direction)
            // makes another cool pattern

            //
            // Left triplet
            //
            addCircleOfCoreCanons<BossEvent::LeftCompoundFire>
                                 (entityIdFrom(*liveBoss),
                                  aEntityEngine,
                                  liveBoss,
                                  3,
                                  Boss1::gCircleCoreCanonsCoverage,
                                  gBoss1CoreCanonOffset,
                                  pi<Radian<>>/5.f,
                                  {0.f, -gBoss1CoreCanonBaseYPosition});

            //
            // Right triplet
            //
            addCircleOfCoreCanons<BossEvent::RightCompoundFire>
                                 (entityIdFrom(*liveBoss),
                                  aEntityEngine,
                                  liveBoss,
                                  3,
                                  Boss1::gCircleCoreCanonsCoverage,
                                  gBoss1CoreCanonOffset,
                                  -pi<Radian<>>/5.f,
                                  {0.f, +gBoss1CoreCanonBaseYPosition});
            //
            // Top triplet
            //
            addCircleOfCoreCanons<BossEvent::TopCompoundFire>
                                 (entityIdFrom(*liveBoss),
                                  aEntityEngine,
                                  liveBoss,
                                  3,
                                  Boss1::gCircleCoreCanonsCoverage,
                                  gBoss1CoreCanonOffset,
                                  0._radf,
                                  {-gBoss1CoreCanonBaseYPosition, 0.f});

        }

        {
            //
            // Arc Canon
            //
            aEntityEngine.addEntity(makeArcCanon(entityIdFrom(*liveBoss),
                                                 liveBoss,
                                                 {-gBoss1BallRadius, 0.f}));
        }

        {
            //
            // Central Rotation Axis
            //
            auto handler = BossEventFunctor{*liveBoss}
                .on<BossEvent::Rotate>([](aunteater::LiveEntity & aEntity)
                        {
                            aEntity.add<LiveTweening<Speed, Radian<>>>(
                                    [](Speed & speed) -> Radian<> & {return speed.rotation.z();},
                                    gBoss1RotationTopSpeed,
                                    gBoss1RotationAccelerationDuration);
                        })
                .on<BossEvent::Stabilize>([](aunteater::LiveEntity & aEntity)
                        {
                            aEntity.add<LiveTweening<Speed, Radian<>>>(
                                    [](Speed & speed) -> Radian<> & {return speed.rotation.z();},
                                    0._radf,
                                    gBoss1RotationAccelerationDuration);
                        })
                ;

            aunteater::Entity rotationAxis;
            rotationAxis
                .add<Geometry>()
                .add<SceneGraphComposite>(Vec<2>::Zero(),
                                          Vec<3, Radian<>>::Zero())
                .add<SceneGraphParent>(liveBoss)
                .add<Speed>(Vec<2>::Zero(),
                            Vec<3, Radian<>>{0._radf, 0._radf, 0._radf})
                .add<EventObserver<BossEvent>>(handler)
                ;

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
                                        entityIdFrom(*liveBoss),
                                        liveLeftTurret,
                                        {gBoss1TurretRadius/2.f, 0.f}));
                    }

                }

                {
                    //
                    // Left Laser
                    //
                    aunteater::weak_entity liveLeftLaser =
                        aEntityEngine.addEntity(detail::makeLaserCanon(
                                    entityIdFrom(*liveBoss),
                                    liveLeftSatellite,
                                    {gBoss1SatelliteRadius, 0.f}));
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
                                        entityIdFrom(*liveBoss),
                                        liveRightTurret,
                                        {gBoss1TurretRadius/2.f, 0.f}));
                    }
                }

                {
                    //
                    // Right Laser
                    //
                    aunteater::weak_entity liveRightLaser =
                        aEntityEngine.addEntity(detail::makeLaserCanon(
                                    entityIdFrom(*liveBoss),
                                    liveRightSatellite,
                                    {gBoss1SatelliteRadius, 0.f}));
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
    aEntityEngine.addSystem<Interpolate<SceneGraphComposite, Vec<2>>>();
    aEntityEngine.addSystem<EventQueue<BossEvent>>();

    /*
     * Entities
     */
    entities::addHero(aEntityEngine,
                      Vec<2>{conf::shipInitialX, conf::shipInitialY});
    detail::makeBoss1(aEntityEngine);
}


} // namespace ad
