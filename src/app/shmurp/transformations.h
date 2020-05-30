#pragma once

#include "commons.h"

namespace ad {
namespace transform {


// TODO Factor that to a dedicated library
// TODO Constexpr that
inline Matrix<4, GLfloat> scaleMatrix(GLfloat x, GLfloat y, GLfloat z)
{
    auto result = Matrix<4, GLfloat>::Identity();
    result[0][0] = x;
    result[1][1] = y;
    result[2][2] = z;
    return result;
}

inline Matrix<4, GLfloat> scaleMatrix(GLfloat s)
{
    return scaleMatrix(s, s, s);
}


inline Matrix<4, GLfloat> translateMatrix(GLfloat x, GLfloat y)
{
    auto result = Matrix<4, GLfloat>::Identity();
    result[3][0] = x;
    result[3][1] = y;
    return result;
}

inline Matrix<4, GLfloat> rotateMatrix_X(Radian<> aAngle)
{
    auto result = Matrix<4, GLfloat>::Identity();
    result[1][1] =  std::cos(aAngle.value());
    result[1][2] =  std::sin(aAngle.value());
    result[2][1] = -std::sin(aAngle.value());
    result[2][2] =  std::cos(aAngle.value());
    return result;
}

inline Matrix<4, GLfloat> rotateMatrix_Y(Radian<> aAngle)
{
    auto result = Matrix<4, GLfloat>::Identity();
    result[0][0] =  std::cos(aAngle.value());
    result[0][2] = -std::sin(aAngle.value());
    result[2][0] =  std::sin(aAngle.value());
    result[2][2] =  std::cos(aAngle.value());
    return result;
}

inline Matrix<4, GLfloat> rotateMatrix_Z(Radian<> aAngle)
{
    auto result = Matrix<4, GLfloat>::Identity();
    result[0][0] =  std::cos(aAngle.value());
    result[0][1] =  std::sin(aAngle.value());
    result[1][0] = -std::sin(aAngle.value());
    result[1][1] =  std::cos(aAngle.value());
    return result;
}


// TODO use the commons.h angle type
inline Radian<> angleBetween(Vec<2> a, Vec<2> b)
{
    // The method with dot product only returns the angle in only [0, Pi]
    //return math::Radian<GLfloat>{std::acos(a.normalize().dot(b.normalize()))};

    // see: https://stackoverflow.com/a/21484228/1027706
    return Radian<>{std::atan2(b.y(), b.x()) - std::atan2(a.y(), a.x())};
}


//
// Ad-hoc helpers, while the transformation system relies on position + 3 orientations
//
inline Matrix<4, GLfloat> makeOrientationMatrix(Vec<3, Radian<>> & aOrientations)
{
    return   transform::rotateMatrix_X(aOrientations.x())
           * transform::rotateMatrix_Y(aOrientations.y())
           * transform::rotateMatrix_Z(aOrientations.z());
}




} // namespace transform
} // namespace ad
