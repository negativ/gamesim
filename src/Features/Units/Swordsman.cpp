#include <Features/Units/Swordsman.hpp>

namespace sw::units {

namespace swc = sw::core;
namespace swcc = sw::core::components;

bool Swordsman::create(core::Registry &reg, core::EntityId id,
                       core::components::Position pos, std::uint32_t hp,
                       std::uint32_t strength) {
    if (!reg.registerEntity(id))
        return false;

    reg.add(id, swcc::Name{"Swordsman"}, pos, swcc::Health{hp, hp},
            swcc::MeleeAttackAbility{strength}, swcc::traits::GroundObject{});

    return true;
}

} // namespace sw::units
