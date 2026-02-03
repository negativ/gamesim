#pragma once

#include <Core/Registry.hpp>
#include <Features/GameContext.hpp>

namespace sw::features {

/// @brief Basic `marching` logic
struct March {

    /// @brief Try to perform `marching` logic for entity.
    /// @param gc game context
    /// @param id entity id
    /// @return `true` if entity has performed any `marching` action
    static bool perform(GameContext &gc, sw::core::EntityId id);
};

} // namespace sw::features
