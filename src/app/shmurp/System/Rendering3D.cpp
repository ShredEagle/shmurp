#include "Rendering3D.h"

#include "../configuration.h"

#include "RenderingShaders.h"
#include "transformations.h"

namespace ad {

Rendering3D::Rendering3D(aunteater::Engine &aEngine,
                         Size<2, GLsizei> aResolution) :
    mRenderables{aEngine},
    mImpl{aResolution}
{
    glClearColor(0.04f, 0.08f, 0.12f, 1.f);
}

void Rendering3D::update(double time)
{
    std::map<Shape::Value, std::vector<instance3D::Data>> sorted;

    for (const auto & renderable : mRenderables)
    {
        sorted[renderable->get<Shape>().enumerator]
            .push_back({renderable->get<Geometry>().position,
                        renderable->get<Geometry>().orientation});
    }

    for (auto & [shape, instancing] : mImpl.mShapeToSpecification)
    {
        // indexing operator returns default constructed vector (i.e. empty) on absent shape
        instancing.updateIBO(sorted[shape]);
    }

    mImpl.draw(time);
}

Rendering3D::Impl::Impl(Size<2, GLsizei> aResolution) :
    mProgram{makeLinkedProgram({ {GL_VERTEX_SHADER,   gVertexShader3D},
                                 {GL_FRAGMENT_SHADER, gFragmentShader} })},
    mOkBloomer{aResolution}
{
    mShapeToSpecification.emplace(Shape::RocketShip,
                                  ShapeInstancing(
                                      ship3D::gVertices,
                                      {},
                                      GL_LINE_LOOP,
                                      Vec<4, GLfloat>(0.44f, 0.9f, 1.0f, 1.0f)));

    mShapeToSpecification.emplace(Shape::Square,
                                  ShapeInstancing(
                                      cube::gVertices,
                                      {},
                                      GL_LINES,
                                      Vec<4, GLfloat>(0.96f, 0.14f, 0.97f, 1.0f)));

    mShapeToSpecification.emplace(Shape::Circle,
                                  ShapeInstancing(
                                      circle3D::makeVertices<30>(conf::gBulletRadius),
                                      {},
                                      GL_TRIANGLE_FAN,
                                      Vec<4, GLfloat>(0.44f, 0.9f, 1.0f, 1.0f)));

    glProgramUniformMatrix4fv(mProgram, glGetUniformLocation(mProgram, "u_WorldToDevice"),
                              1, true, conf::worldToDevice().data());
}

void Rendering3D::Impl::draw(double time)
{
    {
        auto offscreenGuard{mOkBloomer.bindFramebuffer()};

        glClear(GL_COLOR_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 1, gBlack.data());

        glUseProgram(mProgram);

        for (const auto & [shape, instancing] : mShapeToSpecification)
        {
            glProgramUniform4fv(mProgram, glGetUniformLocation(mProgram, "u_Color"),
                                1, instancing.mColor.data());
            instancing.draw();
        }
    }

    mOkBloomer.bloom(2);
    mOkBloomer.drawResult();
}

} // namespace add
