namespace ad {


template <class T_event>
EventQueueS<T_event>::EventQueueS(aunteater::Engine &aEngine) :
    mEvents{aEngine},
    mObservers{aEngine}
{}


template <class T_event>
void EventQueueS<T_event>::update(const aunteater::Timer aTimer)
{
    for(auto & wrapEvent : mEvents)
    {
        const auto & [event] = wrapEvent;
        for (auto & wrapObserver : mObservers)
        {
            const auto & [observer] = wrapObserver;
            std::invoke(observer.handle,
                        aunteater::entityIdFrom(*static_cast<aunteater::weak_entity>(wrapEvent)),
                        event,
                        *static_cast<aunteater::weak_entity>(wrapObserver));
        }
        // TODO this is an example why deferred component removal is needed
        // removig immediatly crashes, because it removes the node from the family, invalidating iterator
        // Remove the event component, it has been consumed by all registered observers
        //wrapEvent->template remove<T_event>();

        // Note: now that events are standalone entites, remove the entity directly
        wrapEvent->markToRemove();
    }

    //// TODO due to immediate removal, we remove in a second loop for the moment
    //for(auto eventIt = mEvents.begin(); eventIt != mEvents.end(); /*in-body*/)
    //{
    //    auto current = eventIt++;
    //    current->template remove<T_event>();
    //}

}


} // namespace ad
