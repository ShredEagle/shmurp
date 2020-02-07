#include "Rendering.h"

#include "configuration.h"
#include "RenderingShaders.h"
#include "transformations.h"

#include <Components/Position.h>
#include <Components/Shape.h>

namespace ad {

typedef aunteater::Archetype<Position, Shape> Renderable;

Rendering::Rendering(aunteater::Engine &aEngine) :
    mRenderables(aEngine.getFamily<Renderable>())
{
    mShapeToInstanceData.emplace(Shape::RocketShip, std::vector<instance::Data>{});
    mShapeToInstanceData.emplace(Shape::Square,     std::vector<instance::Data>{});

    glClearColor(0.04f, 0.08f, 0.12f, 1.f);
}

void Rendering::update(double time)
{
    for (const auto & renderable : mRenderables)
    {
        mShapeToInstanceData.at(renderable->get<Shape>().enumerator)
                                .push_back(renderable->get<Position>().position);
    }

    for (auto & [shape, instanceData] : mShapeToInstanceData)
    {
        respecifyBuffer(mImpl.mShapeToSpecification.at(shape).mInstanceBO,
                        gsl::span<const instance::Data>(instanceData));
        mImpl.mShapeToSpecification.at(shape).mInstanceCount = instanceData.size();
        instanceData.clear();
    }

    mImpl.draw();
}

template <class T_vertex, class T_instance>
Rendering::Impl::Spec::Spec(AttributeDescriptionList aVertexDescription,
                            gsl::span<const T_vertex> aVertexData,
                            AttributeDescriptionList aInstanceDescription,
                            gsl::span<const T_instance> aInstanceData) :
    mVAO(),
    mVBO(loadVertexBuffer(mVAO, aVertexDescription, aVertexData)),
    mInstanceBO(loadVertexBuffer(mVAO, aInstanceDescription, aInstanceData, 1)),
    mVertexCount(aVertexData.size())
{}

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
                                       gsl::span<const instance::Data>()));

    mShapeToSpecification.emplace(Shape::Square,
                                  Spec(gVertexDescription,
                                       gsl::span<const VertexShape>(square::gVertices),
                                       instance::gDescription,
                                       gsl::span<const instance::Data>()));

    glProgramUniformMatrix4fv(mProgram, glGetUniformLocation(mProgram, "u_WorldToDevice"),
                              1, true, mWorldToDevice.data());
}

void Rendering::Impl::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mProgram);

    for (const auto & [shape, specification] : mShapeToSpecification)
    {
        glBindVertexArray(specification.mVAO);
        glDrawArraysInstanced(GL_LINE_LOOP, 0, specification.mVertexCount, specification.mInstanceCount);
    }
}

} // namespace add
