#include "Rendering3D.h"

#include "../configuration.h"

#include "RenderingShaders.h"
#include "transformations.h"

namespace ad {

Rendering3D::Rendering3D(aunteater::Engine &aEngine) :
    mRenderables(aEngine)
{
    glClearColor(0.04f, 0.08f, 0.12f, 1.f);
}

void Rendering3D::update(double time)
{
    std::map<Shape::Value, std::vector<instance3D::Data>> sorted;

    for (const auto & renderable : mRenderables)
    {
        sorted[renderable->get<Shape>().enumerator]
            .push_back(renderable->get<Geometry>().position);
    }

    for (auto & [shape, instancing] : mImpl.mShapeToSpecification)
    {
        // indexing operator returns default constructed vector (i.e. empty) on absent shape
        instancing.updateIBO(sorted[shape]);
    }

    mImpl.draw(time);
}

Rendering3D::Impl::Impl() :
    mProgram(makeLinkedProgram({ {GL_VERTEX_SHADER,   gVertexShader3D},
                                 {GL_FRAGMENT_SHADER, gFragmentShader} })),
    mWorldToDevice(conf::worldToDevice())
{
    mShapeToSpecification.emplace(Shape::RocketShip,
                                  ShapeInstancing(
                                      ship3D::gVertices,
                                      {},
                                      GL_LINE_LOOP,
                                      Vec<4, GLfloat>(0.44, 0.9, 1.0, 1.0)));

    mShapeToSpecification.emplace(Shape::Square,
                                  ShapeInstancing(
                                      cube::gVertices,
                                      {},
                                      GL_LINES,
                                      Vec<4, GLfloat>(0.96, 0.14, 0.97, 1.0)));

    mShapeToSpecification.emplace(Shape::Circle,
                                  ShapeInstancing(
                                      circle3D::makeVertices<30>(conf::gBulletRadius),
                                      {},
                                      GL_TRIANGLE_FAN,
                                      Vec<4, GLfloat>(0.44, 0.9, 1.0, 1.0)));

    glProgramUniformMatrix4fv(mProgram, glGetUniformLocation(mProgram, "u_WorldToDevice"),
                              1, true, mWorldToDevice.data());
}

void Rendering3D::Impl::draw(double time)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(mProgram);

    mTotalTime += time;
    //glProgramUniform1f(mProgram,
    //                   glGetUniformLocation(mProgram, "u_Time"),
    //                   mTotalTime);

    for (const auto & [shape, instancing] : mShapeToSpecification)
    {
        if (shape == Shape::Square)
        {
            glProgramUniformMatrix4fv(mProgram,
                                      glGetUniformLocation(mProgram, "u_RotationSpeed"),
                                      1,
                                      true,
                                      (transform::rotateMatrix_X(mTotalTime * 0.6f)
                                        * transform::rotateMatrix_Y(mTotalTime * 1.f)).data());
        }
        else
        {
            glProgramUniformMatrix4fv(mProgram,
                                      glGetUniformLocation(mProgram, "u_RotationSpeed"),
                                      1,
                                      true,
                                      Matrix<4, GLfloat>::Identity().data());
        }

        glProgramUniform4fv(mProgram, glGetUniformLocation(mProgram, "u_Color"),
                            1, instancing.mColor.data());
        instancing.draw();
    }
}

} // namespace add
