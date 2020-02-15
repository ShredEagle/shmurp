#include "Game.h"

#include "configuration.h"

#include "Components/ControlDevice.h"
#include "Components/Faction.h"
#include "Components/Geometry.h"
#include "Components/Shape.h"

#include "System/Collision.h"
#include "System/Displace.h"
#include "System/EnemySpawn.h"
#include "System/KeyboardControl.h"
#include "System/Rendering.h"

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

    mEntityEngine.addSystem<Collision>();

    mEntityEngine.addSystem<EnemySpawn>();

    mEntityEngine.addSystem<Rendering>();


    /*
     * Entities
     */
    mEntityEngine.addEntity(Entity().add<ControlDevice>(0)
                                    .add<Faction>(Faction::SpaceForce, Faction::Democrats)
                                    .add<Geometry>(conf::shipInitialX, conf::shipInitialY, conf::shipRadius)
                                    .add<Shape>(Shape::RocketShip)
                                    .add<Speed>(0., 0.)
                                    );
}

void Game::update(const Timer & aTimer)
{
    mEntityEngine.update(aTimer.delta());
}

} // namespace shmurp
} // namespace ad
