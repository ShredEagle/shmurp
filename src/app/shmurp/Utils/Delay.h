#pragma once

#include "commons.h"

#include <utility>


namespace ad {


template <class T_timer>
class Delay
{
public:
    template <class... VT_timerCtorArgs>
    Delay(duration_t aDelay, VT_timerCtorArgs &&... aTimerCtorArgs) :
        mTimer{std::forward<VT_timerCtorArgs>(aTimerCtorArgs)...}
    {
        // The delay is introduced via a negative "remaining time"
        // This makes the delay wrapper a zero-cost abstraction (appart from linear time at construction)
        // because it does not have any extra-data member, nor any-extra logic on forEachEvent
        mTimer.forEachEvent(-aDelay, [](duration_t /*ignored*/){});
    }

    template <class T_functor, class... VT_args>
    void forEachEvent(duration_t aDelta, T_functor && aFunctor, VT_args &&... aArgs);

private:
    T_timer mTimer;
};


template <class T_timer>
template <class T_functor, class... VT_args>
void Delay<T_timer>::forEachEvent(duration_t aDelta, T_functor && aFunctor, VT_args &&... aArgs)
{
    mTimer.forEachEvent(aDelta, std::forward<T_functor>(aFunctor), std::forward<VT_args>(aArgs)...);
}


} // namespace ad
