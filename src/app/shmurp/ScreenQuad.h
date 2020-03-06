#pragma once

#include <renderer/VertexSpecification.h>

namespace ad {

struct VertexScreenQuad
{
    Vec<2, GLfloat> mPosition;
    Vec<2, GLfloat> mUV;
};

constexpr std::initializer_list<AttributeDescription> gVertexScreenDescription = {
    { 0, 2, offsetof(VertexScreenQuad, mPosition), MappedGL<GLfloat>::enumerator},
    { 1, 2, offsetof(VertexScreenQuad, mUV),       MappedGL<GLfloat>::enumerator},
};

inline VertexSpecification make_ScreenQuad()
{
    std::array<VertexScreenQuad, 4> gVerticesScreen = {
        VertexScreenQuad{
            {-1.0f, -1.0f},
            {0.0f, 0.0f},
        },
        VertexScreenQuad{
            {-1.0f,  1.0f},
            {0.0f,  1.0f},
        },
        VertexScreenQuad{
            { 1.0f, -1.0f},
            { 1.0f, 0.0f},
        },
        VertexScreenQuad{
            { 1.0f,  1.0f},
            { 1.0f,  1.0f},
        },
    };

    VertexSpecification result;
    appendToVertexSpecification(result, gVertexScreenDescription, gsl::make_span(gVerticesScreen));
    return result;
}

} // namespace ad
