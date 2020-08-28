#pragma once

#include "commons.h"
#include "DebugUI.h"
#include "EventQueue.h"

#include <aunteater/Engine.h>
#include <aunteater/Timer.h>

#include <engine/Application.h>


namespace ad {
namespace shmurp {


namespace event {

    struct Impact
    {
        int damage{0};
    };

} // namespace event

class Game
{
public:
    Game(Application & aApplication);

    /// \return True if an the simulation was updated
    /// (i.e. a new frame was generated, and buffers should be swapped)
    bool update(const aunteater::Timer & aTimer);

private:
    void setBaseSystems(aunteater::Engine & aEntityEngine, Application & aApplication);

    aunteater::Engine mEntityEngine;
    EventQueue<event::Impact> mImpactEvents;
    DebugUI mUI;
};


}} // namespace ad::shmurp
