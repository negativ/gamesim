#pragma once

#include <random>

#include <Core/Components.hpp>
#include <Core/Map.hpp>
#include <Core/Registry.hpp>
#include <Core/Types.hpp>

namespace sw::features {

/// @brief Set of helper functions.
struct AttackHelpers {

    /// @brief Try to obtain object at specified position. Object must have trait
    /// `traits::GroundObject`.
    /// @param reg entity registry
    /// @param p position to check
    /// @return entity id if found.
    static std::optional<core::EntityId>
    getGroundObjectAt(const sw::core::Registry &reg,
                      sw::core::components::Position p) noexcept;

    /// @brief Obtain suitable targets  around some position. Each target is
    /// guaranteed to have `Health` component.
    /// @param reg entity registry
    /// @param entities container to save entity ids
    /// @param center position to check targets around
    /// @param minR minimal range
    /// @param maxR maximal range
    /// @returns `true` if such objects were found.
    template <typename Container>
    static bool
    getTargetsAround(const sw::core::Registry &reg, Container &entities,
                     sw::core::components::Position center, std::uint32_t minR,
                     std::uint32_t maxR) noexcept {
        reg.forEachWith<sw::core::components::Position>([&](auto id, auto pos) {
            if (*pos == center)
                return;

            if (auto d = sw::core::Map::distance(center, *pos);
                d >= minR && d <= maxR) {
                if (auto health = reg.get<sw::core::components::Health>(id);
                    health && !health->dead())
                    entities.push_back(id);
            }
        });

        return !entities.empty();
    }

    /// @brief Pick random element from the container.
    /// @param elements container to pick element
    /// @note throws `std::runtime_error` if container is empty
    template <typename Container>
    static decltype(auto) pickRandom(Container &&elements) {
        // TODO: assert contract?
        if (elements.empty())
            throw std::runtime_error("empty container");

        static std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dist(0, elements.size() - 1);
        return std::forward<Container>(elements)[dist(gen)];
    }
};

} // namespace sw::features
