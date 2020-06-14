#pragma once

#include <aunteater/Component.h>
#include <aunteater/Engine.h>
#include <aunteater/Timer.h>

namespace ad {

struct CustomCallback : public aunteater::Component<CustomCallback>
{
    using function_type = std::function<void(aunteater::LiveEntity &, const aunteater::Timer &, aunteater::Engine &)>;

    explicit CustomCallback(function_type aFunction) :
        callback{std::move(aFunction)}
    {}

    function_type callback;
};



} // namespace ad
