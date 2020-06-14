#pragma once

#include <aunteater/Component.h>

#include <functional>


namespace ad {


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
