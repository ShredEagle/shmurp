#pragma once

#include "../commons.h"

#include <Utils/Periodic.h>

#include <aunteater/Component.h>

namespace ad {

struct FirePattern : public aunteater::Component<FirePattern>
{
    FirePattern(timet aPeriod, double aAngularSpeed) :
        period(aPeriod),
        mAngleIncrement(aAngularSpeed*aPeriod)
    {}

    float nextAngle()
    {
        return mAngle += mAngleIncrement;
    }

    Periodic period;



private:
    float mAngle{0};
    const float mAngleIncrement;
};



} // namespace ad
