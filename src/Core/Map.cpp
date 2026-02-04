#include "Core/Map.hpp"

#include <algorithm>
#include <cmath>

namespace sw::core {

std::uint32_t Map::distance(Position from, Position to) noexcept {
    return std::max(std::abs(std::int32_t(from.x) - std::int32_t(to.x)),
                    std::abs(std::int32_t(from.y) - std::int32_t(to.y)));
}

} // namespace sw::core
