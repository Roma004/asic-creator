#pragma once

#include <asic-engine/exceptions.hpp>
#include <memory>
#include <unordered_map>

// TODO Documantation
template <class T> class ComponentRegistry {
  public:
    ComponentRegistry() = default;
    ComponentRegistry(const ComponentRegistry &) = delete;
    ComponentRegistry(ComponentRegistry &&) = default;
    ComponentRegistry &operator=(const ComponentRegistry &) = delete;

    void
    add_component(const std::string &name, std::unique_ptr<T> &&component) {
        auto res = registry.emplace(name, std::move(component));
        if (!res.second)
            throw KeyError(std::format(
                "Component with name `{}` is already registered", name
            ));
    }

    T &get_component(const std::string &name) const {
        auto it = registry.find(name);
        if (it == registry.end())
            throw KeyError(
                std::format("No component with name `{}` registered", name)
            );
        return *it->second;
    }

    bool has_component(const std::string &name) const {
        return registry.find(name) != registry.end();
    }

  private:
    std::unordered_map<std::string, std::unique_ptr<T>> registry;
};
