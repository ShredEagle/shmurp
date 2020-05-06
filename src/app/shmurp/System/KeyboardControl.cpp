#include "KeyboardControl.h"

#include "../configuration.h"
#include "../transformations.h"

#include <Components/ControlDevice.h>
#include <Components/FirePattern.h>

#include <GLFW/glfw3.h>

#include <iostream>

namespace ad {

typedef aunteater::Archetype<ControlDevice, ControlDevice, Geometry, Speed> PlayerMovable;

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
        if (key == GLFW_KEY_SPACE)
        {
            mFiring = Edge::Press;
        }
        if (key == GLFW_KEY_P)
        {
            mEngine.pause(!mEngine.isPaused());
        }
    }
    else if (action == GLFW_RELEASE)
    {
        mDirection &= ~mapKey(key);
        if (key == GLFW_KEY_SPACE)
        {
            mFiring = Edge::Release;
        }
    }
}

KeyboardControl::KeyboardControl(aunteater::Engine &aEngine) :
    mCallback{std::make_shared<Callback>(aEngine)},
    mPlayerMovable{aEngine.getFamily<PlayerMovable>()}
{}

void KeyboardControl::update(double time)
{
    for (auto & movable : mPlayerMovable)
    {
        auto newSpeed = Vec<2, GLfloat>::Zero();

        if (mCallback->mDirection & Up)
        {
            newSpeed.y() = conf::gShipSpeed;
        }
        if (mCallback->mDirection & Down)
        {
            newSpeed.y() = -conf::gShipSpeed;
        }
        if (mCallback->mDirection & Left)
        {
            newSpeed.x() = -conf::gShipSpeed;
        }
        if (mCallback->mDirection & Right)
        {
            newSpeed.x() = conf::gShipSpeed;
        }

        if (newSpeed != mTargetSpeed)
        {
            mTargetSpeed = newSpeed;
            mSpeedInterpolation.redirect(mTargetSpeed);
        }

        movable->get<Speed>().translation = mSpeedInterpolation(time);

        switch(mCallback->mFiring)
        {
            case Edge::Press:
                movable->add<FirePattern>(std::make_unique<Fire::Burst>(0.02f, pi<GLfloat>/180.f*7.5f));
                mCallback->mFiring = Edge::None;
                break;
            case Edge::Release:
                movable->removeComponent<FirePattern>();
                mCallback->mFiring = Edge::None;
                break;
            default: break;
        }
    }
}

} // namespace ad
