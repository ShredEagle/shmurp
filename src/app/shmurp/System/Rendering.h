#pragma once

#include "../commons.h"

#include "../Instancing.h"
#include "../shapes.h"

#include <Components/Shape.h>

#include <aunteater/System.h>

#include <renderer/Shading.h>

namespace ad {

class Rendering : public aunteater::System
{
    using ShapeInstancing = Instancing<VertexShape, instance::Data>;
    struct Impl
    {
        Impl();
        void draw();

        std::map<Shape::Value, ShapeInstancing> mShapeToSpecification;
        Program mProgram;

        Matrix<4, GLfloat> mWorldToDevice;
    };

public:
    Rendering(aunteater::Engine &aEngine);

    void update(const aunteater::Timer aTimer) override;

private:
    const aunteater::Family & mRenderables;
    Impl mImpl;
};

} // namespace ad
