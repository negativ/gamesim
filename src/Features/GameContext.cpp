#include <Features/GameContext.hpp>

namespace sw::features {

GameContext::Events &&GameContext::extractEvents() noexcept {
    return std::move(eventQueue);
}

} // namespace sw::features
