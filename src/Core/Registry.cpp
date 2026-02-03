#include "Core/Registry.hpp"

namespace sw::core {

bool Registry::registerEntity(EntityId id) {
    if (auto it = std::find(allEntities.cbegin(), allEntities.cend(), id);
        it != allEntities.cend())
        return false;

    allEntities.push_back(id);

    return true;
}

void Registry::unregisterEntity(EntityId id) { removalQueue.push_back(id); }

void Registry::cleanup() {
    if (removalQueue.empty())
        return;

    std::sort(removalQueue.begin(), removalQueue.end());

    removalQueue.erase(std::unique(removalQueue.begin(), removalQueue.end()),
                       removalQueue.end());

    for (auto id : removalQueue) {
        for (auto &[type, storage] : storageIndex) {
            storage->erase(id);
        }
        allEntities.erase(std::remove(allEntities.begin(), allEntities.end(), id),
                          allEntities.end());
    }

    removalQueue.clear();
}

const EntityList &Registry::entities() const noexcept { return allEntities; }

} // namespace sw::core
