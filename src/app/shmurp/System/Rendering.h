#pragma once

#include "commons.h"

#include "../shapes.h"

#include <Components/Shape.h>

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

        struct Spec
        {
            template <class T_vertex, class T_instance>
            Spec(AttributeDescriptionList aVertexDescription,
                 gsl::span<const T_vertex> aVertexData,
                 AttributeDescriptionList aInstanceDescription,
                 gsl::span<const T_instance> aInstanceData,
                 Vec<4, GLfloat> aColor);

            VertexArrayObject mVAO;
            VertexBufferObject mVBO;
            VertexBufferObject mInstanceBO;
            GLsizei mVertexCount{0};
            GLsizei mInstanceCount{0};
            Vec<4, GLfloat> mColor{Vec<4, GLfloat>::Zero()};
        };

        std::map<Shape::Value, Spec> mShapeToSpecification;
        Program mProgram;

        Matrix<4, GLfloat> mWorldToDevice;
    };

public:
    Rendering(aunteater::Engine &aEngine);

    void update(double time) override;

private:
    const aunteater::Family & mRenderables;
    Impl mImpl;

    std::map<Shape::Value, std::vector<instance::Data>> mShapeToInstanceData;
};

} // namespace ad
