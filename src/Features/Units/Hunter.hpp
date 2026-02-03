#pragma once

#include <Core/Components.hpp>
#include <Core/Registry.hpp>

namespace sw::units {

/// @brief The Hunter entity factory
struct Hunter {

    /// @brief Create `Hunter` unit in entity registry.
    /// @param reg entity registry to create unit
    /// @param id entity id
    /// @param pos entity position
    /// @param hp entity health
    /// @param strength entity strength
    /// @param agility entity agility
    /// @param range max range
    /// @return `true` if entity successfully registered in `reg`.
    static bool create(sw::core::Registry &reg, sw::core::EntityId id,
                       sw::core::components::Position pos, std::uint32_t hp,
                       std::uint32_t strength, std::uint32_t agility,
                       std::uint32_t range);
};

} // namespace sw::units
