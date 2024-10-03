#pragma once
#include "asic-engine/exceptions.hpp"
#include "mod-loader/dl-loader.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>

class ModuleSettings {
  public:
    void add_int(const std::string &name, int64_t val) {
        auto res = ints.emplace(name, val);
        if (!res.second) throw ModuleLoadExc("Int Settings key duplication");
    }

    void add_float(const std::string &name, double val) {
        auto res = floats.emplace(name, val);
        if (!res.second) throw ModuleLoadExc("Float Settings key duplication");
    }

    void add_string(const std::string &name, const std::string &val) {
        auto res = strings.emplace(name, val);
        if (!res.second) throw ModuleLoadExc("String Settings key duplication");
    }

    int64_t get_int(const std::string name) const {
        auto it = ints.find(name);
        if (it == ints.end())
            throw KeyError(std::format(
                "Int Module Setting with name {} does not exist", name
            ));
        return it->second;
    }

    double get_float(const std::string name) const {
        auto it = floats.find(name);
        if (it == floats.end())
            throw KeyError(std::format(
                "Float Module Setting with name {} does not exist", name
            ));
        return it->second;
    }

    const std::string &get_string(const std::string name) const {
        auto it = strings.find(name);
        if (it == strings.end())
            throw KeyError(std::format(
                "String Module Setting with name {} does not exist", name
            ));
        return it->second;
    }

  private:
    std::unordered_map<std::string, int64_t> ints;
    std::unordered_map<std::string, double> floats;
    std::unordered_map<std::string, std::string> strings;
};
