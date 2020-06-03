#pragma once

#include "commons.h"
#include "Instancing.h"

#include <renderer/VertexSpecification.h>

#include <gsl/gsl>


namespace ad {

namespace vertex3D
{

struct Data
{
    Vec<3, GLfloat> mPosition;
    Vec<3, GLfloat> mNormal{0.f, 0.f, 1.f};
};

constexpr AttributeDescriptionList gDescription = {
    { 0, 3, offsetof(Data, mPosition), MappedGL<GLfloat>::enumerator},
    { 1, 3, offsetof(Data, mNormal), MappedGL<GLfloat>::enumerator},
};

inline Data operator*(const Data & aLhs, const Matrix<4> & aRhs)
{
    Vec<4> position{aLhs.mPosition.x(), aLhs.mPosition.y(), aLhs.mPosition.z(), 1.0f};
    Vec<4> normal{aLhs.mNormal.x(), aLhs.mNormal.y(), aLhs.mNormal.z(), 0.0f};

    position *= aRhs;
    normal *= aRhs;

    return {
        {position.x(), position.y(), position.z()},
        {normal.x(), normal.y(), normal.z()},
    };
}

inline const gsl::span<Data> & operator*=(const gsl::span<Data> & aLhs, const Matrix<4> & aRhs)
{
    std::transform(aLhs.begin(), aLhs.end(), aLhs.begin(),
                   [&aRhs](const Data & data){ return data * aRhs; });
    return aLhs;
}

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
        {-1.5f, -1.25f, 0.0f},
    },
    vertex3D::Data{
        { 0.0f,  3.75f, 0.0f},
    },
    vertex3D::Data{
        { 1.5f, -1.25f, 0.0f},
    },
    vertex3D::Data{
        { 0.0f,  0.0f,  0.0f},
    },
};

} // namespace ship3D

namespace cube {

static constexpr float cpi4 = 0.70710678f;

static const std::array<vertex3D::Data, 8*3> gVertices = {
    // Frontface
    vertex3D::Data{
        {-1.0f, -1.0f, 1.0f},
        {-cpi4,  0.0f, cpi4},
    },
    vertex3D::Data{
        {-1.0f,  1.0f, 1.0f},
        {-cpi4,  0.0f, cpi4},
    },
    vertex3D::Data{
        {-1.0f,  1.0f, 1.0f},
        { 0.0f,  cpi4, cpi4},
    },
    vertex3D::Data{
        { 1.0f,  1.0f, 1.0f},
        { 0.0f,  cpi4, cpi4},
    },
    vertex3D::Data{
        { 1.0f,  1.0f, 1.0f},
        { cpi4,  0.0f, cpi4},
    },
    vertex3D::Data{
        { 1.0f,  -1.0f, 1.0f},
        { cpi4,  0.0f,  cpi4},
    },
    vertex3D::Data{
        { 1.0f, -1.0f, 1.0f},
        { 0.0f, -cpi4, cpi4},
    },
    vertex3D::Data{
        {-1.0f, -1.0f, 1.0f},
        { 0.0f, -cpi4, cpi4},
    },

    // Backface
    vertex3D::Data{
        {-1.0f, -1.0f, -1.0f},
          {-cpi4,  0.0f, -cpi4},
    },
    vertex3D::Data{
        {-1.0f,  1.0f, -1.0f},
        {-cpi4,  0.0f, -cpi4},
    },
    vertex3D::Data{
        {-1.0f,  1.0f, -1.0f},
        { 0.0f,  cpi4, -cpi4},
    },
    vertex3D::Data{
        { 1.0f,  1.0f, -1.0f},
        { 0.0f,  cpi4, -cpi4},
    },
    vertex3D::Data{
        { 1.0f,  1.0f, -1.0f},
        { cpi4,  0.0f, -cpi4},
    },
    vertex3D::Data{
        { 1.0f,  -1.0f, -1.0f},
        { cpi4,   0.0f, -cpi4},
    },
    vertex3D::Data{
        { 1.0f, -1.0f, -1.0f},
        { 0.0f, -cpi4, -cpi4},
    },
    vertex3D::Data{
        {-1.0f, -1.0f, -1.0f},
        { 0.0f, -cpi4, -cpi4},
    },

    // Connect
    vertex3D::Data{
        {-1.0f, -1.0f, -1.0f},
        {-cpi4, -cpi4,  0.0f},
    },
    vertex3D::Data{
        {-1.0f, -1.0f, +1.0f},
        {-cpi4, -cpi4,  0.0f},
    },
    vertex3D::Data{
        {-1.0f,  1.0f, -1.0f},
        {-cpi4,  cpi4,  0.0f},
    },
    vertex3D::Data{
        {-1.0f,  1.0f, +1.0f},
        {-cpi4,  cpi4,  0.0f},
    },
    vertex3D::Data{
        { 1.0f,  1.0f, -1.0f},
        { cpi4,  cpi4,  0.0f},
    },
    vertex3D::Data{
        { 1.0f,  1.0f, +1.0f},
        { cpi4,  cpi4,  0.0f},
    },
    vertex3D::Data{
        { 1.0f,  -1.0f, -1.0f},
        { cpi4,  -cpi4,  0.0f},
    },
    vertex3D::Data{
        { 1.0f,  -1.0f, +1.0f},
        { cpi4,  -cpi4,  0.0f},
    },
};

} // namespace cube

namespace circle3D {

enum Type
{
    Circle,
    Disc,
};

template <GLsizei N_vertices, Type N_type = Circle>
std::vector<vertex3D::Data> makeVertices(GLfloat aRadius)
{
    std::vector<vertex3D::Data> result;
    if (N_type == Disc)
    {
        result.push_back(vertex3D::Data{
            {0.f, 0.f, 0.f},
        });
    }
    //result.reserve(N_vertices+2);
    for(int i=0; i<=N_vertices; ++i)
    {
        result.emplace_back(vertex3D::Data{{
                                                cos(2*pi<float>*i/N_vertices) * aRadius,
                                                sin(2*pi<float>*i/N_vertices) * aRadius,
                                                0.f,
                                           },
                                           {
                                               0.f,
                                               0.f,
                                               N_type == Disc ? -1.f : 1.0f,
                                           }});
    }
    return result;
}

} // namespace circle3D

namespace instance3D {

struct Data
{
    Vec<2, GLfloat> mPosition;
    Matrix<4, GLfloat> mOrientationScale{Matrix<4, GLfloat>::Identity()};
};

constexpr AttributeDescriptionList gDescription = {
    {2, 2, offsetof(Data, mPosition),      MappedGL<GLfloat>::enumerator},
    {3, 4, offsetof(Data, mOrientationScale),   MappedGL<GLfloat>::enumerator},
    {4, 4, offsetof(Data, mOrientationScale) +  4*sizeof(GLfloat),   MappedGL<GLfloat>::enumerator},
    {5, 4, offsetof(Data, mOrientationScale) +  8*sizeof(GLfloat),   MappedGL<GLfloat>::enumerator},
    {6, 4, offsetof(Data, mOrientationScale) + 12*sizeof(GLfloat),   MappedGL<GLfloat>::enumerator},
};

} // namespace instance3D

template <>
class BufferData_trait<instance3D::Data>
{
public:
    static constexpr AttributeDescriptionList description_value = instance3D::gDescription;
};


} // namespace ad
