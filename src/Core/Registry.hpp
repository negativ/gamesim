#pragma once

#include <deque>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include <Core/Types.hpp>

namespace sw::core {

/// @brief The entity registry class
class Registry {
private:
    /// @brief Interface for any storage-like object
    struct StorageConcept {
        virtual ~StorageConcept() = default;

        /// @brief Remove entity from the storage.
        /// @param id entity id
        /// @returns `true` if entity was deleted from the storage.
        virtual bool erase(EntityId id) = 0;

        /// @brief Check that entity registered in the storage.
        /// @param id entity id
        /// @returns `true` if such an entity exists in the storage.
        virtual bool contains(EntityId id) const noexcept = 0;
    };

    /// @brief Storage implementation for component of type `T`.
    template <typename T> class Storage final : public StorageConcept {
        std::unordered_map<EntityId, std::shared_ptr<T>> data;

    public:
        /// @brief Adds component to the entity.
        /// @param id entity id.
        /// @param val component value to add.
        void add(EntityId id, T val) {
            data[id] = std::make_shared<T>(std::move(val));
        }

        /// @brief Remove entity from the storage.
        /// @param id entity id
        /// @returns `true` if entity was deleted from the storage.
        bool erase(EntityId id) override {
            if (auto it = data.find(id); it != end(data)) {
                data.erase(it);

                return true;
            }

            return false;
        }

        /// @brief Check that entity registered in the storage.
        /// @param id entity id
        /// @returns `true` if such an entity exists in the storage.
        bool contains(EntityId id) const noexcept override {
            return data.count(id);
        }

        /// @brief Get component associated with the entity.
        /// @param id entity id
        /// @returns Valid `std::shared_ptr<T>  if such a component for entity
        /// exists, `nullptr` otherwise.
        std::shared_ptr<T> get(EntityId id) noexcept {
            auto it = data.find(id);

            return it == end(data) ? nullptr : it->second;
        }

        /// @brief Get component associated with the entity.
        /// @param id entity id
        /// @returns Valid `std::shared_ptr<const T>` if such a component for entity
        /// exists, `nullptr otherwise.
        std::shared_ptr<const T> get(EntityId id) const noexcept {
            auto it = data.find(id);

            return it == end(data) ? nullptr : it->second;
        }
    };

    std::unordered_map<std::type_index, std::unique_ptr<StorageConcept>>
        storageIndex;
    EntityList allEntities;
    EntityList removalQueue;

    /// @brief Try to obtain component storage from the index.
    template <typename T> Storage<T> *tryGetStorage() noexcept {
        std::type_index type = typeid(T);
        auto it = storageIndex.find(type);

        if (it == end(storageIndex))
            return nullptr;

        return static_cast<Storage<T> *>(it->second.get());
    }

    /// @brief Try to obtain component storage from the index.
    template <typename T> const Storage<T> *tryGetStorage() const noexcept {
        std::type_index type = typeid(T);
        auto it = storageIndex.find(type);

        if (it == end(storageIndex))
            return nullptr;

        return static_cast<const Storage<T> *>(it->second.get());
    }

    /// @brief Obtain component storage from the index.
    /// @note If storage doesn't exist it will be created.
    template <typename T> Storage<T> &storage() {
        std::type_index type = typeid(T);
        auto it = storageIndex.find(type);

        if (it == end(storageIndex))
            it = storageIndex.insert({type, std::make_unique<Storage<T>>()}).first;

        return *static_cast<Storage<T> *>(it->second.get());
    }

public:
    Registry() = default;

    Registry(const Registry &) = delete;
    Registry &operator=(const Registry &) = delete;

    Registry(Registry &&) noexcept = default;
    Registry &operator=(Registry &&) noexcept = default;

    /// @brief Register entity within this context.
    /// @param id entity id.
    /// @returns `true` if entity registered in registry.
    bool registerEntity(EntityId id);

    /// @brief Unregister entity within this context.
    /// @param id entity id.
    /// @note There is no check that `id` is previously registered.
    void unregisterEntity(EntityId id);

    /// @brief Add components to the intity.
    /// @param id
    /// @param comps components to add.
    template <typename... Components>
    void add(EntityId id, Components &&...comps) {
        static_assert(sizeof...(Components) > 0);
        (storage<std::decay_t<Components>>().add(id,
                                                 std::forward<Components>(comps)),
         ...);
    }

    /// @brief Call functor for each entity registered in registry.
    /// @param functor `void(EntityId)`-like functor
    template <typename F> void forEach(const F &functor) const {
        for (const auto &id : allEntities)
            functor(id);
    }

    /// @brief Call functor for each entity registered in registry which has
    /// `Component`.
    /// @param functor `void(EntityId,std::shared_ptr<const Component>)`-like
    /// functor
    template <typename Component, typename F>
    void forEachWith(const F &functor) const {
        for (auto id : allEntities) {
            if (auto comp = get<Component>(id); comp)
                functor(id, comp);
        }
    }

    /// @brief Remove component from the entity.
    /// @param id entity id
    /// @returns `true` if component was successfully removed from the entity,
    /// `false` otherwise.
    template <typename T> bool erase(EntityId id) noexcept {
        if (auto *storage = tryGetStorage<T>(); storage)
            return storage->erase(id);

        return false;
    }

    /// @brief Perform cleanup of unregistered enities.
    void cleanup();

    /// @brief entities
    /// @return
    ///
    const EntityList &entities() const noexcept;

    /// @brief Get component associated with the entity.
    /// @param id entity id
    /// @returns Valid `std::shared_ptr<T> if such a component for entity exists,
    /// `nullptr otherwise.
    template <typename T>
    [[nodiscard]] std::shared_ptr<T> get(EntityId id) noexcept {
        if (auto *storage = tryGetStorage<T>(); storage)
            return storage->get(id);

        return nullptr;
    }

    /// @brief Get component associated with the entity.
    /// @param id entity id
    /// @returns Valid `std::shared_ptr<const T>` if such a component for entity
    /// exists, `nullptr` otherwise.
    template <typename T>
    [[nodiscard]] std::shared_ptr<const T> get(EntityId id) const noexcept {
        if (auto *storage = tryGetStorage<T>(); storage)
            return storage->get(id);

        return nullptr;
    }
};

} // namespace sw::core
