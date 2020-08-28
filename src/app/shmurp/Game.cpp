#include "Game.h"

#include "Boss1.h"
#include "configuration.h"

#include "Components/Geometry.h"
#include "Components/Shape.h"

#include "Entities/Ships.h"

#include "System/BulletSpawn.h"
#include "System/Collision.h"
#include "System/Customizer.h"
#include "System/Damage.h"
#include "System/Displace.h"
#include "System/EnemySpawn.h"
#include "System/KeyboardControl.h"
#include "System/LimitPosition.h"
#include "System/PruneEntities.h"
#include "System/Rendering.h"
#include "System/Rendering3D.h"
#include "System/SceneGraph.h"
#include "System/Tracking.h"

#include <aunteater/UpdateTiming.h>

using aunteater::Entity;

using namespace ad::math::angle_literals;

namespace ad {
namespace shmurp {


void Game::setBaseSystems(aunteater::Engine & aEntityEngine, Application & aApplication)
{
    /*
     * Systems
     */
    aEntityEngine.addSystem<Customizer>();

    auto kbControl = std::make_shared<KeyboardControl>(aEntityEngine);
    aApplication.getEngine()->registerKeyCallback(kbControl->getCallback());
    aEntityEngine.addSystem(kbControl);

    aEntityEngine.addSystem<Tracking>();

    aEntityEngine.addSystem<Displace>();

    aEntityEngine.addSystem<LimitPosition>();

    aEntityEngine.addSystem<SceneGraph>();

    aEntityEngine.addSystem<Collision>(mImpactEvents);

    aEntityEngine.addSystem<Damage>(mImpactEvents);

    aEntityEngine.addSystem<BulletSpawn>();

    aEntityEngine.addSystem<PruneEntities>();

    //aEntityEngine.addSystem<Rendering>();
    aEntityEngine.addSystem<Rendering3D>(*aApplication.getEngine());
}


void cubeLevel(aunteater::Engine & aEntityEngine, Application & aApplication)
{
    /*
     * Systems
     */
    aEntityEngine.addSystem<EnemySpawn>();

    /*
     * Entities
     */
    entities::addHero(aEntityEngine, Vec<2>{conf::shipInitialX, conf::shipInitialY});

    entities::addTrackingPyramid(
            aEntityEngine,
            Vec<2>{5.f, conf::gWindowWorldHeight-5.f},
            Vec<2>{0.f, 0.f},
            Vec<3, Radian<>>{0.4_radf, 0._radf, 0._radf});

    //aEntityEngine.addEntity(Entity().add<FirePattern>(std::make_unique<Fire::Spiral>(0.05f, pi<float>))
    //                                .add<Geometry>(5.f, conf::gWindowWorldHeight-5.f, conf::squareRadius)
    //                                .add<Shape>(Shape::RocketShip)
    //                                .add<Speed>(0., 0.)
    //                                );
    //aEntityEngine.addEntity(Entity().add<FirePattern>(std::make_unique<Fire::Spiral>(0.05f, pi<float>))
    //                                .add<Geometry>(conf::gWindowWorldWidth-5.f, conf::gWindowWorldHeight-5.f, conf::squareRadius)
    //                                .add<Shape>(Shape::RocketShip)
    //                                .add<Speed>(0., 0.)
    //                                );
    //aEntityEngine.addEntity(Entity().add<FirePattern>(std::make_unique<Fire::Circle>(0.5f, 25))
    //                                .add<Geometry>(conf::gWindowWorldWidth/2.f, conf::gWindowWorldHeight-5.f, conf::squareRadius)
    //                                .add<Shape>(Shape::RocketShip)
    //                                .add<Speed>(0., 0.)
    //                                );
}


Game::Game(Application & aApplication)
{
    setBaseSystems(mEntityEngine, aApplication);
    //cubeLevel(mEntityEngine, aApplication);
    boss1(mEntityEngine, aApplication);
}


bool Game::update(const aunteater::Timer & aTimer)
{
    aunteater::UpdateTiming timings;
    mEntityEngine.update(aTimer, timings);
    mImpactEvents.clear();

    mUI.broadcast(timings);

    return ! mEntityEngine.isPaused();
}

} // namespace shmurp
} // namespace ad
