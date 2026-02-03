#pragma once

#include <Features/GameContext.hpp>

namespace sw::features {

using namespace sw::core;

/// @brief Basic `melee attack` logic
struct MeleeAttack {

    /// @brief Try to perform `melee` logic for entity.
    /// @param gc game context
    /// @param id entity id
    /// @return `true` if entity has performed any `marching` action
    static bool perform(GameContext &gc, EntityId id);
};

} // namespace sw::features
