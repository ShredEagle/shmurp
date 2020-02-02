#include <math/Vector.h>
#include <math/Matrix.h>

#include <glad/glad.h>

namespace ad {

template <int N, class T>
using Vec = math::Vec<N, T>;

template <int N, class T>
using Matrix = math::Matrix<N, N, T>;

} // namespace ad
