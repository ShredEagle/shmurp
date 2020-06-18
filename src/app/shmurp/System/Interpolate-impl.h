namespace ad {


template <class T_component, class T_value>
Interpolate<T_component, T_value>::Interpolate(aunteater::Engine &aEngine) :
    mAnimateds{aEngine},
    mEngine{aEngine}
{}


template <class T_component, class T_value>
void Interpolate<T_component, T_value>::update(const aunteater::Timer aTimer)
{
    for(auto it = mAnimateds.begin(); it != mAnimateds.end(); /*in-body*/)
    {
        auto current = it++;
        auto & [component, tweening] = current;

        // Interpolate the value
        std::invoke(tweening.accessor, component) =
            tweening.interpolation(std::invoke(tweening.accessor, component), aTimer.delta());

        // On completion, execute completion callback and remove tweening component
        if (tweening.interpolation.isComplete())
        {
            // Remove before calling completion handler, because completion handler might install
            // another tweening (and it should not be removed directly afterward)
            // This then require copying the tweening completion handler, before the component is disposed
            // As well as accessing "current" before it is invalidated
            auto completionCopy = tweening.onCompletion;
            auto & entity = static_cast<aunteater::LiveEntity &>(current);

            current->template remove<tweening_type>();
            completionCopy(entity, aTimer, mEngine);
        }
    }
}


} // namespace ad
