#pragma once

#include <aunteater/Component.h>
#include <aunteater/Engine.h>
#include <aunteater/Timer.h>

namespace ad {

// Not undefined at the end of header, because client code might use it!
#define AUNTEATER_CB_ARGS aunteater::LiveEntity & aEntity, const aunteater::Timer & aTimer, aunteater::Engine & aEngine

struct CustomCallback : public aunteater::Component<CustomCallback>
{
    using function_type = std::function<void(AUNTEATER_CB_ARGS)>;
    static void NullCallback(AUNTEATER_CB_ARGS)
    {}

    explicit CustomCallback(function_type aFunction) :
        callback{std::move(aFunction)}
    {}

    function_type callback;
};



} // namespace ad
