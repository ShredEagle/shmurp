#pragma once

#include "commons.h"
#include "transformations.h"

namespace ad {
namespace conf {

// Pixels
constexpr int gWindowWidth = 600;
constexpr int gWindowHeight = 800;

// World unit (meters)
constexpr float gWindowWorldHeight = 25.f;
constexpr float gWindowWorldWidth = gWindowWorldHeight*gWindowWidth/gWindowHeight;

constexpr double getWorldToDeviceScale(int aDimension)
{
    // x2, because Device goes [-1; 1], so length 2
    return static_cast<double>(gWindowHeight)/gWindowWorldHeight/aDimension * 2;
}

// World unit
constexpr float shipInitialX = gWindowWorldWidth/2.f;
constexpr float shipInitialY = 1.f;
constexpr float gShipRadius = 0.2f;
constexpr float squareRadius = 0.3f;
constexpr float gBulletRadius = 0.1f;

constexpr float gViewportOffset = 2.f; // how far from the viewport to spawn/discard entities

constexpr float gShipSpeed = 12.5f;
constexpr float gShipDirectionChangeDuration = 1.f/20.f;

constexpr float gBulletSpeed = 25.f;
constexpr float gEnemyBulletSpeed = 8.f;

constexpr Rectangle<> gShipBoundingRect = {
    {0.3f, 0.25f},
    {gWindowWorldWidth-0.3f-0.3f, gWindowWorldHeight-0.25f-0.75f}
};

constexpr Floating gPyramidRadius = 1.0f;
constexpr Floating gPyramidHeight = gPyramidRadius;

// TODO should be able to constexpr
inline Matrix<4, GLfloat> worldToDevice()
{
    // TODO the Z factor can be used to control the frustrum near/far
    return transform::scaleMatrix(conf::getWorldToDeviceScale(conf::gWindowWidth),
                                  conf::getWorldToDeviceScale(conf::gWindowHeight),
                                  1)
           * transform::translateMatrix(-1., -1.);
}

} // namespace conf
} // namespace ad
