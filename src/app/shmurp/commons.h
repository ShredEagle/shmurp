#pragma once

#include <math/Vector.h>
#include <math/Matrix.h>

#include <glad/glad.h>

namespace ad {

/// TODO move to Math lib
template<class T>
constexpr T pi = T(3.1415926535897932385L);

template <int N, class T>
using Vec = math::Vec<N, T>;

template <int N, class T>
using Matrix = math::Matrix<N, N, T>;

using Radians = GLfloat;

} // namespace ad
