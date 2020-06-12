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
                          Vec<2> aBasePosition,
                          Vec<3, Radian<>> aOrientation) = 0;

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
              Vec<2, GLfloat> aBasePosition,
              Vec<3, Radian<>> aOrientation)
    {
        mImplementation->fire(aDelta, aEngine, aBasePosition, aOrientation);
    }

private:
    std::unique_ptr<Base_impl> mImplementation;
};


// Might make a good component, then fire() virtual method would also take a reference to this component
// Yet, it would either mean a component whose member is a component (here faction)
// Or we could use the faction of the composed entity, but then the bullet would always have the same faction as the entity
struct BulletConfig
{
    Faction faction{Faction::LibLies, Faction::SpaceForce};
    Floating radius{conf::gBulletRadius};
    Floating velocity{conf::gEnemyBulletSpeed};
};


namespace Fire {

template <class T_timer>
class Line : public FirePattern::Base<Line<T_timer>>
{
public:
    explicit Line(T_timer aTimer, BulletConfig aBulletConfig={}) :
        mTimer{std::move(aTimer)},
        mBulletConfig{std::move(aBulletConfig)}
    {}

    void fire(double aDelta,
              aunteater::Engine & aEngine,
              Vec<2, GLfloat> aBasePosition,
              Vec<3, Radian<>> aOrientation) override
    {
        mTimer.forEachEvent(aDelta, [&, this](duration_t aRemainingTime)
        {
#if !defined(__clang__)
            // see: https://stackoverflow.com/q/61060240/1027706
            constexpr
#endif
            Vec<4, GLfloat> gSpeed{mBulletConfig.velocity, 0.f, 0.f, 1.f};

            auto speed = gSpeed * transform::makeOrientationMatrix(aOrientation);

            Vec<2, GLfloat> startPosition =
                aBasePosition
                + static_cast<GLfloat>(aRemainingTime)*Vec<2>{speed.x(), speed.y()};
            aEngine.addEntity(entities::makeBullet(mBulletConfig.faction,
                                                   startPosition,
                                                   speed,
                                                   mBulletConfig.radius));
        });
    }

private:
    T_timer mTimer;
    BulletConfig mBulletConfig;
};


class Spiral : public FirePattern::Base<Spiral>
{
public:
    Spiral(duration_t aPeriod, double aAngularSpeed, BulletConfig aBulletConfig) :
        mPeriod{aPeriod},
        mAngleIncrement(aAngularSpeed*aPeriod),
        mBulletConfig{std::move(aBulletConfig)}
    {}

    void fire(double aDelta,
              aunteater::Engine & aEngine,
              Vec<2, GLfloat> aBasePosition,
              Vec<3, Radian<>> /*unused*/) override
    {
        mPeriod.forEachEvent(aDelta, [&, this](duration_t aRemainingTime)
        {
            static const Vec<4, GLfloat> gSpeed(mBulletConfig.velocity, 0.f, 0.f, 1.f);
            auto speed = gSpeed * transform::rotateMatrix_Z(nextAngle());

            Vec<2, GLfloat> startPosition =
                aBasePosition
                + static_cast<GLfloat>(aRemainingTime)*Vec<2, GLfloat>{speed.x(), speed.y()};
            aEngine.addEntity(entities::makeBullet(mBulletConfig.faction,
                                                   startPosition,
                                                   speed,
                                                   mBulletConfig.radius));
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
    BulletConfig mBulletConfig;
};


class Arc : public FirePattern::Base<Arc>
{
public:
    /// \brief aCount has to be >= 2 to make any sense
    /// yet a negative or zero value will simply not fire, and a value of 1 will fire in front
    Arc(duration_t aPeriod, Radian<> aCoverage, int aCount, BulletConfig aBulletConfig) :
        mPeriod{aPeriod},
        mStartingAngle{aCount > 1 ? -aCoverage/2. : Radian<>{0}},
        mAngleIncrement{aCoverage / std::max(1, aCount-1)},
        mCount{aCount},
        mBulletConfig{std::move(aBulletConfig)}
    {}

    void fire(double aDelta,
              aunteater::Engine & aEngine,
              Vec<2, GLfloat> aBasePosition,
              Vec<3, Radian<>> aOrientation) override
    {
        mPeriod.forEachEvent(aDelta, [&, this](duration_t aRemainingTime)
        {
            static const Vec<4, GLfloat> gSpeed(mBulletConfig.velocity, 0.f, 0.f, 1.f);

            for (int bulletCount = 0; bulletCount < mCount; ++bulletCount)
            {
                // There are count-1 intervals
                auto speed = gSpeed
                             * transform::rotateMatrix_Z(mStartingAngle
                                                         + (bulletCount * mAngleIncrement))
                             * transform::makeOrientationMatrix(aOrientation);

                Vec<2> startPosition =
                    aBasePosition + aRemainingTime * Vec<2>{speed.x(), speed.y()};
                aEngine.addEntity(entities::makeBullet(mBulletConfig.faction,
                                                       startPosition,
                                                       speed,
                                                       mBulletConfig.radius));
            }

        });
    }

private:
    Periodic mPeriod;
    const Radian<> mStartingAngle;
    const Radian<> mAngleIncrement;
    const int mCount;
    BulletConfig mBulletConfig;
};


class Circle : public FirePattern::Base<Circle>
{
public:
    Circle(duration_t aPeriod, int aCount, BulletConfig aBulletConfig) :
        mPeriod{aPeriod},
        mCount{aCount},
        mBulletConfig{std::move(aBulletConfig)}
    {}

    void fire(double aDelta,
              aunteater::Engine & aEngine,
              Vec<2, GLfloat> aBasePosition,
              Vec<3, Radian<>> aOrientation) override
    {
        mPeriod.forEachEvent(aDelta, [&, this](duration_t aRemainingTime)
        {
            static const Vec<4, GLfloat> gSpeed(mBulletConfig.velocity, 0.f, 0.f, 1.f);

            for (int bulletCount = 0; bulletCount < mCount; ++bulletCount)
            {
                auto speed = gSpeed
                             * transform::rotateMatrix_Z(bulletCount * (2*pi<Radian<>>/mCount))
                             * transform::makeOrientationMatrix(aOrientation);

                Vec<2> startPosition =
                    aBasePosition + aRemainingTime * Vec<2>{speed.x(), speed.y()};
                aEngine.addEntity(entities::makeBullet(mBulletConfig.faction,
                                                       startPosition,
                                                       speed,
                                                       mBulletConfig.radius));
            }

        });
    }

private:
    Periodic mPeriod;
    const int mCount;
    BulletConfig mBulletConfig;
};


template <int N_divisions = 30>
class Burst : public FirePattern::Base<Burst<N_divisions>>
{
public:
    Burst(duration_t aPeriod, Radian<> aSpreadAngle, BulletConfig aBulletConfig) :
        mPeriod{aPeriod},
        mAngleQuant{aSpreadAngle/N_divisions},
        mBulletConfig{std::move(aBulletConfig)}
    {}

    void fire(double aDelta,
              aunteater::Engine & aEngine,
              Vec<2, GLfloat> aBasePosition,
              Vec<3, Radian<>> aOrientation) override
    {
        static const Vec<4, GLfloat> gSpeed(mBulletConfig.velocity, 0.f, 0.f, 1.f);

        mPeriod.forEachEvent(aDelta, [&, this](duration_t aRemainingTime)
        {
            auto speed = gSpeed
                         * transform::rotateMatrix_Z(mAngleQuant*mRandomizer())
                         * transform::makeOrientationMatrix(aOrientation);
            Vec<2, GLfloat> startPosition =
                aBasePosition
                + static_cast<GLfloat>(aRemainingTime)*Vec<2, GLfloat>{speed.x(), speed.y()};
            aEngine.addEntity(entities::makeBullet(mBulletConfig.faction,
                                                   startPosition,
                                                   speed,
                                                   mBulletConfig.radius));
        });
    }

private:
    Randomizer<> mRandomizer{-N_divisions, N_divisions, ++gSeed};
    Periodic mPeriod;
    const Radian<> mAngleQuant;
    BulletConfig mBulletConfig;

    static std::atomic<int> gSeed;

};

template <int N_divisions>
std::atomic<int> Burst<N_divisions>::gSeed = 0;


} // namespace Fire

} // namespace ad
