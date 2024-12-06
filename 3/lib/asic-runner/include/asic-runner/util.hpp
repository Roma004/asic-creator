#pragma once
#include <asic-engine/exceptions.hpp>
#include <mod-loader/module-settings.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

template <class T> class Dict : public std::unordered_map<std::string, T> {
  public:
    Dict(const nlohmann::json &j) : std::unordered_map<std::string, T>() {
        for (auto &&[key, val] : j.items()) { this->emplace(key, T(val)); }
    }

    bool has_config(const std::string &name) {
        return this->find(name) != this->end();
    }
};

const nlohmann::json &
json_get_obj(const nlohmann::json &j, const std::string &name);

ModuleSettings settings_from_json(const nlohmann::json &j);
