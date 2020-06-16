namespace ad {


template <class T_component, class T_value>
Interpolate<T_component, T_value>::Interpolate(aunteater::Engine &aEngine) :
    mAnimateds{aEngine}
{}


template <class T_component, class T_value>
void Interpolate<T_component, T_value>::update(const aunteater::Timer aTimer)
{
    for(auto & [component, tweening] : mAnimateds)
    {
        std::invoke(tweening.accessor, component) =
            tweening.interpolation(std::invoke(tweening.accessor, component), aTimer.delta());
    }
}


} // namespace ad
