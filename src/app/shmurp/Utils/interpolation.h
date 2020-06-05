#pragma once

#include <algorithm>

// TODO move to a math lib
namespace ad {

// TODO revisit parameters semantic if used with more than arithmetic types
template <class T_val>
T_val clamp(T_val aValue, T_val aMin, T_val aMax)
{
    return std::min(std::max(aMin, aValue), aMax);
}


// Note: Does not work with floating point, which cannot be a non-type template parameter
//template<class T_val, T_val aMin=0, T_val aMax=1>
//class Clamped
//{
//public:
//    constexpr static T_val min_v{aMin};
//    constexpr static T_val max_v{aMax};
//
//    /*implicit*/ Clamped(T_val aValue) :
//        mValue(clamp(std::move(aValue), min_v, max_v))
//    {}
//
//    /*implicit*/ operator const T_val &() const
//    {
//        return mValue;
//    }
//
//private:
//    T_val mValue;
//};

template<class T_val>
class Clamped
{
public:
    typedef T_val value_t;
    constexpr static value_t min_v{0};
    constexpr static value_t max_v{1};

    /*implicit*/ Clamped(value_t aValue) :
        mValue(clamp(std::move(aValue), min_v, max_v))
    {}

    /*implicit*/ operator const value_t &() const
    {
        return mValue;
    }

    const value_t & getValue() const
    {
        return mValue;
    }

private:
    value_t mValue;
};

// Expects T_interpolant to be a floating poiht type
// TODO revisit parameters semantic if used with more than arithmetic types
template <class T_value, class T_interpolant>
T_value lerp(T_value aStart, T_value aEnd, Clamped<T_interpolant> aInterpolant)
{
    return (Clamped<T_interpolant>::max_v - aInterpolant) * aStart
           + aInterpolant * aEnd;
}

namespace animate {

template <class T_interpolant>
Clamped<T_interpolant> linear(Clamped<T_interpolant> aAdvancement)
{
    return aAdvancement;
}

} // namespace animate

template <class T_interpolant>
using animation_fun = Clamped<T_interpolant>(*)(Clamped<T_interpolant>);

template <class T_interpolant, animation_fun<T_interpolant> F_curve = &animate::linear>
class Animation
{
public:
    explicit Animation(T_interpolant aDuration) :
        mFactor(std::move(1/aDuration))
    {}

    Clamped<T_interpolant> increment(T_interpolant aIncrement)
    {
        return (*F_curve)(mFactor * (mAccumulator += aIncrement));
    }

    void reset()
    {
        mAccumulator = 0;
    }

private:
    T_interpolant mFactor;
    T_interpolant mAccumulator{0};
};

// TODO make the lower level class without redirect(), for the usual case when we can do without
// saving the current value
template <class T_value, class T_interpolant, animation_fun<T_interpolant> F_curve = &animate::linear>
class Interpolation
{
public:
    Interpolation(T_value aStart, T_value aEnd, T_interpolant aDuration = 1) :
        mStart(std::move(aStart)),
        mEnd(std::move(aEnd)),
        mCurrent(mStart),
        mAnimation(aDuration)
    {}

    T_value operator()(T_interpolant aIncrement)
    {
        return mCurrent = lerp(mStart, mEnd, mAnimation.increment(aIncrement));
    }

    Interpolation & redirect(T_value aEnd, T_interpolant aDuration)
    {
        mAnimation = Animation(std::move(aDuration));
        mStart = mCurrent;
        mEnd = std::move(aEnd);
        return *this;
    }

    Interpolation & redirect(T_value aEnd)
    {
        mAnimation.reset();
        mStart = mCurrent;
        mEnd = std::move(aEnd);
        return *this;
    }

private:
    T_value mStart;
    T_value mEnd;
    T_value mCurrent;
    Animation<T_interpolant, F_curve> mAnimation;
};


} // namespace ad
