#pragma once

#include <aunteater/System.h>

namespace ad {

class KeyboardControl : public aunteater::System
{
    struct Callback
    {
        void operator()(int key, int scancode, int action, int mods);
    };

public:
    virtual void addedToEngine(aunteater::Engine &aEngine) override;
    virtual void update(double time) override;

    std::shared_ptr<Callback> getCallback();

private:
    std::shared_ptr<Callback> mCallback;
};

inline std::shared_ptr<KeyboardControl::Callback> KeyboardControl::getCallback()
{
    return mCallback;
}

} // namespace ad
