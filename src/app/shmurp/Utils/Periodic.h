#pragma once

#include <utility>

#include <cstddef>


namespace ad {

typedef double timet;

/// TODO make it std::duration based
class Periodic
{
public:
    explicit Periodic(timet aPeriod) :
        mPeriod(aPeriod)
    {}

    constexpr std::size_t countEvents(timet aDelta);

    template <class T_functor, class... VT_args>
    void forEachEvent(timet aDelta, const T_functor & aFunctor, VT_args &&... aArgs);

private:
    const float mPeriod;
    float mRemainingTime{0.};
};


constexpr std::size_t Periodic::countEvents(timet aDelta)
{
    std::size_t result=0;
    mRemainingTime += aDelta;

    /// TODO once integral, go modulo
    while((mRemainingTime - mPeriod) >= 0.)
    {
        mRemainingTime -= mPeriod;
        ++result;
    }
    return result;
}

template <class T_functor, class... VT_args>
void Periodic::forEachEvent(timet aDelta, const T_functor & aFunctor, VT_args &&... aArgs)
{
    mRemainingTime += aDelta;

    while((mRemainingTime - mPeriod) >= 0.)
    {
        mRemainingTime -= mPeriod;
        aFunctor(mRemainingTime, std::forward<VT_args>(aArgs)...);
    }
}

} // namespace ad
