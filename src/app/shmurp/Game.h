#pragma once

#include "commons.h"
#include "DebugUI.h"

#include <aunteater/Engine.h>
#include <aunteater/Timer.h>

#include <engine/Application.h>


namespace ad {
namespace shmurp {

class Game
{
public:
    Game(Application & aApplication);

    /// \return True if an the simulation was updated
    /// (i.e. a new frame was generated, and buffers should be swapped)
    bool update(const aunteater::Timer & aTimer);

private:
    aunteater::Engine mEntityEngine;
    DebugUI mUI;
};


}} // namespace ad::shmurp
