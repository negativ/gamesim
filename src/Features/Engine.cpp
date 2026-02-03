#include <Features/Engine.hpp>

namespace sw::features {

namespace swc = sw::core;
namespace swcc = sw::core::components;

void Engine::createMap(std::uint32_t width, std::uint32_t height) {
    gc.serviceLocator().emplace<swc::Map>(width, height);
    gc.addEvent(sw::io::MapCreated{width, height});
}

void Engine::emitMarchOrder(swc::EntityId id, std::uint32_t toX,
                            std::uint32_t toY) {
    if (auto pos = gc.entityRegistry().get<swcc::Position>(id); pos) {
        gc.entityRegistry().add(id, swcc::MarchCommand{toX, toY});
        gc.addEvent(sw::io::MarchStarted{id, pos->x, pos->y, toX, toY});
    }
}

void Engine::run() {
    std::uint64_t tick = 1;

    while (true) {
        std::uint64_t alive = 0;

        gc.entityRegistry().forEachWith<swcc::Health>(
            [&alive](auto id, auto health) { alive += health->dead() ? 0 : 1; });

        if (alive < 2)
            break;

        auto hasUpdates = false;

        gc.entityRegistry().forEach([&hasUpdates, this](auto id) {
            hasUpdates |= RangedAttack::perform(gc, id) ||
                          MeleeAttack::perform(gc, id) || March::perform(gc, id);
        });

        auto events = std::move(gc.extractEvents());

        if (auto logger = gc.serviceLocator().get<sw::EventLog>(); logger) {
            for (auto &ev : events) {
                std::visit(
                    [logger, tick](auto &payload) {
                        logger->log(tick, std::move(payload));
                    },
                    ev);
            }
        }

        events.clear();

        // Garbage collect
        gc.entityRegistry().forEachWith<swcc::Health>([this](auto id, auto health) {
            if (health->dead())
                gc.entityRegistry().unregisterEntity(id);
        });
        gc.entityRegistry().cleanup();

        if (!hasUpdates)
            break;

        ++tick;
    }
}

} // namespace sw::features
