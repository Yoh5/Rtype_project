#pragma once
#include "entity.hpp"
#include "sparseArray.hpp"
#include "system.hpp"
#include "../Error/error.hpp"
#include <unordered_set>
#include <set>

namespace BJENGINE
{
    class System;
    class registry
    {
    public:
        using sparseArrayMap = std::unordered_map<std::type_index, std::any>;

        using systems = std::map<std::type_index, std::shared_ptr<System>>;

        registry() {
            _init_state = true;
        }
        ~registry() {
            for (auto [tag, sys] : _systems)
                sys->stop();
        } 
        template <class Component>
        void register_component()
        {
            if (_components_arrays.contains(std::type_index(typeid(Component)))) return;
            _components_arrays[std::type_index(typeid(Component))] = std::any(sparse_array<Component>());
        }

        template <typename Component>
        sparse_array<Component> &get_components()
        {
            if (!_components_arrays.contains(typeid(Component)))
            {
                throw BJENGINE::Error("get_components", "31", "Unregistered Component" + (std::string)std::type_index(typeid(Component)).name());
            }
            return std::any_cast<sparse_array<Component> &>(_components_arrays[std::type_index(typeid(Component))]);
        }

        template<typename Component>
        bool isComponentRegistered() {
            return _components_arrays.contains(std::type_index(typeid(Component)));
        }

        template <typename Component>
        Component &get_component(entity entt)
        {
            if (!_components_arrays.contains(typeid(Component)))
            {
                throw BJENGINE::Error("get_component", "41", "Unregistered Component" + (std::string)std::type_index(typeid(Component)).name());
            }
            return *(std::any_cast<sparse_array<Component> &>(_components_arrays[std::type_index(typeid(Component))]))[entt];
        }

        template <typename Component>
        void attach_component(entity entt, Component component)
        {
                if (!_components_arrays.contains(typeid(Component)))
                {
                    throw BJENGINE::Error("attach_component", "51", "Unregistered Component" + (std::string)std::type_index(typeid(Component)).name()) ;
                }
            std::any_cast<sparse_array<Component> &>(_components_arrays[std::type_index(typeid(Component))]).insert_at(entt, component);
        }

        template <typename... Args>
        void attach_components(entity entt, Args... args)
        {
            ((attach_component(entt, args)), ...);
        }

        template <class Component>
        void detach_component(entity entt, Component component)
        {
            if (!_components_arrays.contains(typeid(Component)))
            {
                throw BJENGINE::Error("detach_component", "67", "Unregistered Component" + (std::string)std::type_index(typeid(Component)).name());
            }

            std::any_cast<sparse_array<Component> &>(_components_arrays[std::type_index(typeid(Component))]).erase(entt);
        }

        template <typename... Args>
        void detach_components(entity entt, Args... args)
        {
            ((detach_component(entt, args)), ...);
        }

        entity spawn_entity()
        {
            entity id = 0;

            if (_suspended.empty() && _entities.empty())
            {
                id = 0;
            }

            if (_suspended.empty() && !_entities.empty())
            {
                id = *max_element(_entities.begin(), _entities.end()) + 1;
            }

            if (!_suspended.empty() && _entities.empty())
            {
                id = *max_element(_suspended.begin(), _suspended.end()) + 1;
            }

            if (!_suspended.empty() && !_entities.empty())
            {
                int max_suspended= *max_element(_suspended.begin(), _suspended.end());
                int max_active = *max_element(_entities.begin(), _entities.end());

                id = (std::max(max_suspended, max_active)) + 1;
            }

            _entities.push_back(id);

            return id;
        }

        void kill_entity(entity entt)
        {
            _dead_entities.push_back(entt);
        }

        std::vector<int> entities() const { return _entities; }

        template <typename type>
        static bool contains(std::vector<type> list, type element)
        {
            return (std::find(list.begin(), list.end(), element) != list.end());
        }

        template <typename TAG>
        void add_system(std::shared_ptr<System> sys)
        {
            _systems[typeid(TAG)] = std::shared_ptr(sys);
        }

        template <typename TAG>
        void run_system()
        {
            if (!_systems.contains(typeid(TAG)))
                throw BJENGINE::Error("run_system", "127", "Unknown Tag" + (std::string) std::type_index(typeid(TAG)).name());
            _systems[typeid(TAG)]->update();
        }

        void run_system(std::type_index type)
        {
            if (!_systems.contains(type))
                throw BJENGINE::Error("run_system", "134", "Unknown Tag");
            _systems[type]->update();

        }

        bool isEntityAlive(entity id)
        {
            return std::find(_entities.begin(), _entities.end(), id) == _entities.end();
        }


        void init_systems()
        {
            if (!_init_state) return;
            for (auto [tag, sys] : _systems)
                sys->start();
        }

        std::unordered_set<std::type_index> get_tags()
        {
            std::unordered_set<std::type_index> result;

            for (auto [key, value] : _systems)
                result.insert(key);
            return result;
        }

        void deActivateEntity(entity idx)
        {
            std::vector<int>::iterator pos;
            if ((pos = std::find(_entities.begin(), _entities.end(), idx))== _entities.end()) {
                throw BJENGINE::Error("deActivateEntity", "164", "Unknown Entity" + std::to_string(idx));
            }
            _entities.erase(pos);
            _suspended.push_back(idx);
        }

        void activateEntity(entity idx)
        {
            std::vector<int>::iterator pos;
            if ((pos = std::find(_suspended.begin(), _suspended.end(), idx)) == _suspended.end()) {
                throw BJENGINE::Error("activateEntity", "164", "Unknown Entity" + std::to_string(idx));
            }
            _entities.push_back(idx);
            _suspended.erase(pos);
        }

    private:

        sparseArrayMap _components_arrays;
        systems _systems;
        std::vector<entity> _entities;
        std::vector<entity> _dead_entities;
        std::vector<entity> _suspended;
        
        bool _init_state;
    };
}
