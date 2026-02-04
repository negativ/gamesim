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



} // namespace sw::features
