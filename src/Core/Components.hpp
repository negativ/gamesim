#pragma once

#include <compare>
#include <cstdint>
#include <string>

namespace sw::core::components {

// Core data components

struct Position {
    std::uint32_t x, y;

    auto operator<=>(const Position &) const noexcept = default;
};

struct Health {
    std::uint32_t current;
    std::uint32_t max;

    bool dead() const noexcept { return current == 0; }

    auto operator<=>(const Health &) const noexcept = default;

    Health &operator-=(std::uint32_t damage) noexcept {
        if (current <= damage)
            current = 0;
        else
            current -= damage;

        return *this;
    }
};

struct Name {
    std::string value;
};

// Core abilities

struct MeleeAttackAbility {
    std::uint32_t damage;

    auto operator<=>(const MeleeAttackAbility &) const noexcept = default;
};

struct RangedAttackAbility {
    std::uint32_t minRange;
    std::uint32_t maxRange;
    std::uint32_t agility;

    auto operator<=>(const RangedAttackAbility &) const noexcept = default;
};

// Core commands

struct MarchCommand {
    std::uint32_t targetX, targetY;
};

// Core traits

namespace traits {

struct GroundObject {}; // Entity that has this trait occupies place on map.

} // namespace traits

} // namespace sw::core::components
