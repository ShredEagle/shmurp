#include "KeyboardControl.h"

#include "Components/ControlDevice.h"
#include "Components/Position.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace ad {

void KeyboardControl::Callback::operator()(int key, int scancode, int action, int mods)
{
    auto mapKey = [](int aKey) -> Direction
    {
        switch(aKey)
        {
            default: return None;
            case GLFW_KEY_UP: return Up;
            case GLFW_KEY_DOWN: return Down;
            case GLFW_KEY_LEFT: return Left;
            case GLFW_KEY_RIGHT: return Right;
        }
    };

    if (action == GLFW_PRESS)
    {
        mDirection |= mapKey(key);
    }
    else if (action == GLFW_RELEASE)
    {
        mDirection &= ~mapKey(key);
    }
}

typedef aunteater::Archetype<ControlDevice, ControlDevice> PlayerMovable;

KeyboardControl::KeyboardControl(aunteater::Engine &aEngine) :
    mPlayerMovable(aEngine.getFamily<PlayerMovable>())
{}

void KeyboardControl::update(double time)
{
    for (auto & movable : mPlayerMovable)
    {
        Vec<2, GLfloat> & position = movable->get<Position>().position;
        if (mCallback->mDirection & Up)
        {
            position.y() += 0.5f;
        }
        if (mCallback->mDirection & Down)
        {
            position.y() -= 0.5f;
        }
        if (mCallback->mDirection & Left)
        {
            position.x() -= 0.5f;
        }
        if (mCallback->mDirection & Right)
        {
            position.x() += 0.5f;
        }
    }
}

} // namespace ad
