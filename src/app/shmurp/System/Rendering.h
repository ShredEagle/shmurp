#pragma once

#include "commons.h"

#include <aunteater/System.h>

#include <renderer/VertexSpecification.h>
#include <renderer/Shading.h>

namespace ad {

class Rendering : public aunteater::System
{
    struct Impl
    {
        Impl();
        void draw();

        VertexArrayObject mVAO;
        VertexBufferObject mVBO;
        Program mProgram;

        Matrix<4, GLfloat> mWorldToScreen;
    };

public:
    Rendering() = default;

    virtual void addedToEngine(aunteater::Engine &aEngine) override;
    virtual void update(double time) override;

private:
    aunteater::Family * mRenderables;
    Impl mImpl;
};

} // namespace ad
