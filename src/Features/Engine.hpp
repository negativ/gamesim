#pragma once

#include <Core/Components.hpp>
#include <Core/Map.hpp>
#include <Features/GameContext.hpp>
#include <Features/March.hpp>
#include <Features/MeleeAttack.hpp>
#include <Features/RangedAttack.hpp>
#include <IO/System/EventLog.hpp>

namespace sw::features {

class Engine {
    GameContext &gc;

    auto &entities() { return gc.entityRegistry().entities(); }

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
    void run();
};

} // namespace sw::features
