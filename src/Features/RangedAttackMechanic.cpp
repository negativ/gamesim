#include <Features/RangedAttackMechanic.hpp>

#include <Features/AttackHelpers.hpp>

namespace sw::features {

namespace swc = sw::core;
namespace swcc = sw::core::components;

bool RangedAttackMechanic::perform(GameContext &gc, swc::EntityId id) {
    auto ra = gc.entityRegistry().get<swcc::RangedAttackAbility>(id);
    auto pos = gc.entityRegistry().get<swcc::Position>(id);

    if (!(ra && pos))
        return false;

    std::deque<swc::EntityId> neighbors;
    if (AttackHelpers::getTargetsAround(gc.entityRegistry(), neighbors, *pos, 1,
                                        1)) {
        // We cannot attack if there is another unit close to current
        if (std::any_of(cbegin(neighbors), cend(neighbors),
                        [id](auto other) { return other != id; }))
            return false;
    }

    // Scanning for a potential targets
    std::deque<swc::EntityId> targets;
    if (!AttackHelpers::getTargetsAround(gc.entityRegistry(), targets, *pos,
                                         ra->minRange, ra->maxRange))
        return false;

    targets.erase(std::remove(targets.begin(), targets.end(), id), targets.end());

    // Picking random target
    auto targetId = AttackHelpers::pickRandom(targets);

    // Returned `targetId` has `Health` component as guaranteed by
    // `AttackHelpers::getTargetsAround` function.
    auto targetHp = gc.entityRegistry().get<swcc::Health>(targetId);

    // TODO: Send unit attacked message
    *targetHp -= ra->agility;

    gc.addEvent(io::UnitAttacked(id, targetId, ra->agility, targetHp->current));

    return true;
}

} // namespace sw::features
