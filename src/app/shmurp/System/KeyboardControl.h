#pragma once

#include "../commons.h"
#include "../configuration.h"

#include "../Utils/interpolation.h"

#include <aunteater/System.h>


namespace ad {

class KeyboardControl : public aunteater::System
{
    enum Direction : short
    {
        None    = 0,
        Up      = (1 << 0),
        Down    = (1 << 1),
        Left    = (1 << 2),
        Up_Left = (Up & Left),
        Down_Left = (Down & Left),
        Right   = (1 << 3),
        Up_Right = (Up & Right),
        Down_Right = (Down & Right),
    };

    enum class Edge : short
    {
        None,
        Press,
        Release,
    };

    struct Callback
    {
        explicit Callback(aunteater::Engine & aEngine) :
            mEngine(aEngine)
        {}

        void operator()(int key, int scancode, int action, int mods);

        aunteater::Engine & mEngine;
        short mDirection{0};
        Edge mFiring{Edge::None};
    };

public:
    KeyboardControl(aunteater::Engine &aEngine);
    void update(double time) override;

    std::shared_ptr<Callback> getCallback();

private:
    std::shared_ptr<Callback> mCallback;
    aunteater::Family & mPlayerMovable;
    Vec<2, GLfloat> mTargetSpeed{0.f, 0.f};
    Interpolation<Vec<2, GLfloat>, GLfloat> mSpeedInterpolation{mTargetSpeed,
                                                                mTargetSpeed,
                                                                conf::gShipAccelerationFactor};
};

inline std::shared_ptr<KeyboardControl::Callback> KeyboardControl::getCallback()
{
    return mCallback;
}

} // namespace ad
