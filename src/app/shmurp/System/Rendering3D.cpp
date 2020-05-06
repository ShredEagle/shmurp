#include "Rendering3D.h"

#include "../configuration.h"

#include "RenderingShaders.h"
#include "Shaper.h"
#include "transformations.h"

namespace ad {


namespace sp = std::placeholders;


Rendering3D::Rendering3D(aunteater::Engine &aEngine, ::ad::Engine & aAppEngine) :
    mRenderables{aEngine},
    mImpl{aAppEngine.getFramebufferSize()}
{
    glClearColor(0.04f, 0.08f, 0.12f, 1.f);
    // Note: binding once construction is complete
    mSizeListener = aAppEngine.listenFramebufferResize(
            std::bind(&Rendering3D::resizeRenderTarget, this, sp::_1));
}

void Rendering3D::update(const aunteater::Timer aTimer)
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

    mImpl.draw();
}


void Rendering3D::resizeRenderTarget(Size<2, GLsizei> aNewResolution)
{
    mImpl.mOkBloomer = Bloom{aNewResolution};
}


Rendering3D::Impl::Impl(Size<2, GLsizei> aResolution):
    mProgram{makeLinkedProgram({ {GL_VERTEX_SHADER,   gVertexShader3D},
                                 {GL_FRAGMENT_SHADER, gFragmentShader3D} })},
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
                                      //Vec<4, GLfloat>(0.44f, 0.9f, 1.0f, 1.0f)));
                                      Vec<4, GLfloat>(1.0f, 0.42f, 0.07f, 1.0f)));

    // Apparently, cannot instantiate a valid gsl::span from a temporary vector
    auto pyramidVertices = make_pyramid<3>(conf::gPyramidRadius, conf::gPyramidHeight);
    mShapeToSpecification.emplace(Shape::Pyramid,
                                  ShapeInstancing(
                                      pyramidVertices *=
                                        transform::rotateMatrix_X(-pi<>/2.f)
                                        * transform::translateMatrix(0.f, conf::gPyramidHeight/2.f),
                                      {},
                                      GL_LINES,
                                      Vec<4, GLfloat>(1.0f, 0.22f, 0.39f, 1.0f)));

    glProgramUniformMatrix4fv(mProgram, glGetUniformLocation(mProgram, "u_WorldToDevice"),
                              1, true, conf::worldToDevice().data());
}

void Rendering3D::Impl::draw()
{
    {
        auto offscreenGuard{mOkBloomer.bindFramebuffer()};

        glClear(GL_COLOR_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 1, gBlack.data());

        //glEnable(GL_BLEND);
        //glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        //glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);

        glUseProgram(mProgram);

        for (const auto & [shape, instancing] : mShapeToSpecification)
        {
            glProgramUniform4fv(mProgram, glGetUniformLocation(mProgram, "u_Color"),
                                1, instancing.mColor.data());
            instancing.draw();
        }
    }

    //glDisablei(GL_BLEND, 0);
    mOkBloomer.bloom(4);
    mOkBloomer.drawResult();
}

} // namespace add
