#include "Rendering.h"

#include "configuration.h"
#include "RenderingShaders.h"
#include "transformations.h"

#include <Components/Geometry.h>
#include <Components/Shape.h>

namespace ad {

typedef aunteater::Archetype<Geometry, Shape> Renderable;

Rendering::Rendering(aunteater::Engine &aEngine) :
    mRenderables(aEngine.getFamily<Renderable>())
{
    glClearColor(0.04f, 0.08f, 0.12f, 1.f);
}

void Rendering::update(double time)
{
    for (const auto & renderable : mRenderables)
    {
        mImpl.mShapeToSpecification.at(renderable->get<Shape>().enumerator)
                                .mInstancesData.push_back(renderable->get<Geometry>().position);
    }

    mImpl.respecifyBuffers();
    mImpl.draw();
}

template <class T_vertex, class T_instance>
Rendering::Impl::Spec::Spec(AttributeDescriptionList aVertexDescription,
                            gsl::span<const T_vertex> aVertexData,
                            AttributeDescriptionList aInstanceDescription,
                            gsl::span<const T_instance> aInstanceData,
                            Vec<4, GLfloat> aColor) :
    mVAO(),
    mVBO(loadVertexBuffer(mVAO, aVertexDescription, aVertexData)),
    mInstanceBO(loadVertexBuffer(mVAO, aInstanceDescription, aInstanceData, 1)),
    mVertexCount(aVertexData.size()),
    mColor(aColor)
{}

void Rendering::Impl::respecifyBuffers()
{
    for (auto & [shape, spec] : mShapeToSpecification)
    {
        respecifyBuffer(spec.mInstanceBO, gsl::span<const instance::Data>(spec.mInstancesData));
        spec.mInstanceCount = spec.mInstancesData.size();
        spec.mInstancesData.clear();
    }
}

Matrix<4, GLfloat> worldToDevice()
{
    return transform::scaleMatrix(conf::getWorldToDeviceScale(conf::gWindowWidth),
                                  conf::getWorldToDeviceScale(conf::gWindowHeight))
           * transform::translateMatrix(-1., -1.);
}

Rendering::Impl::Impl() :
    mProgram(makeLinkedProgram({ {GL_VERTEX_SHADER,   gVertexShader},
                                 {GL_FRAGMENT_SHADER, gFragmentShader} })),
    mWorldToDevice(worldToDevice())
{
    mShapeToSpecification.emplace(Shape::RocketShip,
                                  Spec(gVertexDescription,
                                       gsl::span<const VertexShape>(triangle::gVertices),
                                       instance::gDescription,
                                       gsl::span<const instance::Data>(),
                                       Vec<4, GLfloat>(0.1, 0.1, 0.85, 1.0)));

    mShapeToSpecification.emplace(Shape::Square,
                                  Spec(gVertexDescription,
                                       gsl::span<const VertexShape>(square::gVertices),
                                       instance::gDescription,
                                       gsl::span<const instance::Data>(),
                                       Vec<4, GLfloat>(0.96, 0.14, 0.97, 1.0)));

    mShapeToSpecification.emplace(Shape::Circle,
                                  Spec(gVertexDescription,
                                       gsl::span<const VertexShape>(
                                           circle::makeVertices<20>(conf::gBulletRadius)),
                                       instance::gDescription,
                                       gsl::span<const instance::Data>(),
                                       Vec<4, GLfloat>(0.2, 0.2, 0.98, 1.0)));

    glProgramUniformMatrix4fv(mProgram, glGetUniformLocation(mProgram, "u_WorldToDevice"),
                              1, true, mWorldToDevice.data());
}

void Rendering::Impl::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mProgram);

    for (const auto & [shape, specification] : mShapeToSpecification)
    {
        glProgramUniform4fv(mProgram, glGetUniformLocation(mProgram, "u_Color"),
                            1, specification.mColor.data());
        glBindVertexArray(specification.mVAO);
        glDrawArraysInstanced(GL_LINE_LOOP,
                              0,
                              specification.mVertexCount,
                              specification.mInstanceCount);
    }
}

} // namespace add
