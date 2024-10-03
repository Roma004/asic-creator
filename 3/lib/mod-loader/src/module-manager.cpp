#include "mod-loader/module-manager.hpp"
#include "mod-loader/dl-loader.hpp"
#include "mod-loader/module-interface.hpp"
#include <string>

ModuleManager &ModuleManager::get_instance() {
    static ModuleManager manager;
    return manager;
}

void ModuleManager::add_module(const ModuleInterface &mod) {
    auto res = mods.emplace(std::string(mod.get_info().name), std::ref(mod));
    if (!res.second) {
        auto old_module = res.first->second;
        throw ModuleLoadExc(std::format(
            "duplicate module: {}:{} overloads {}:{}",
            mod.get_info().name,
            mod.get_info().version,
            old_module.get().get_info().name,
            old_module.get().get_info().version
        ));
    }
}

void ModuleManager::remove_module(const std::string &name) { mods.erase(name); }

const ModuleManager::module_map_t &ModuleManager::get_modules() { return mods; }
const ModuleInterface &ModuleManager::get_module(const std::string &name) {
    auto res = mods.find(name);
    if (res == mods.end())
        throw ModuleLoadExc(
            std::format("Requested module with name `{}` does not exist!", name)
        );
    return res->second;
}

ModuleToken::ModuleToken(const ModuleInterface &mod) : mod(mod) {
    ModuleManager::get_instance().add_module(mod);
}

ModuleToken::~ModuleToken() {
    ModuleManager::get_instance().remove_module(mod.get_info().name);
}
