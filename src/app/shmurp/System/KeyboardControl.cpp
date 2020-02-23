#include "KeyboardControl.h"

#include "../configuration.h"
#include "../transformations.h"

#include <Components/ControlDevice.h>
#include <Components/Faction.h>
#include <Components/Geometry.h>
#include <Components/Shape.h>
#include <Components/Speed.h>

#include <handy/random.h>

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
            mFiring = true;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        mDirection &= ~mapKey(key);
        if (key == GLFW_KEY_SPACE)
        {
            mFiring = false;
        }
    }
}

KeyboardControl::KeyboardControl(aunteater::Engine &aEngine) :
    mPlayerMovable(aEngine.getFamily<PlayerMovable>()),
    mEngine(aEngine)
{}

void spawnBullet(timet aRemainingTime, aunteater::Engine & aEngine, Vec<2, GLfloat> aBasePosition)
{
    static constexpr Vec<4, GLfloat> gSpeed(0.f, conf::gBulletSpeed, 0.f, 1.f);
    static constexpr GLfloat gAngleQuant(pi<GLfloat>/180/4);
    static Randomizer randFactor(-30, 30);

    using aunteater::Entity;
    auto speed = gSpeed * transform::rotateMatrix(gAngleQuant*randFactor());
    Vec<2, GLfloat> startPosition = aBasePosition
                                    + static_cast<GLfloat>(aRemainingTime)*Vec<2, GLfloat>{speed.x(), speed.y()};
    aEngine.addEntity(Entity().add<Faction>(Faction::TruthBullet, Faction::Democrats)
                              .add<Geometry>(startPosition, conf::gBulletRadius)
                              .add<Shape>(Shape::Circle)
                              .add<Speed>(speed.x(), speed.y()));
}

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

        movable->get<Speed>().speed = mSpeedInterpolation(time);

        if (mCallback->mFiring)
        {
            mBulletPeriod.forEachEvent(time,
                                       spawnBullet,
                                       mEngine,
                                       movable->get<Geometry>().position);
        }
    }

}

} // namespace ad
