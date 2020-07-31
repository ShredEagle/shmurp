#pragma once

#include "../commons.h"

#include <functional>
#include <map>


namespace ad {


template <class... VT_functorArgs>
class TimedSequence
{
public:
    using Functor = std::function<void(/*timepoint_t aMoment, */VT_functorArgs...)>;
    using sequence_t = std::vector<std::pair<timepoint_t, Functor>>;

    explicit TimedSequence(sequence_t aSequence) :
        mSequence{std::move(aSequence)}
    {}

    void elapse(duration_t aDelta, VT_functorArgs... aArgs);

    void setRepeat(bool aRepeating);

protected:
    typename sequence_t::const_iterator getIterator() const
    {
        return mSequence.begin() + mEventPosition;
    }

private:
    sequence_t mSequence;
    timepoint_t mCurrentTime{0}; // Wall time starting at Timer construction
    // Bad idea: the default copy leads to UB, because it points into the original object
    //typename sequence_t::const_iterator mEventIt{mSequence.cbegin()};
    typename sequence_t::size_type mEventPosition{0};
    bool mRepeat{false};
};


template <class... VT_functorArgs>
void TimedSequence<VT_functorArgs...>::elapse(duration_t aDelta, VT_functorArgs... aArgs)
{
    mCurrentTime += aDelta;

    while (getIterator() != mSequence.cend()
        && getIterator()->first <= mCurrentTime)
    {
        auto eventTime = getIterator()->first;
        getIterator()->second(std::forward<VT_functorArgs>(aArgs)...);
        if (++mEventPosition == mSequence.size() && mRepeat)
        {
            mCurrentTime -= eventTime;
            mEventPosition = 0;
        }
    }

}

template <class... VT_functorArgs>
void TimedSequence<VT_functorArgs...>::setRepeat(bool aRepeating)
{
    mRepeat = aRepeating;
}

} // namespace ad
