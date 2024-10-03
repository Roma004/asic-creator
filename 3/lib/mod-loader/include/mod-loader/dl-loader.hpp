#pragma once

#include <dlfcn.h>
#include <format>
#include <stdexcept>
#include <type_traits>

class ModuleLoadExc : public std::runtime_error {
  public:
    ModuleLoadExc(const std::string &msg) : std::runtime_error(msg) {};
};

class DlLoader {
  private:
    const std::string &path;
    void *sym_handle = nullptr;

    void open();
    void close();

  public:
    DlLoader() = delete;
    DlLoader(const std::string &so_path);
    DlLoader(const DlLoader &) = delete;
    DlLoader(DlLoader &&) = default;

    DlLoader &operator=(const DlLoader &) = delete;

    ~DlLoader();

    template <class T>
    T &gey_symbol(const std::string &name)
        requires std::is_pointer_v<T>;
};

template <class T>
T &DlLoader::gey_symbol(const std::string &name)
    requires std::is_pointer_v<T>
{
    auto sym = reinterpret_cast<T>(dlsym(sym_handle, name.c_str()));

    if (!sym) {
        throw ModuleLoadExc(std::format(
            "Unable to load symbol '{}' from module '{}' "
            "due to: `{}`",
            name,
            path,
            dlerror()
        ));
    }

    return sym;
}
