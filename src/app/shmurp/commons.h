#pragma once

#include <math/Vector.h>
#include <math/Matrix.h>

#include <glad/glad.h>

namespace ad {

/// TODO move to Math lib
constexpr double pi = 3.14159265358979323846;

template <int N, class T>
using Vec = math::Vec<N, T>;

template <int N, class T>
using Matrix = math::Matrix<N, N, T>;

} // namespace ad
