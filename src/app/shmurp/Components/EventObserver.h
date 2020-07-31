#pragma once

#include <aunteater/Component.h>

#include <functional>


namespace ad {


// Note: Events are currently implemented as standalone entities (with the corresonding Event component)
// Because of this, the entity_id is for the event entity, which has little interest
// We cannot attach the Event component to the source entity which semantically emitted it:
// there can be only a single component of a given type on a given entity,
// and several events of the same type might be emitted in the same frame by the same entity
template <class T_event>
struct EventObserver : public aunteater::Component<EventObserver<T_event>>
{
    using handler_fun =
        std::function<void(aunteater::entity_id, const T_event &, aunteater::LiveEntity &)>;

    EventObserver(handler_fun aHandler) :
        handle{std::move(aHandler)}
    {}

    std::function<void(aunteater::entity_id, const T_event &, aunteater::LiveEntity &)> handle;
};


} // namespace ad
