#include <Features/AttackHelpers.hpp>

namespace sw::features {

using namespace sw::core;
using namespace sw::core::components;

std::optional<EntityId> AttackHelpers::getGroundObjectAt(const Registry &ctx,
                                                         Position p) noexcept {
    for (auto id : ctx.entities()) {
        auto pos = ctx.get<Position>(id);
        auto staysOnGround = ctx.get<traits::GroundObject>(id);

        if (pos && staysOnGround) {
            if (pos->x == p.x && pos->y == p.y)
                return id;
        }
    }

    return std::nullopt;
}

} // namespace sw::features
