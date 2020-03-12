
#pragma once

#include <aunteater/Component.h>

namespace ad {

struct ComposedEntity : public aunteater::Component<HitPoints>
{
    static const short BASE_HP{1};

    HitPoints(short aHp = BASE_HP) :
      hp(aHp)
    {}

    short hp{0};
};



} // namespace ad
