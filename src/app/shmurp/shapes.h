#pragma once

#include "commons.h"

namespace ad {

struct VertexShape
{
    Vec<2, GLfloat> mPosition;
};

namespace triangle {


std::array<VertexShape, 4> gVertices= {
    VertexShape{
        {-0.3f,  0.0f},
    },
    VertexShape{
        { 0.0f,  1.0f},
    },
    VertexShape{
        { 0.3f,  0.0f},
    },
    VertexShape{
        { 0.0f,  0.25f},
    },
};

constexpr std::initializer_list<AttributeDescription> gDescription = {
    { 0, 2, offsetof(VertexShape, mPosition), MappedGL<GLfloat>::enumerator},
};

} // namespace triangle

} // namespace ad
