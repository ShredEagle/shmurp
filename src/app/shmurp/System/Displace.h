#pragma once

#include <aunteater/System.h>

namespace ad {

class Displace : public aunteater::System
{

public:
    Displace(aunteater::Engine &aEngine);

    void update(double aDelta) override;

private:
    const aunteater::Family & mMovables;
};

} // namespace ad
