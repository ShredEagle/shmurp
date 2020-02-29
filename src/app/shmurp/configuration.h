#pragma once

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
constexpr float shipRadius = 0.2f;
constexpr float squareRadius = 0.3f;
constexpr float gBulletRadius = 0.1f;

constexpr float gViewportOffset = 2.f; // how far from the viewport to spawn/discard entities

constexpr float gShipSpeed = 12.5f;
constexpr float gShipAccelerationFactor = 20.f;

constexpr float gBulletSpeed = 25.f;
constexpr float gEnemyBulletSpeed = 8.f;

// TODO should be able to constexpr
inline Matrix<4, GLfloat> worldToDevice()
{
    return transform::scaleMatrix(conf::getWorldToDeviceScale(conf::gWindowWidth),
                                  conf::getWorldToDeviceScale(conf::gWindowHeight))
           * transform::translateMatrix(-1., -1.);
}

} // namespace conf
} // namespace ad
