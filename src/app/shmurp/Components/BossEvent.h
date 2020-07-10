#pragma once

#include <aunteater/Component.h>


namespace ad {


struct BossEvent : public aunteater::Component<BossEvent>
{
    enum Phase
    {
        None,

        Arc,
        CoreCanon_1,
        CoreCanonOff,
        LaserOn,
        LaserOff,
        LeftCompoundFire,
        RightCompoundFire,
        TopCompoundFire,
        Rotate,
        Stabilize,
        Turret_1,
        TurretOff,
    };

    BossEvent(aunteater::entity_id aSource, Phase aPhase) :
        source{aSource},
        phase{aPhase}
    {}

    aunteater::entity_id source;
    Phase phase{None};
};



} // namespace ad
