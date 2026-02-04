#pragma once

#include <Core/Components.hpp>
#include <Core/Map.hpp>
#include <Features/GameContext.hpp>
#include <IO/System/EventLog.hpp>

namespace sw::features {

template <typename T>
concept GameMechanic = requires(GameContext &gc, sw::core::EntityId id) {
    { T::perform(gc, id) } -> std::same_as<bool>;
};

class Engine {
    GameContext &gc;

public:
    Engine(GameContext &gc) : gc{gc} {}

    /// @brief Create map inside game context and emit all required events
    /// @param width map width
    /// @param height map height
    void createMap(std::uint32_t width, std::uint32_t height);

    /// @brief emitMarchOrder
    /// @param id
    /// @param toX
    /// @param toY
    void emitMarchOrder(sw::core::EntityId id, std::uint32_t toX,
                        std::uint32_t toY);

    /// @brief Spawn unit on map. Calls `Class::create()` factory function and
    /// forwards entity registry and all passed arguments to it.
    /// @param id entity id to spwan
    /// @param args argument list to forward
    /// @return `true` if entity registered in entity registry and all required
    /// events are emitted.
    template <typename Class, typename... Args>
    bool spawn(sw::core::EntityId id, Args &&...args) {
        if (Class::create(gc.entityRegistry(), id, std::forward<Args>(args)...)) {
            sw::io::UnitSpawned event;

            event.unitId = id;

            if (auto pos =
                gc.entityRegistry().get<sw::core::components::Position>(id);
                pos) {
                event.x = pos->x;
                event.y = pos->y;
            }

            if (auto name = gc.entityRegistry().get<sw::core::components::Name>(id);
                name)
                event.unitType = name->value;

            gc.addEvent(event);

            return true;
        }

        return false;
    }

    /// @brief Start engine main routine. Routine stops if there are less than 2
    /// entities on map or no entity can make an action.
    template <GameMechanic... Mechanics> void run() {
        std::uint64_t tick = 1;

        auto log = [&tick,
                    logger{gc.serviceLocator().get<sw::EventLog>()}](auto &&event) {
            if (logger) {
                std::visit(
                    [logger, tick](auto &payload) {
                        logger->log(tick, std::move(payload));
                    },
                    event);
            }
        };

        while (true) {
            std::uint64_t alive = 0;

            gc.entityRegistry().forEachWith<sw::core::components::Health>(
                [&alive](auto id, auto health) { alive += health->dead() ? 0 : 1; });

            if (alive < 2)
                break;

            auto hasUpdates = false;

            gc.entityRegistry().forEach([&hasUpdates, this](auto id) {
                hasUpdates |= (Mechanics::perform(gc, id) || ...);
            });

            // Garbage collect
            gc.entityRegistry().forEachWith<sw::core::components::Health>(
                [this](auto id, auto health) {
                    if (health->dead()) {
                        gc.entityRegistry().unregisterEntity(id);
                        gc.addEvent(sw::io::UnitDied(id));
                    }
                });
            gc.entityRegistry().cleanup();

            // Event delivery
            auto events = std::move(gc.extractEvents());

            for (auto &ev : events)
                log(std::move(ev));

            events.clear();

            if (!hasUpdates)
                break;

            ++tick;
        }
    }
};

} // namespace sw::features
