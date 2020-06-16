#pragma once

#include <Components/Tweening.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {


template <class T_component, class T_value>
using Animated = aunteater::Archetype<T_component, LiveTweening<T_component, T_value>>;


template <class T_component, class T_value>
class Interpolate : public aunteater::System
{
public:
    Interpolate(aunteater::Engine &aEngine);

    void update(const aunteater::Timer aTimer) override;

private:
    const aunteater::FamilyHelp<Animated<T_component, T_value>> mAnimateds;

};


} // namespace ad


#include "Interpolate-impl.h"
