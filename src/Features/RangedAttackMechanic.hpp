#pragma once

#include <Features/GameContext.hpp>

namespace sw::features {

/// @brief Basic `ranged attack` logic
struct RangedAttackMechanic {
    /// @brief Try to perform `ranged` logic for entity.
    /// @param gc game context
    /// @param id entity id
    /// @return `true` if entity has performed any `marching` action
    static bool perform(GameContext &gc, sw::core::EntityId id);
};

} // namespace sw::features
