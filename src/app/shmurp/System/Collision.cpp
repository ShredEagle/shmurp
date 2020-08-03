#include "Collision.h"

#include <Components/Health.h>


namespace ad {

Collision::Collision(aunteater::Engine &aEngine) :
    mEngine(aEngine),
    mColliders(mEngine)
{}


void Collision::update(const aunteater::Timer aTimer)
{
    for(auto attacker : mColliders)
    {
        for(auto defender : mColliders)
        {
            // Note: this loop currently tests each pair two times, since it is "ordered"
            // (attacher-defender, then the defender becomes the attacker the second time around)
            // This is a bad pessimisation, but saves us from testing the symmetric isAttacking
            // during the same iteration
            if(   (attacker->get<Faction>().isAttacking(defender->get<Faction>()))
               && (attacker->get<Geometry>().isColliding(defender->get<Geometry>())) )
            {
                handleCollision(defender, attacker->get<Faction>());
            }
        }
    }
}


void Collision::handleCollision(aunteater::weak_entity aDefender, const Faction & aAttackerFaction)
{
    if (aDefender->has<Health>())
    {
        Health & health = aDefender->get<Health>();
        health.points -= aAttackerFaction.attackValue;
        if (health.points > 0)
        {
            return;
        }
    }

    mEngine.markToRemove(aDefender);
}


} // namespace ad
