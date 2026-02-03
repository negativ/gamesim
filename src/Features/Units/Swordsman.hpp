#pragma once

#include <Core/Components.hpp>
#include <Core/Registry.hpp>

namespace sw::units {

/// @brief The Swordsman entity factory.
struct Swordsman {

    /// @brief Create `Swordsman` unit in entity registry.
    /// @param reg entity registry to create unit
    /// @param id entity id
    /// @param pos entity position
    /// @param hp entity health
    /// @param strength entity strength
    /// @return `true` if entity successfully registered in `reg`.
    static bool create(sw::core::Registry &reg, sw::core::EntityId id,
                       sw::core::components::Position pos, std::uint32_t hp,
                       std::uint32_t strength);
};

} // namespace sw::units
