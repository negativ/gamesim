#pragma once

#include <cstdint>

#include <Core/Components.hpp>
#include <Core/Registry.hpp>

namespace sw::core {

using namespace sw::core::components;

/// @brief The `width x height` map class
class Map {
    std::uint32_t width{0};
    std::uint32_t height{0};

public:
    /// @brief Create a map of size `width x height`
    /// @param width map width
    /// @param height map height
    constexpr Map(std::uint32_t width, std::uint32_t height)
        : width(width), height(height) {}

    /// @brief Check that position is located inside of the map area.
    /// @param p position to check
    /// @return `true` if `p` is inside of the map area.
    constexpr bool inside(Position p) const noexcept {
        return p.x < width && p.y < height;
    }

    /// @brief Calculate the distance between two positions on the map.
    /// @param from first position
    /// @param to second position
    /// @return Distance between two positions
    static std::uint32_t distance(Position from, Position to) noexcept;
};

} // namespace sw::core
