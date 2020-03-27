#pragma once

#include "commons.h"
#include "DebugUI.h"

#include <aunteater/Engine.h>

#include <engine/Application.h>
#include <engine/Timer.h>


namespace ad {
namespace shmurp {

class Game
{
public:
    Game(Application & aApplication);

    /// \return True if an the simulation was updated
    /// (i.e. a new frame was generated, and buffers should be swapped)
    bool update(const Timer & aTimer);

private:
    aunteater::Engine mEntityEngine;
    DebugUI mUI;
};


}} // namespace ad::shmurp
