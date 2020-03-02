#pragma once

#include <math/Constants.h>
#include <math/Matrix.h>
#include <math/Vector.h>

#include <glad/glad.h>

namespace ad {

template <class T> T pi = math::pi<T>;

template <int N, class T>
using Vec = math::Vec<N, T>;

template <int N, class T>
using Matrix = math::Matrix<N, N, T>;

using Radians = GLfloat;

} // namespace ad
