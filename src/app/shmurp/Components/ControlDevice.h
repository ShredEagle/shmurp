#pragma once

#include "../commons.h"

#include <aunteater/Component.h>

namespace ad {

struct ControlDevice : public aunteater::Component<ControlDevice>
{
    ControlDevice(unsigned aId) :
        id(aId)
    {}

    unsigned id;
};

} // namespace ad
