#pragma once

#include "../commons.h"
#include "../configuration.h"
#include "../transformations.h"

#include <Utils/Periodic.h>

#include <Components/Faction.h>
#include <Components/Geometry.h>
#include <Components/Shape.h>
#include <Components/Speed.h>

#include <aunteater/Component.h>
#include <aunteater/Engine.h>

namespace ad {

class FirePattern : public aunteater::Component<FirePattern>
{
public:
    class Base
    {
    public:
        virtual void fire(double aDelta,
                          aunteater::Engine & aEngine,
                          Vec<2, GLfloat> aBasePosition) = 0;

        virtual std::unique_ptr<Base> clone() = 0;
        virtual ~Base()
        {}
    };

    /// TODO unusable with current Engine.add()
    template <class T_base, class... VT_aArgs>
    FirePattern(VT_aArgs &&... aArgs) :
        mImplementation(new T_base(std::forward<VT_aArgs>(aArgs)...))
    {}

    template <class T_base>
    FirePattern(std::unique_ptr<T_base> aImplementation) :
        mImplementation(std::move(aImplementation))
    {}

    FirePattern(const FirePattern & aOther) :
        mImplementation(aOther.mImplementation->clone())
    {}

    void fire(double aDelta,
              aunteater::Engine & aEngine,
              Vec<2, GLfloat> aBasePosition)
    {
        mImplementation->fire(aDelta, aEngine, aBasePosition);
    }

private:
    std::unique_ptr<Base> mImplementation;
};

namespace Fire {

class Spiral : public FirePattern::Base
{
public:
    Spiral(timet aPeriod, double aAngularSpeed) :
        mPeriod(aPeriod),
        mAngleIncrement(aAngularSpeed*aPeriod)
    {}

    void fire(double aDelta,
              aunteater::Engine & aEngine,
              Vec<2, GLfloat> aBasePosition) override
    {
        mPeriod.forEachEvent(aDelta, [&, this](timet aRemainingTime)
        {
            static constexpr Vec<4, GLfloat> gSpeed(0.f, -conf::gEnemyBulletSpeed, 0.f, 1.f);

            using aunteater::Entity;
            auto speed = gSpeed * transform::rotateMatrix(nextAngle());
            Vec<2, GLfloat> startPosition = aBasePosition
                                            + static_cast<GLfloat>(aRemainingTime)*Vec<2, GLfloat>{speed.x(), speed.y()};
            aEngine.addEntity(Entity().add<Faction>(Faction::LibLies,
                                                    Faction::SpaceForce)
                                      .add<Geometry>(startPosition, conf::gBulletRadius)
                                      .add<Shape>(Shape::Circle)
                                      .add<Speed>(speed.x(), speed.y()));
        });
    }

    std::unique_ptr<Base> clone() override
    {
        return std::make_unique<Spiral>(*this);
    }

private:
    float nextAngle()
    {
        return mAngle += mAngleIncrement;
    }

private:
    Periodic mPeriod;
    float mAngle{0};
    const float mAngleIncrement;
};

} // namespace Fire

} // namespace ad
