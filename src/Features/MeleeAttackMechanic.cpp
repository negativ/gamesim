#include <Features/MeleeAttackMechanic.hpp>

#include <deque>

#include <Core/Components.hpp>
#include <Features/AttackHelpers.hpp>

namespace sw::features {

namespace swc = sw::core;
namespace swcc = sw::core::components;

bool MeleeAttackMechanic::perform(GameContext &gc, EntityId id) {
    auto melee = gc.entityRegistry().get<swcc::MeleeAttackAbility>(id);
    auto pos = gc.entityRegistry().get<swcc::Position>(id);

    if (!(melee && pos)) {
        return false;
    }

    std::deque<EntityId> targets;
    AttackHelpers::getTargetsAround(gc.entityRegistry(), targets, *pos, 1, 1);

    targets.erase(std::remove(targets.begin(), targets.end(), id), targets.end());

    if (targets.empty()) {
        return false;
    }

    // Picking random target
    auto targetId = AttackHelpers::pickRandom(targets);

    // Returned `targetId` has `Health` component as guaranteed by
    // `AttackHelpers::getTargetsAround` function.
    auto targetHp = gc.entityRegistry().get<swcc::Health>(targetId);

    *targetHp -= melee->damage;

    gc.addEvent(io::UnitAttacked(id, targetId, melee->damage, targetHp->current));

    return true;
}

} // namespace sw::features
