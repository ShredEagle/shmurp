#include "Rendering.h"

#include "RenderingShaders.h"

#include "../shapes.h"

#include <Components/Position.h>

namespace ad {

typedef aunteater::Archetype<Position> Renderable;

void Rendering::addedToEngine(aunteater::Engine &aEngine)
{
    mRenderables = & aEngine.getFamily<Renderable>();
}

void Rendering::update(double time)
{
    mImpl.draw();
}

Matrix<4, GLfloat> ScaleMatrix(GLfloat x, GLfloat y)
{
    auto result = Matrix<4, GLfloat>::Identity();
    result[0][0] = x;
    result[1][1] = y;
    return result;
}

Rendering::Impl::Impl() :
    mVBO(loadVertexBuffer(mVAO, triangle::gDescription, gsl::span<VertexShape>(triangle::gVertices))),
    mProgram(makeLinkedProgram({ {GL_VERTEX_SHADER,   gVertexShader},
                                 {GL_FRAGMENT_SHADER, gFragmentShader} })),
    mWorldToScreen(ScaleMatrix(1.f/12, 1.f/16))
{
    std::cout << mWorldToScreen << std::endl;
    glProgramUniformMatrix4fv(mProgram, glGetUniformLocation(mProgram, "u_WorldToScreen"),
                              1, false, mWorldToScreen.data());
}

void Rendering::Impl::draw()
{
    glBindVertexArray(mVAO);
    glUseProgram(mProgram);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

} // namespace add
