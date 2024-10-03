#pragma once
#include <string>

struct ModuleBasicInfo {
    const std::string &name;
    const std::string &version;
};

class ModuleInterface {
  public:
    virtual ~ModuleInterface() {};
    virtual const ModuleBasicInfo &get_info() const = 0;
    virtual const std::string &get_component_type() const = 0;
};
