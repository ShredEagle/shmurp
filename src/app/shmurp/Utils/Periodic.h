#pragma once

#include <cstddef>


namespace ad {

typedef double timet;

/// TODO make it std::duration based
class Periodic
{
public:
    Periodic(timet aPeriod) :
        mPeriod(aPeriod)
    {}

    constexpr std::size_t countEvents(timet aDelta);


private:
    const float mPeriod;
    float mTimeSinceLastEvent{0.};
};


constexpr std::size_t Periodic::countEvents(timet aDelta)
{
    std::size_t result=0;
    mTimeSinceLastEvent += aDelta;

    /// TODO once integral, go modulo
    while((mTimeSinceLastEvent - mPeriod) >= 0.)
    {
        mTimeSinceLastEvent -= mPeriod;
        ++result;
    }
    return result;
}


} // namespace ad
