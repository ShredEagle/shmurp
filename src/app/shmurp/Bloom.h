#pragma once

#include "BloomShaders.h"
#include "commons.h"
#include "ScreenQuad.h"

#include <renderer/FrameBuffer.h>
#include <renderer/Shading.h>
#include <renderer/Texture.h>
#include <renderer/VertexSpecification.h>


namespace ad {

class Bloom
{
public:
    Bloom(Size<2, GLsizei> aResolution)

    {
        glProgramUniform1i(mHBlurProgram,
                           glGetUniformLocation(mHBlurProgram, "inputTexture"),
                           0);
        glProgramUniform1i(mVBlurProgram,
                           glGetUniformLocation(mVBlurProgram, "inputTexture"),
                           1);

        glProgramUniform1i(mScreenProgram,
                           glGetUniformLocation(mScreenProgram, "sceneTexture"),
                           1);
        glProgramUniform1i(mScreenProgram,
                           glGetUniformLocation(mScreenProgram, "bloomTexture"),
                           0);

        allocateStorage(mTextureScene, GL_RGBA8, aResolution);
        allocateStorage(mTexturePing,  GL_RGBA8, aResolution);
        allocateStorage(mTexturePong,  GL_RGBA8, aResolution);

        attachImage(mSceneFB, mTextureScene, GL_COLOR_ATTACHMENT0, 0);
        attachImage(mSceneFB, mTexturePing,  GL_COLOR_ATTACHMENT1, 0);
        // TODO somehow auto-manage that
        {
            bind_guard bound{mSceneFB};
            unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
            glDrawBuffers(2, attachments);
        }

        attachImage(mPingFB, mTexturePing, GL_COLOR_ATTACHMENT0, 0);
        attachImage(mPongFB, mTexturePong, GL_COLOR_ATTACHMENT0, 0);
    }

    [[nodiscard]] auto bindFramebuffer()
    {
        return bind_guard{mSceneFB};
    }

    void bloom(int aPasses)
    {
        glBindVertexArray(mScreenQuad.mVertexArray);

        glActiveTexture(GL_TEXTURE0);
        bind(mTexturePing);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glActiveTexture(GL_TEXTURE1);
        bind(mTexturePong);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        for (int pass = 0; pass != 2*aPasses; ++pass)
        {
            // when pass is pair: ping is rendering to pong
            glUseProgram(pass%2 ? mVBlurProgram : mHBlurProgram);
            bind_guard boundFB{pass%2 ? mPingFB : mPongFB};
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    void drawResult()
    {
        glBindVertexArray(mScreenQuad.mVertexArray);

        // in case other code bound something else since bloom()
        glActiveTexture(GL_TEXTURE0);
        bind(mTexturePing);

        // replaces pong texture
        glActiveTexture(GL_TEXTURE1);
        bind(mTextureScene);

        glUseProgram(mScreenProgram);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

private:
    Program mScreenProgram{makeLinkedProgram({
            {GL_VERTEX_SHADER, gScreenVertex},
            {GL_FRAGMENT_SHADER, gScreenFragment}})};

    Program mHBlurProgram{makeLinkedProgram({
            {GL_VERTEX_SHADER, gScreenVertex},
            {GL_FRAGMENT_SHADER, gHBlurFragment}})};
    Program mVBlurProgram{makeLinkedProgram({
            {GL_VERTEX_SHADER, gScreenVertex},
            {GL_FRAGMENT_SHADER, gVBlurFragment}})};

    Texture mTextureScene{GL_TEXTURE_2D};
    Texture mTexturePing{GL_TEXTURE_2D};
    Texture mTexturePong{GL_TEXTURE_2D};

    FrameBuffer mSceneFB;
    FrameBuffer mPingFB;
    FrameBuffer mPongFB;

    // Cannot be static: the application must be started before starting to load
    // VOAs and VBOs
    VertexSpecification mScreenQuad{make_ScreenQuad()};
};


} // namespace ad
