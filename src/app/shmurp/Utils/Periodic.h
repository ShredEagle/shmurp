#pragma once

#include "../commons.h"

#include <utility>

#include <cstddef>


namespace ad {


/// TODO make it std::duration based
// \brief A periodic tick, notably allowing to trigger events at regulare interval
// \important The first tick happens instantly (not waiting one period)
class Periodic
{
public:
    explicit Periodic(duration_t aPeriod) :
        mPeriod(aPeriod)
    {}

    constexpr std::size_t countEvents(duration_t aDelta);

    template <class T_functor, class... VT_args>
    void forEachEvent(duration_t aDelta, const T_functor & aFunctor, VT_args &&... aArgs);

private:
    const duration_t mPeriod;
    duration_t mRemainingTime{0.};
};


constexpr std::size_t Periodic::countEvents(duration_t aDelta)
{
    std::size_t result = 0;
    mRemainingTime += aDelta;

    /// TODO once integral, go modulo
    while(mRemainingTime >= 0.)
    {
        mRemainingTime -= mPeriod;
        ++result;
    }
    return result;
}


template <class T_functor, class... VT_args>
void Periodic::forEachEvent(duration_t aDelta, const T_functor & aFunctor, VT_args &&... aArgs)
{
    mRemainingTime += aDelta;

    while(mRemainingTime >= 0.)
    {
        aFunctor(mRemainingTime, std::forward<VT_args>(aArgs)...);
        mRemainingTime -= mPeriod;
    }
}

} // namespace ad
