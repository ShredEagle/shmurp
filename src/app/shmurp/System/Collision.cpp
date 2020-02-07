#include "Collision.h"

namespace ad {

Collision::Collision(aunteater::Engine &aEngine) :
    mEngine(aEngine),
    mColliders(mEngine)
{}

void Collision::update(double aDelta)
{
    mColliders.forEach([this](Faction & attackerFaction, Geometry & attackerGeometry)
    {
        for(auto defender : mColliders)
        {
            if(   (attackerFaction.isAttacking(defender->get<Faction>()))
               && (attackerGeometry.isColliding(defender->get<Geometry>())) )
            {
                mEngine.markToRemove(defender);
            }
        }
    });
}

} // namespace ad
