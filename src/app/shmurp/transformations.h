#pragma once

#include "commons.h"

namespace ad {
namespace transform {

// TODO Factor that to a dedicated library
Matrix<4, GLfloat> scaleMatrix(GLfloat x, GLfloat y)
{
    auto result = Matrix<4, GLfloat>::Identity();
    result[0][0] = x;
    result[1][1] = y;
    return result;
}

Matrix<4, GLfloat> translateMatrix(GLfloat x, GLfloat y)
{
    auto result = Matrix<4, GLfloat>::Identity();
    result[3][0] = x;
    result[3][1] = y;
    return result;
}

} // namespace transform
} // namespace ad
