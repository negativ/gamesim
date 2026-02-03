#include <fstream>

#include <Core/Registry.hpp>
#include <Features/AllUnits.hpp>
#include <Features/Engine.hpp>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/TypeRegistry.hpp>

namespace swcc = sw::core::components;
namespace swu = sw::units;

int main(int argc, char **argv) {
    using namespace sw;

    if (argc != 2) {
        throw std::runtime_error(
            "Error: No file specified in command line argument");
    }

    std::ifstream file(argv[1]);
    if (!file) {
        throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
    }

    TypeRegistry typeRegistry;
    typeRegistry.emplace<EventLog>();

    sw::core::Registry entityRegistry;
    sw::features::GameContext gc{typeRegistry, entityRegistry};
    sw::features::Engine engine{gc};

    io::CommandParser parser;
    parser
        .add<io::CreateMap>([&](auto command) {
            engine.createMap(command.width, command.height);
        })
        .add<io::SpawnSwordsman>([&](auto command) {
            engine.spawn<swu::Swordsman>(command.unitId,
                                         swcc::Position{command.x, command.y},
                                         command.hp, command.strength);
        })
        .add<io::SpawnHunter>([&](auto command) {
            engine.spawn<swu::Hunter>(
                command.unitId, swcc::Position{command.x, command.y}, command.hp,
                command.strength, command.agility, command.range);
        })
        .add<io::March>([&](auto command) {
            engine.emitMarchOrder(command.unitId, command.targetX, command.targetY);
        });

    parser.parse(file);

    engine.run();

    return 0;
}
