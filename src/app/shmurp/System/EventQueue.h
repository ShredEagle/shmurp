#pragma once

#include <Components/EventObserver.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {


template <class T_event>
class EventQueue : public aunteater::System
{
public:

    using Event = aunteater::Archetype<T_event>;
    using Observer = aunteater::Archetype<EventObserver<T_event>>;

    EventQueue(aunteater::Engine &aEngine);

    void update(const aunteater::Timer aTimer) override;

private:
    const aunteater::FamilyHelp<Event> mEvents;
    const aunteater::FamilyHelp<Observer> mObservers;
};


} // namespace ad


#include "EventQueue-impl.h"
