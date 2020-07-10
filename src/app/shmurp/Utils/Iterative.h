#pragma once

#include "../commons.h"

#include <utility>

#include <cstddef>


namespace ad {


template <class T_timer>
class Iterative
{
public:
    template <class... VT_timerCtorArgs>
    Iterative(int aIterations, VT_timerCtorArgs &&... aTimerCtorArgs) :
        mIterations{aIterations},
        mTimer{std::forward<VT_timerCtorArgs>(aTimerCtorArgs)...}
    {}

    template <class T_functor, class... VT_args>
    void forEachEvent(duration_t aDelta, T_functor && aFunctor, VT_args &&... aArgs);

private:
    int mIterations;
    T_timer mTimer;
};

template <class T_timer>
template <class T_functor, class... VT_args>
void Iterative<T_timer>::forEachEvent(duration_t aDelta, T_functor && aFunctor, VT_args &&... aArgs)
{
    mTimer.forEachEvent(
            aDelta,
            [this, functor = std::forward<T_functor>(aFunctor)]
                (duration_t aRemainingTime, VT_args &&... aArgs)
                {
                    for (int iteration = 0; iteration != mIterations; ++iteration)
                    {
                        functor(aRemainingTime, iteration, std::forward<VT_args>(aArgs)...);
                    }
                });
}

} // namespace ad
