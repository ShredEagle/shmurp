#pragma once

#include "commons.h"

#include <renderer/VertexSpecification.h>

namespace ad {

struct VertexShape
{
    Vec<2, GLfloat> mPosition;
};

constexpr AttributeDescriptionList gVertexDescription = {
    { 0, 2, offsetof(VertexShape, mPosition), MappedGL<GLfloat>::enumerator},
};

namespace triangle {

static const std::array<VertexShape, 4> gVertices = {
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

} // namespace triangle


namespace square {

static const std::array<VertexShape, 4> gVertices = {
    VertexShape{
        {-0.3f, -0.3f},
    },
    VertexShape{
        {-0.3f,  0.3f},
    },
    VertexShape{
        { 0.3f,  0.3f},
    },
    VertexShape{
        { 0.3f,  -0.3f},
    },
};

} // namespace square

namespace instance {

struct Data
{
    Data(Vec<2, float> aPosition) :
        mPosition(aPosition)
    {}

    Vec<2, GLfloat> mPosition;
};

constexpr AttributeDescriptionList gDescription = {
    { 1, 2, offsetof(Data, mPosition), MappedGL<GLfloat>::enumerator},
};

} // namespace instance

} // namespace ad
