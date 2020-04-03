#pragma once

#include <aunteater/Component.h>

#include <iostream>
namespace ad {

struct FactionSpaceForce : public aunteater::Component<FactionSpaceForce>
{
  FactionSpaceForce()
  {}
};

struct FactionDemocrats : public aunteater::Component<FactionDemocrats>
{
  FactionDemocrats()
  {}
};

struct FactionTruthBullet : public aunteater::Component<FactionTruthBullet>
{
  FactionTruthBullet()
  {}
};

struct FactionLibLies : public aunteater::Component<FactionLibLies>
{
  FactionLibLies()
  {}
};

} // namespace ad
