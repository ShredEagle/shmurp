#include "Customizer.h"

namespace ad {


Customizer::Customizer(aunteater::Engine &aEngine) :
    mCustoms{aEngine}
{}


void Customizer::update(const aunteater::Timer aTimer)
{
    for(aunteater::weak_entity entity : mCustoms)
    {
        entity->get<CustomCallback>().callback(*entity, aTimer);
    }
}


} // namespace ad
