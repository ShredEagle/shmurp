#include "Collision.h"

namespace ad {

Collision::Collision(aunteater::Engine &aEngine) :
    mEngine(aEngine),
    mColliders(mEngine)
{}

void Collision::update(double aDelta)
{
    for(auto attacker : mColliders)
    {
        for(auto defender : mColliders)
        {
            if(   (attacker->get<Faction>().isAttacking(defender->get<Faction>()))
               && (attacker->get<Geometry>().isColliding(defender->get<Geometry>())) )
            {
                defender->get<HitPoints>().hp -= 1;

                if (defender->get<HitPoints>().hp == 0)
                {
                  mEngine.markToRemove(defender);
                }

                if (defender->get<Faction>().isAttacking(attacker->get<Faction>()))
                {
                    mEngine.markToRemove(attacker);
                }
            }
        }
    }
}

} // namespace ad
