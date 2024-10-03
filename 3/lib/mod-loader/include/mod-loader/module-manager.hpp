#pragma once
#include "module-interface.hpp"
#include <functional>
#include <unordered_map>

class ModuleManager {
  public:
    using module_map_t = std::
        unordered_map<std::string, std::reference_wrapper<const ModuleInterface>>;

    void add_module(const ModuleInterface &mod);
    void remove_module(const std::string &name);

    const module_map_t &get_modules();
    const ModuleInterface &get_module(const std::string &name);

    static ModuleManager &get_instance();

  private:
    module_map_t mods;

    ModuleManager() = default;
};

class ModuleToken {
  public:
    ModuleToken(const ModuleInterface &mod);

    ~ModuleToken();

  private:
    const ModuleInterface &mod;
};
