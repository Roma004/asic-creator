#include <asic-engine/asic/util.hpp>

const nlohmann::json &
json_get_obj(const nlohmann::json &j, const std::string &name) {
    if (j.find(name) == j.end())
        throw KeyError(std::format("Parsing config no key `{}` found", name));
    return j[name];
}

ModuleSettings settings_from_json(const nlohmann::json &j) {
    ModuleSettings stt;
    if (j.type() == nlohmann::detail::value_t::null) return stt;
    if (j.type() != nlohmann::detail::value_t::object)
        throw std::runtime_error(std::format(
            "Field 'settings' must be either `json::object` or `json::null`"
        ));
    for (auto &&[key, val] : j.items()) {
        switch (val.type()) {
        case nlohmann::detail::value_t::string: stt.add_string(key, val); break;
        case nlohmann::detail::value_t::number_integer:
        case nlohmann::detail::value_t::number_unsigned:
            stt.add_int(key, val);
            break;
        case nlohmann::detail::value_t::number_float:
            stt.add_float(key, val);
            break;
        default:
            throw std::runtime_error(
                std::format("ModuleSetting key `{}` has unsupported type!", key)
            );
        }
    }
    return stt;
}
