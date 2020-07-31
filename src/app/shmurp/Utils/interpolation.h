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


/// \brief More general overload, replaces aStart value with a value given at any specified t
/// \note It requires branching to avoid division by zero when previous value was already at the end
template <class T_value, class T_interpolant>
T_value lerp(std::pair<T_value, Clamped<T_interpolant>> aPrevious,
             T_value aEnd,
             Clamped<T_interpolant> aInterpolant)
{
    if (Clamped<T_interpolant>::max_v == aPrevious.second)
    {
        return aEnd;
    }
    // see: https://en.wikipedia.org/wiki/Linear_interpolation
    // Generic formula for y
    return aPrevious.first
           + (aInterpolant - aPrevious.second)
             * ( (aEnd - aPrevious.first) / (Clamped<T_interpolant>::max_v - aPrevious.second) );
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

    Clamped<T_interpolant> current() const
    {
        return (*F_curve)(mFactor * mAccumulator);
    }

    Clamped<T_interpolant> increment(T_interpolant aIncrement)
    {
        return (*F_curve)(mFactor * (mAccumulator += aIncrement));
    }

    void reset()
    {
        mAccumulator = 0;
    }

    bool isComplete() const
    {
        return (mFactor * mAccumulator) >= Clamped<T_interpolant>::max_v;
    }

private:
    T_interpolant mFactor;
    T_interpolant mAccumulator{0};
};


/// \brief Functor interpolating values in the range [aStart, aEnd] given at instanciation,
/// following the curve defined by F_curve.
/// \note The interpolation range is given at construction.
template <class T_value, class T_interpolant, animation_fun<T_interpolant> F_curve = &animate::linear>
class Interpolation
{
public:
    Interpolation(T_value aStart, T_value aEnd, T_interpolant aDuration = 1) :
        mStart{std::move(aStart)},
        mEnd{std::move(aEnd)},
        mAnimation{aDuration}
    {}

    T_value operator()(T_interpolant aIncrement)
    {
        return lerp(mStart, mEnd, mAnimation.increment(aIncrement));
    }

    bool isComplete() const
    {
        return mAnimation.isComplete();
    }

protected:
    T_value mStart;
    T_value mEnd;
    Animation<T_interpolant, F_curve> mAnimation;
};


/// \brief Functor interpolating values between an explicit previous interpolation result,
/// and aEnd given at instanciation. Follows the curve defined by F_curve.
/// \note See Interpolation for a more standard approach.
template <class T_value, class T_interpolant, animation_fun<T_interpolant> F_curve = &animate::linear>
class LiveInterpolation
{
public:
    LiveInterpolation(T_value aEnd, T_interpolant aDuration = 1) :
        mEnd{std::move(aEnd)},
        mAnimation{aDuration}
    {}

    T_value operator()(const T_value & aPreviousResult, T_interpolant aIncrement)
    {
        return lerp(std::make_pair(aPreviousResult, mAnimation.current()),
                    mEnd,
                    mAnimation.increment(aIncrement));
    }

    bool isComplete() const
    {
        return mAnimation.isComplete();
    }

private:
    T_value mEnd;
    Animation<T_interpolant, F_curve> mAnimation;
};


template <class T_value, class T_interpolant, animation_fun<T_interpolant> F_curve = &animate::linear>
class RedirectableInterpolation : public Interpolation<T_value, T_interpolant, F_curve>
{
    using parent_type = Interpolation<T_value, T_interpolant, F_curve>;

public:
    RedirectableInterpolation(T_value aStart, T_value aEnd, T_interpolant aDuration = 1) :
        parent_type{aStart, aEnd, aDuration},
        mCurrent{aStart}
    {}

    RedirectableInterpolation & redirect(T_value aEnd, T_interpolant aDuration)
    {
        this->mAnimation = Animation(std::move(aDuration));
        this->mStart = mCurrent;
        this->mEnd = std::move(aEnd);
        return *this;
    }

    RedirectableInterpolation & redirect(T_value aEnd)
    {
        this->mAnimation.reset();
        this->mStart = mCurrent;
        this->mEnd = std::move(aEnd);
        return *this;
    }

private:
    T_value mCurrent;
};


} // namespace ad
