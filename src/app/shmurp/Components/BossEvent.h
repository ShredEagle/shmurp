#pragma once

#include <aunteater/Component.h>


namespace ad {


struct BossEvent : public aunteater::Component<BossEvent>
{
    enum Phase
    {
        None,
        LaserOn,
        LaserOff,
        Rotate,
        Stabilize,
    };

    BossEvent(/*aunteater::entity_id aSource, */Phase aPhase) :
        //source{aSource},
        phase{aPhase}
    {}

    //aunteater::entity_id source;
    Phase phase{None};
};



} // namespace ad
