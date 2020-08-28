#include "Damage.h"


namespace ad {


Damage::Damage(aunteater::Engine &aEngine,
               EventQueue<shmurp::event::Impact> & aImpactEvents) :
    mEngine{aEngine},
    mImpactables{aEngine},
    mImpactReplicators{aEngine},
    mHealthBars{aEngine},
    mImpactEvents{aImpactEvents}
{}


void Damage::update(const aunteater::Timer aTimer)
{
    updateDirectImpact();
    updateForwardImpact();
    updateHealthBars();
}


void Damage::updateDirectImpact()
{
    for(auto & impactable : mImpactables)
    {
        for (auto & impact : mImpactEvents)
        {
            if (impact.first == impactable)
            {
                auto & [unusedFaction, health] = impactable;
                handleDamage(health, impact.second.damage, impactable);
            }
        }
    }
}


void Damage::updateForwardImpact()
{
    for (auto & replicator : mImpactReplicators)
    {
        auto & [health, impactRef] = replicator;
        for (auto & impact : mImpactEvents)
        {
            if (std::find(impactRef.sources.begin(), impactRef.sources.end(), impact.first)
                != impactRef.sources.end())
            {
                handleDamage(health, impact.second.damage, replicator);
            }
        }
    }
}


void Damage::updateHealthBars()
{
    for (auto & healthBar : mHealthBars)
    {
        auto & [geometry, follower] = healthBar;
        auto found = mImpactables.find(follower.followed);
        if (found != mImpactables.end())
        {
            auto & [unusedFaction, health] = *found;
            geometry.scale.x() = follower.size.x()
                                 * (static_cast<Floating>(health.points) / health.total);
            geometry.scale.y() = follower.size.y();
        }
        else
        {
            mEngine.markToRemove(healthBar);
        }
    }
}



void Damage::handleDamage(Health & aHealth, int aDamage, aunteater::weak_entity aImpacted)
{
    aHealth.points -= aDamage;
    if (aHealth.points <= 0)
    {
        mEngine.markToRemove(aImpacted);
    }
}


} // namespace ad
