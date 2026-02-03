#pragma once

#include <deque>
#include <variant>

#include <Core/Registry.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <IO/System/TypeRegistry.hpp>

namespace sw::features {

/// @brief The game context class used by `Engine`.
class GameContext {
public:
    using Event = std::variant<io::MapCreated, io::MarchEnded, io::MarchStarted,
                               io::UnitAttacked, io::UnitDied, io::UnitMoved,
                               io::UnitSpawned>;
    using Events = std::deque<Event>;

private:
    TypeRegistry &d_typeRegistry;
    core::Registry &d_entityRegistry;
    Events eventQueue;

public:
    GameContext(TypeRegistry &typeRegistry, core::Registry &entityRegistry)
        : d_typeRegistry{typeRegistry}, d_entityRegistry{entityRegistry} {}

    /// @brief Add IO event to the event queue.
    /// @param e IO event to add
    template <typename EventT> void addEvent(EventT &&e) {
        eventQueue.emplace_back(std::forward<EventT>(e));
    }

    /// @brief Extract event queue from the context.
    /// @return r-value to the event queue object
    Events &&extractEvents() noexcept;

    /// @brief Return service locator instance. Use it to register or retrieve
    /// services (for e.g. `EventLog`).
    /// @return service instance
    auto &serviceLocator() noexcept { return d_typeRegistry; }

    /// @brief Return service locator instance. Use it to register or retrieve
    /// services (for e.g. `EventLog`).
    /// @return service instance
    auto &serviceLocator() const noexcept { return d_typeRegistry; }

    /// @brief Return entity registry instance. Use it to register entities and
    /// add/retrieve components to them.
    /// @return service instance
    auto &entityRegistry() noexcept { return d_entityRegistry; }

    /// @brief Return entity registry instance. Use it to register entities and
    /// add/retrieve components to them.
    /// @return service instance
    auto &entityRegistry() const noexcept { return d_entityRegistry; }
};

} // namespace sw::features
