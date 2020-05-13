#pragma once

#include "../commons.h"
#include "../configuration.h"
#include "../Entities/Bullets.h"
#include "../transformations.h"

#include "../Utils/Periodic.h"

#include <aunteater/Component.h>
#include <aunteater/Engine.h>

#include <handy/random.h>


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

        virtual std::unique_ptr<Base_impl> clone() const = 0;
        virtual ~Base_impl() = default;
    };

    template <class T_derived>
    class Base : public Base_impl
    {
    public:
        std::unique_ptr<Base_impl> clone() const final
        {
            return std::make_unique<T_derived>(*static_cast<const T_derived*>(this));
        }
    };

    /// TODO not reachable with current Engine.add()
    template <class T_base, class... VT_aArgs>
    FirePattern(VT_aArgs &&... aArgs) :
        mImplementation(new T_base(std::forward<VT_aArgs>(aArgs)...))
    {}

    template <class T_base>
    explicit FirePattern(std::unique_ptr<T_base> aImplementation) :
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

template <class T_timer>
class Line : public FirePattern::Base<Line<T_timer>>
{
public:
    explicit Line(T_timer aTimer, Radian<> aAngle=pi<Radian<>>/2.f) :
        mTimer{std::move(aTimer)},
        mAngle{aAngle}
    {}

    void fire(double aDelta,
              aunteater::Engine & aEngine,
              Vec<2, GLfloat> aBasePosition) override
    {
        mTimer.forEachEvent(aDelta, [&, this](timet aRemainingTime)
        {
#if !defined(__clang__)
            // see: https://stackoverflow.com/q/61060240/1027706
            constexpr
#endif
            Vec<4, GLfloat> gSpeed{conf::gEnemyBulletSpeed, 0.f, 0.f, 1.f};

            auto speed = gSpeed * transform::rotateMatrix_Z(mAngle);

            Vec<2, GLfloat> startPosition =
                aBasePosition
                + static_cast<GLfloat>(aRemainingTime)*Vec<2>{speed.x(), speed.y()};
            aEngine.addEntity(entities::makeEnemyBullet(startPosition, speed));
        });
    }

private:
    T_timer mTimer;
    Radian<> mAngle;
};


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
            auto speed = gSpeed * transform::rotateMatrix_Z(nextAngle());

            Vec<2, GLfloat> startPosition =
                aBasePosition
                + static_cast<GLfloat>(aRemainingTime)*Vec<2, GLfloat>{speed.x(), speed.y()};
            aEngine.addEntity(entities::makeEnemyBullet(startPosition, speed));
        });
    }

private:
    Radian<> nextAngle()
    {
        return mAngle += mAngleIncrement;
    }

private:
    Periodic mPeriod;
    Radian<> mAngle{0};
    const Radian<> mAngleIncrement;
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
                auto speed = gSpeed * transform::rotateMatrix_Z(bulletCount * 2*pi<Radian<>>/mCount);
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


class Burst : public FirePattern::Base<Burst>
{
public:
    Burst(timet aPeriod, Radian<> aSpreadAngle) :
        mPeriod{aPeriod},
        mAngleQuant{aSpreadAngle/gDivisions}
    {}

    void fire(double aDelta,
              aunteater::Engine & aEngine,
              Vec<2, GLfloat> aBasePosition) override
    {
        static constexpr Vec<4, GLfloat> gSpeed(0.f, conf::gBulletSpeed, 0.f, 1.f);

        mPeriod.forEachEvent(aDelta, [&, this](timet aRemainingTime)
        {
            auto speed = gSpeed * transform::rotateMatrix_Z(mAngleQuant*mRandomizer());
            Vec<2, GLfloat> startPosition =
                aBasePosition
                + static_cast<GLfloat>(aRemainingTime)*Vec<2, GLfloat>{speed.x(), speed.y()};
            aEngine.addEntity(entities::makeFriendBullet(startPosition, speed));
        });
    }

private:
    static constexpr int gDivisions{30};
    Randomizer<> mRandomizer{-gDivisions, gDivisions};
    Periodic mPeriod;
    const Radian<> mAngleQuant;

};

} // namespace Fire

} // namespace ad
