#include <Features/Units/Hunter.hpp>

namespace sw::units {

namespace swc = sw::core;
namespace swcc = sw::core::components;

bool Hunter::create(core::Registry &reg, core::EntityId id,
                    core::components::Position pos, std::uint32_t hp,
                    std::uint32_t strength, std::uint32_t agility,
                    std::uint32_t range) {
    if (!reg.registerEntity(id))
        return false;

    reg.add(id, swcc::Name{"Hunter"}, pos, swcc::Health{hp, hp},
            swcc::MeleeAttackAbility{strength},
            swcc::RangedAttackAbility{2, range, agility},
            swcc::traits::GroundObject{});

    return true;
}

} // namespace sw::units
