#pragma once

#include "Periodic.h"

#include <gsl/gsl>

#include <algorithm>
#include <vector>


namespace ad {


class Rythm
{
public:
    explicit Rythm(duration_t aPeriod, int aDuration) :
        mMetronom(aPeriod),
        mDuration(aDuration)
    {}

    Rythm & note(int aTime);

    template <class T_functor, class... VT_args>
    void forEachEvent(duration_t aDelta, const T_functor & aFunctor, VT_args &&... aArgs);

private:
    Periodic mMetronom;
    int mDuration;
    std::vector<int> mScore{};
    int mCurrentNote{0};
};


inline Rythm & Rythm::note(int aTime)
{
    Expects( (aTime >= 0) && (aTime < mDuration) );
    mScore.push_back(aTime);
    return *this;
}


template <class T_functor, class... VT_args>
void Rythm::forEachEvent(duration_t aDelta, const T_functor & aFunctor, VT_args &&... aArgs)
{
    mMetronom.forEachEvent(aDelta, [&aFunctor, this](duration_t aRemainingTime, VT_args &&... aArgs)
    {
        if (std::find(mScore.begin(), mScore.end(), mCurrentNote) != mScore.end())
        {
            aFunctor(aRemainingTime, std::forward<VT_args>(aArgs)...);
        }
        if (++mCurrentNote == mDuration)
        {
            mCurrentNote = 0;
        };
    }, std::forward<VT_args>(aArgs)...);
}


} // namespace ad
