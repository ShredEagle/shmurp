#pragma once

#include "../commons.h"
#include "../configuration.h"
#include "../Entities.h"
#include "../transformations.h"

#include <Utils/Periodic.h>


#include <aunteater/Component.h>
#include <aunteater/Engine.h>

namespace ad {

class FirePattern : public aunteater::Component<FirePattern>
{
public:
    class Base_impl
    {
    public:
        virtual void fire(double aDelta,
                          aunteater::Engine & aEngine,
                          Vec<2, GLfloat> aBasePosition) = 0;

        virtual std::unique_ptr<Base_impl> clone() = 0;
        virtual ~Base_impl()
        {}
    };

    template <class T_derived>
    class Base : public Base_impl
    {
    public:
        std::unique_ptr<Base_impl> clone() final
        {
            return std::make_unique<T_derived>(*static_cast<T_derived*>(this));
        }
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
    std::unique_ptr<Base_impl> mImplementation;
};

namespace Fire {

class Spiral : public FirePattern::Base<Spiral>
{
public:
    Spiral(timet aPeriod, double aAngularSpeed) :
        mPeriod{aPeriod},
        mAngleIncrement(aAngularSpeed*aPeriod)
    {}

    void fire(double aDelta,
              aunteater::Engine & aEngine,
              Vec<2, GLfloat> aBasePosition) override
    {
        mPeriod.forEachEvent(aDelta, [&, this](timet aRemainingTime)
        {
            static constexpr Vec<4, GLfloat> gSpeed(0.f, -conf::gEnemyBulletSpeed, 0.f, 1.f);
            auto speed = gSpeed * transform::rotateMatrix(nextAngle());

            Vec<2, GLfloat> startPosition =
                aBasePosition
                + static_cast<GLfloat>(aRemainingTime)*Vec<2, GLfloat>{speed.x(), speed.y()};
            aEngine.addEntity(entities::makeEnemyBullet(startPosition, speed));
        });
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


class Circle : public FirePattern::Base<Circle>
{
public:
    Circle(timet aPeriod, int aCount) :
        mPeriod{aPeriod},
        mCount{aCount}
    {}

    void fire(double aDelta,
              aunteater::Engine & aEngine,
              Vec<2, GLfloat> aBasePosition) override
    {
        mPeriod.forEachEvent(aDelta, [&, this](timet aRemainingTime)
        {
            static constexpr Vec<4, GLfloat> gSpeed(0.f, -conf::gEnemyBulletSpeed, 0.f, 1.f);

            for (int bulletCount = 0; bulletCount != mCount; ++bulletCount)
            {
                auto speed = gSpeed * transform::rotateMatrix(bulletCount * 2*pi<GLfloat>/mCount);
                Vec<2, GLfloat> startPosition =
                    aBasePosition
                    + static_cast<GLfloat>(aRemainingTime)*Vec<2, GLfloat>{speed.x(), speed.y()};
                aEngine.addEntity(entities::makeEnemyBullet(startPosition, speed));
            }

        });
    }

private:
    Periodic mPeriod;
    const int mCount{0};
};

} // namespace Fire

} // namespace ad
