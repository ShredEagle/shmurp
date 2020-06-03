#pragma once

#include "../commons.h"
#include "../Utils/interpolation.h"

#include <aunteater/Component.h>

#include <functional>


namespace ad {


// Note: this is a dynamic solution, yet the nature of aunteater make it impossible to attach
// several component of the same type to an entity.
// This prevents to simultaneously tween different values of the same type under the same component
template <class T_component, class T_value>
struct Tweening : public aunteater::Component<Tweening<T_component, T_value>>
{
    using Accessor = std::function<T_value &(T_component &)>;

    Tweening(Accessor aAccessor, T_value aStart, T_value aEnd, Floating aDuration) :
        accessor{std::move(aAccessor)},
        interpolation{aStart, aEnd, aDuration}
    {}

    Accessor accessor;
    Interpolation<T_value, Floating> interpolation;
};


template <class T_component, class T_entity, class T_value>
T_entity & setupTweening(T_entity & aEntity,
                         typename Tweening<T_component, T_value>::Accessor aAccessor,
                         T_value aTarget,
                         Floating aAnimationDuration)
{
    aEntity.template add<Tweening<T_component, T_value>>(
        aAccessor,
        std::invoke(aAccessor, aEntity.template get<T_component>()),
        std::move(aTarget),
        aAnimationDuration);

    return aEntity;
}


//template <class T_component, class T_value, T_value &(* F_accessor)(T_component)>
//struct Tweening : public aunteater::Component<Tweening<T_component, T_value, F_accessor>>
//{
//    using Accessor = std::function<T_value &(T_component &)>;
//
//    Tweening(T_value aStart, T_value aEnd, Floating aDuration) :
//        interpolation{aStart, aEnd, aDuration}
//    {}
//
//    Interpolation<T_value, Floating> interpolation;
//};
//
//
//// Problem: how to just give the function pointer, without having to spell other types expicilty?
//template <class T_component, class T_entity, class T_value, T_value &(* F_accessor)(T_component)>
//T_entity & setupTweening(T_entity & aEntity,
//                         typename Tweening<T_component, T_value>::Accessor aAccessor,
//                         T_value aTarget,
//                         Floating aAnimationDuration)
//{
//    aEntity.template add<Tweening<T_component, T_value>>(
//        aAccessor,
//        std::invoke(aAccessor, aEntity.template get<T_component>()),
//        std::move(aTarget),
//        aAnimationDuration);
//
//    return aEntity;
//}


} // namespace ad
