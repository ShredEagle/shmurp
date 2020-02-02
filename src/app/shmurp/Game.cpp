#include "Game.h"

#include "Components/Position.h"

#include "System/KeyboardControl.h"
#include "System/Rendering.h"

using aunteater::Entity;

namespace ad {
namespace shmurp {

Game::Game(Application & aApplication)
{
    auto kbControl = std::make_shared<KeyboardControl>();
    aApplication.registerKeyCallback(kbControl->getCallback());
    mEntityEngine.addSystem(kbControl);
    mEntityEngine.addSystem(std::make_shared<Rendering>());

    mEntityEngine.addEntity(Entity().add<Position>(.5f, .5f));
}

void Game::update(const Timer & aTimer)
{
    mEntityEngine.update(aTimer.delta());
}

} // namespace shmurp
} // namespace ad
