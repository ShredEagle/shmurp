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

void Rendering::update(const aunteater::Timer aTimer)
{
    std::map<Shape::Value, std::vector<instance::Data>> sorted;

    for (const auto & renderable : mRenderables)
    {
        sorted[renderable->get<Shape>().enumerator]
            .push_back(renderable->get<Geometry>().position);
    }

    for (auto & [shape, instancing] : mImpl.mShapeToSpecification)
    {
        if (auto found = sorted.find(shape); found != sorted.end())
        {
            instancing.updateIBO(found->second);
        }
    }

    mImpl.draw();
}

Rendering::Impl::Impl() :
    mProgram(makeLinkedProgram({ {GL_VERTEX_SHADER,   gVertexShader},
                                 {GL_FRAGMENT_SHADER, gFragmentShader} })),
    mWorldToDevice(conf::worldToDevice())
{
    mShapeToSpecification.emplace(Shape::RocketShip,
                                  ShapeInstancing(
                                      gsl::span<const VertexShape>(triangle::gVertices),
                                      gsl::span<const instance::Data>(),
                                      GL_LINE_LOOP,
                                      Vec<4, GLfloat>(0.44f, 0.9f, 1.0f, 1.0f)));

    mShapeToSpecification.emplace(Shape::Square,
                                  ShapeInstancing(
                                      gsl::span<const VertexShape>(square::gVertices),
                                      gsl::span<const instance::Data>(),
                                      GL_LINE_LOOP,
                                      Vec<4, GLfloat>(0.96f, 0.14f, 0.97f, 1.0f)));

    mShapeToSpecification.emplace(Shape::Bullet,
                                  ShapeInstancing(
                                      gsl::span<const VertexShape>(
                                          circle::makeVertices<20>(conf::gBulletRadius)),
                                      gsl::span<const instance::Data>(),
                                      GL_LINE_LOOP,
                                      Vec<4, GLfloat>(0.44f, 0.9f, 1.0f, 1.0f)));

    glProgramUniformMatrix4fv(mProgram, glGetUniformLocation(mProgram, "u_WorldToDevice"),
                              1, true, mWorldToDevice.data());
}

void Rendering::Impl::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mProgram);

    for (const auto & [shape, instancing] : mShapeToSpecification)
    {
        glProgramUniform4fv(mProgram, glGetUniformLocation(mProgram, "u_Color"),
                            1, instancing.mColor.data());
        instancing.draw();
    }
}

} // namespace add
