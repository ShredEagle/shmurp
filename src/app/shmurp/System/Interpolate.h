#pragma once

#include <Components/Tweening.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {

template <class T_component, class T_value>
class Interpolate;

template <class T_component, class T_value>
using Animated = aunteater::Archetype<T_component,
                                      typename Interpolate<T_component, T_value>::tweening_type>;


template <class T_component, class T_value>
class Interpolate : public aunteater::System
{
public:
    using tweening_type = LiveTweening<T_component, T_value>;

    Interpolate(aunteater::Engine &aEngine);

    void update(const aunteater::Timer aTimer) override;

private:
    const aunteater::FamilyHelp<Animated<T_component, T_value>> mAnimateds;
    aunteater::Engine & mEngine;
};


} // namespace ad


#include "Interpolate-impl.h"
