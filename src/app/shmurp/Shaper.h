#pragma once

#include "shapes3D.h"

#include <gsl/gsl>


namespace ad {


template <int N_baseEdges>
std::vector<vertex3D::Data> make_pyramid(float aBaseRadius, float aHeight)
{
    std::vector<vertex3D::Data> result;
    // There are twice the number of edges in the pyramid, each edge being two vertices
    constexpr std::size_t ElementsCount{N_baseEdges*2*2};
    result.reserve(ElementsCount);

    Vec<3> topPosition = {0.0f, 0.0f, aHeight};

    auto getBasePosition = [](int aId, float aRadius) -> Vec<3>
    {
        return {
            std::cos(2*pi<>*aId/N_baseEdges) * aRadius,
            std::sin(2*pi<>*aId/N_baseEdges) * aRadius,
            0.0f,
        };
    };

    Vec<3> position_prev = getBasePosition(-1, aBaseRadius);
    for(int i=0; i<N_baseEdges; ++i)
    {
        Vec<3> position_a = getBasePosition(i, aBaseRadius);
        Vec<3> position_b = getBasePosition(i+1, aBaseRadius);

        // The base segment
        {
            // The normal of BA vector (not AB)
            Vec<3> ba = (position_a - position_b).normalize();
            Vec<3> base_normal = {-ba.y(), ba.x(), 0.0f};

            result.emplace_back(vertex3D::Data{{position_a}, {base_normal}});
            result.emplace_back(vertex3D::Data{{position_b}, {base_normal}});
        }

        // The segment connecting base vertex "i" to the top
        {
            float teta = std::atan(aBaseRadius/aHeight); // angle between Z and a side edge
            Vec<3> b_prev = (position_prev - position_b).normalize();
            Vec<3> side_normal = {
                -b_prev.y() * std::cos(teta),
                b_prev.x() * std::cos(teta),
                std::sin(teta),
            };
            result.emplace_back(vertex3D::Data{{position_a}, {side_normal}});
            result.emplace_back(vertex3D::Data{{topPosition}, {side_normal}});
        }

        position_prev = position_a;
    }

    Expects(result.size() == ElementsCount);
    return result;
}

} // namespace ad
