#pragma once

#include "commons.h"
#include "Instancing.h"

#include <renderer/VertexSpecification.h>

namespace ad {

namespace vertex3D
{

struct Data
{
    Vec<3, GLfloat> mPosition;
};

constexpr AttributeDescriptionList gDescription = {
    { 0, 3, offsetof(Data, mPosition), MappedGL<GLfloat>::enumerator},
};

} // namespace vertex3D

template <>
class BufferData_trait<vertex3D::Data>
{
public:
    static constexpr AttributeDescriptionList description_value = vertex3D::gDescription;
};

namespace ship3D {

static const std::array<vertex3D::Data, 4> gVertices = {
    vertex3D::Data{
        {-0.3f, -0.25f, 0.0f},
    },
    vertex3D::Data{
        { 0.0f,  0.75f, 0.0f},
    },
    vertex3D::Data{
        { 0.3f, -0.25f, 0.0f},
    },
    vertex3D::Data{
        { 0.0f,  0.0f,  0.0f},
    },
};

} // namespace ship3D

namespace cube {

static const std::array<vertex3D::Data, 8*3> gVertices = {
    //Upface
    vertex3D::Data{
        {-0.3f, -0.3f, 0.3f},
    },
    vertex3D::Data{
        {-0.3f,  0.3f, 0.3f},
    },
    vertex3D::Data{
        {-0.3f,  0.3f, 0.3f},
    },
    vertex3D::Data{
        { 0.3f,  0.3f, 0.3f},
    },
    vertex3D::Data{
        { 0.3f,  0.3f, 0.3f},
    },
    vertex3D::Data{
        { 0.3f,  -0.3, 0.3f},
    },
    vertex3D::Data{
        { 0.3f,  -0.3, 0.3f},
    },
    vertex3D::Data{
        {-0.3f, -0.3f, 0.3f},
    },

    //Lowface
    vertex3D::Data{
        {-0.3f, -0.3f, -0.3f},
    },
    vertex3D::Data{
        {-0.3f,  0.3f, -0.3f},
    },
    vertex3D::Data{
        {-0.3f,  0.3f, -0.3f},
    },
    vertex3D::Data{
        { 0.3f,  0.3f, -0.3f},
    },
    vertex3D::Data{
        { 0.3f,  0.3f, -0.3f},
    },
    vertex3D::Data{
        { 0.3f,  -0.3, -0.3f},
    },
    vertex3D::Data{
        { 0.3f,  -0.3, -0.3f},
    },
    vertex3D::Data{
        {-0.3f, -0.3f, -0.3f},
    },

    //Connect
    vertex3D::Data{
        {-0.3f, -0.3f, -0.3f},
    },
    vertex3D::Data{
        {-0.3f, -0.3f, +0.3f},
    },
    vertex3D::Data{
        {-0.3f,  0.3f, -0.3f},
    },
    vertex3D::Data{
        {-0.3f,  0.3f, +0.3f},
    },
    vertex3D::Data{
        { 0.3f,  0.3f, -0.3f},
    },
    vertex3D::Data{
        { 0.3f,  0.3f, +0.3f},
    },
    vertex3D::Data{
        { 0.3f,  -0.3, -0.3f},
    },
    vertex3D::Data{
        { 0.3f,  -0.3, +0.3f},
    },
};

} // namespace cube

namespace circle3D {

template <GLsizei N_vertices>
std::vector<vertex3D::Data> makeVertices(GLfloat aRadius)
{
    std::vector<vertex3D::Data> result{vertex3D::Data{
        {0.f, 0.f, 0.f},
    }};
    result.reserve(N_vertices+2);
    for(int i=0; i<=N_vertices; ++i)
    {
        result.emplace_back(vertex3D::Data{ {cos(2*pi<float>*i/N_vertices) * aRadius,
                                             sin(2*pi<float>*i/N_vertices) * aRadius,
                                             0.f} });
    }
    return result;
}

} // namespace circle3D

namespace instance3D {

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

} // namespace instance3D

template <>
class BufferData_trait<instance3D::Data>
{
public:
    static constexpr AttributeDescriptionList description_value = instance3D::gDescription;
};


} // namespace ad
