#include <Features/March.hpp>

#include <array>

#include <Core/Components.hpp>
#include <Core/Map.hpp>
#include <Features/AttackHelpers.hpp>

namespace {

constexpr std::array DXs = {-1, 0, 1};
constexpr std::array DYs = {-1, 0, 1};

} // namespace

namespace sw::features {

namespace swc = sw::core;
namespace swcc = sw::core::components;

bool March::perform(GameContext &gc, swc::EntityId id) {
    auto map = gc.serviceLocator().get<swc::Map>();

    if (!map)
        return false;

    auto march = gc.entityRegistry().get<swcc::MarchCommand>(id);
    auto pos = gc.entityRegistry().get<swcc::Position>(id);

    if (!(march && pos))
        return false;

    auto marchTo = swcc::Position{march->targetX, march->targetY};

    // Already at target position
    if (*pos == marchTo)
        return false;

    auto currentDist = swc::Map::distance(*pos, marchTo);
    swc::Position bestStep = *pos;
    auto found = false;

    for (auto dx : DXs) {
        for (auto dy : DYs) {
            if (dx == 0 && dy == 0)
                continue;

            swcc::Position checkPosition{pos->x + dx, pos->y + dy};

            // Out-of-bounds check
            if (!map->inside(checkPosition))
                continue;

            // Any blocking object at this position?
            if (auto blocker = AttackHelpers::getGroundObjectAt(gc.entityRegistry(),
                                                                checkPosition);
                blocker)
                continue;

            auto d = swc::Map::distance(checkPosition, marchTo);

            if (d < currentDist) {
                std::swap(currentDist, d);
                std::swap(checkPosition, bestStep);
                found = true;
            }
        }
    }

    if (found) {
        *pos = bestStep;

        gc.addEvent(sw::io::UnitMoved(id, pos->x, pos->y));

        if (*pos == marchTo) {
            gc.addEvent(sw::io::MarchEnded(id, pos->x, pos->y));
            gc.entityRegistry().erase<swcc::MarchCommand>(id);
        }
    }

    return found;
}

} // namespace sw::features
