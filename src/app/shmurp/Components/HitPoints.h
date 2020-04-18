#pragma once

#include <aunteater/Component.h>

namespace ad {

struct HitPoints : public aunteater::Component<HitPoints>
{
    static const short BASE_HP{1};

    HitPoints(short aHp = BASE_HP) :
      hp(aHp),
      totalHp(aHp)
    {}

    short hp;
    short totalHp;
};



} // namespace ad
