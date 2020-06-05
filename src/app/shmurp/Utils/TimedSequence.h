#pragma once

#include "../commons.h"

#include <functional>
#include <map>


namespace ad {


template <class... VT_functorArgs>
class TimedSequence
{
public:
    using Functor = std::function<void(timepoint_t aMoment, VT_functorArgs...)>;
    using sequence_t = std::map<timepoint_t, Functor>;

    explicit TimedSequence(sequence_t aSequence) :
        mSequence{std::move(aSequence)}
    {}

    void elapse(duration_t aDelta, VT_functorArgs... aArgs);

private:
    sequence_t mSequence;
    timepoint_t mCurrentTime{0}; // Wall time starting at Timer construction
};


template <class... VT_functorArgs>
inline void TimedSequence<VT_functorArgs...>::elapse(duration_t aDelta, VT_functorArgs... aArgs)
{
    mCurrentTime += aDelta;

    for(auto eventIt = mSequence.begin();
        eventIt != mSequence.end() && eventIt->first <= mCurrentTime;
        /* in-body */)
    {
        eventIt->second(eventIt->first, std::forward<VT_functorArgs>(aArgs)...);
        eventIt = mSequence.erase(eventIt);
    }
}


//template <class... VT_functorArgs>
//TimedSequence<VT_functorArgs...> make_TimedSequence(std::map<timepoint_t,

} // namespace ad
