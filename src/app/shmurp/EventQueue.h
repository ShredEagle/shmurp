#pragma once

#include <aunteater/globals.h>

#include <vector>


namespace ad {


template <class T_event>
using EventQueue = std::vector<std::pair<aunteater::entity_id, T_event>>;


} // namespace ad
