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

typedef aunteater::Archetype<ControlDevice, ControlDevice> PlayerMovable;

KeyboardControl::KeyboardControl(aunteater::Engine &aEngine) :
    mPlayerMovable(aEngine.getFamily<PlayerMovable>()),
    mEngine(aEngine)
{}

void spawnBullet(aunteater::Engine & aEngine, Vec<2, GLfloat> aBasePosition)
{
    static constexpr GLfloat gMaxAngle(pi/25);
    static constexpr Vec<4, GLfloat> gSpeed(0.f, 15.f, 0.f, 1.f);
    // TODO does not seem well distributed, and quid about 0?
    static Randomizer randDenominator(-20, 20);

    using aunteater::Entity;
    auto speed = gSpeed * transform::rotateMatrix(gMaxAngle/randDenominator());
    aEngine.addEntity(Entity().add<Faction>(Faction::TruthBullet, Faction::Democrats)
                              .add<Geometry>(aBasePosition.x(),
                                             aBasePosition.y(),
                                             conf::gBulletRadius)
                              .add<Shape>(Shape::Circle)
                              .add<Speed>(speed.x(), speed.y()));
}

void KeyboardControl::update(double time)
{
    for (auto & movable : mPlayerMovable)
    {
        Vec<2, GLfloat> & position = movable->get<Geometry>().position;
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

        if (mCallback->mFiring)
        {
            mBulletPeriod.forEachEvent(time, spawnBullet, mEngine, position);
        }
    }

}

} // namespace ad
