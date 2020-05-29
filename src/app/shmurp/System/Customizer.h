#pragma once

#include <Components/CustomCallback.h>

#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>


namespace ad {


using Custom = aunteater::Archetype<CustomCallback>;

class Customizer : public aunteater::System
{

public:
    Customizer(aunteater::Engine &aEngine);

    void update(const aunteater::Timer aTimer) override;

private:
    const aunteater::FamilyHelp<Custom> mCustoms;
};


} // namespace ad
