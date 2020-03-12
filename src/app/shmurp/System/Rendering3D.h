#pragma once

#include "../commons.h"
#include "../Instancing.h"
#include "../shapes3D.h"

#include <Components/Geometry.h>
#include <Components/Shape.h>
#include <Components/HitPoints.h>

#include <aunteater/Archetype.h>
#include <aunteater/FamilyHelp.h>
#include <aunteater/System.h>

#include <renderer/Shading.h>

namespace ad {

class Rendering3D : public aunteater::System
{
    using ShapeInstancing = Instancing<vertex3D::Data, instance3D::Data>;

    struct Impl
    {
        Impl();
        void draw(double time);

        std::map<Shape::Value, ShapeInstancing> mShapeToSpecification;
        Program mProgram;
    };

public:
    Rendering3D(aunteater::Engine &aEngine);

    void update(double time) override;

private:
    aunteater::Family &mRenderables;
    aunteater::Family &mHealthBarRenderables;
    Impl mImpl;
};

} // namespace ad
