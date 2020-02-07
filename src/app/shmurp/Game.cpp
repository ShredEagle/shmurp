#include "Game.h"

#include "configuration.h"

#include "Components/ControlDevice.h"
#include "Components/Position.h"
#include "Components/Shape.h"

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

    mEntityEngine.addSystem<Rendering>();

    mEntityEngine.addSystem<Displace>();

    mEntityEngine.addSystem<EnemySpawn>();

    /*
     * Entities
     */
    mEntityEngine.addEntity(Entity().add<ControlDevice>(0)
                                    .add<Position>(conf::shipInitialX, conf::shipInitialY)
                                    .add<Shape>(Shape::RocketShip));
}

void Game::update(const Timer & aTimer)
{
    mEntityEngine.update(aTimer.delta());
}

} // namespace shmurp
} // namespace ad
