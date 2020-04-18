#include "Game.h"

#include "configuration.h"

#include "Components/Boundaries.h"
#include "Components/ControlDevice.h"
#include "Components/Faction.h"
#include "Components/Geometry.h"
#include "Components/Shape.h"
#include "Components/HitPoints.h"

#include "System/BulletSpawn.h"
#include "System/Collision.h"
#include "System/Displace.h"
#include "System/EnemySpawn.h"
#include "System/KeyboardControl.h"
#include "System/LimitPosition.h"
#include "System/Rendering.h"
#include "System/Rendering3D.h"

#include <aunteater/UpdateTiming.h>

using aunteater::Entity;

namespace ad {
namespace shmurp {

Game::Game(Application & aApplication)
{
    /*
     * Systems
     */
    auto kbControl = std::make_shared<KeyboardControl>(mEntityEngine);
    aApplication.registerKeyCallback(kbControl->getCallback());
    mEntityEngine.addSystem(kbControl);

    mEntityEngine.addSystem<Displace>();

    mEntityEngine.addSystem<LimitPosition>();

    mEntityEngine.addSystem<Collision>();

    mEntityEngine.addSystem<EnemySpawn>();

    mEntityEngine.addSystem<BulletSpawn>();

    //mEntityEngine.addSystem<Rendering>();
    mEntityEngine.addSystem<Rendering3D>(aApplication.mEngine->getFramebufferSize());


    /*
     * Entities
     */
    mEntityEngine.addEntity(Entity()
            .add<Boundaries>(ad::conf::gShipBoundingRect)
            .add<ControlDevice>(0)
            .add<FactionSpaceForce>()
            .add<Geometry>(conf::shipInitialX, conf::shipInitialY, conf::gShipRadius)
            .add<HitPoints>(conf::gSpaceForceHitPoints)
            .add<Shape>(Shape::RocketShip)
            .add<Speed>(0., 0.)
            );

    //mEntityEngine.addEntity(Entity().add<FirePattern>(std::make_unique<Fire::Spiral>(0.05f, pi<float>))
    //                                .add<Geometry>(5.f, conf::gWindowWorldHeight-5.f, conf::squareRadius)
    //                                .add<Shape>(Shape::RocketShip)
    //                                .add<Speed>(0., 0.)
    //                                );
    //mEntityEngine.addEntity(Entity().add<FirePattern>(std::make_unique<Fire::Spiral>(0.05f, pi<float>))
    //                                .add<Geometry>(conf::gWindowWorldWidth-5.f, conf::gWindowWorldHeight-5.f, conf::squareRadius)
    //                                .add<Shape>(Shape::RocketShip)
    //                                .add<Speed>(0., 0.)
    //                                );
    //mEntityEngine.addEntity(Entity().add<FirePattern>(std::make_unique<Fire::Circle>(0.5f, 25))
    //                                .add<Geometry>(conf::gWindowWorldWidth/2.f, conf::gWindowWorldHeight-5.f, conf::squareRadius)
    //                                .add<Shape>(Shape::RocketShip)
    //                                .add<Speed>(0., 0.)
    //                                );
}

bool Game::update(const Timer & aTimer)
{
    aunteater::UpdateTiming timings;
    mEntityEngine.update(aTimer.delta(), timings);

    mUI.broadcast(timings);

    return ! mEntityEngine.isPaused();
}

} // namespace shmurp
} // namespace ad
