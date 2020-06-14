#include "Customizer.h"

namespace ad {


Customizer::Customizer(aunteater::Engine &aEngine) :
    mCustoms{aEngine},
    mEngine{aEngine}
{}


void Customizer::update(const aunteater::Timer aTimer)
{
    for(aunteater::weak_entity entity : mCustoms)
    {
        std::invoke(entity->get<CustomCallback>().callback, *entity, aTimer, mEngine);
    }
}


} // namespace ad
