#include <dlfcn.h>
#include <format>
#include "mod-loader/dl-loader.hpp"

void DlLoader::open() {
    if (sym_handle)
        throw ModuleLoadExc(std::format("Module '{}' is already loaded", path));
    if (!(sym_handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_NOW)))
        throw ModuleLoadExc(std::format(
            "Unable to load module '{}' due to: `{}`", path, dlerror()
        ));
}

void DlLoader::close() {
    if (!sym_handle) return;
    if (dlclose(sym_handle) != 0)
        throw ModuleLoadExc(std::format(
            "Unable to close module '{}' due to: `{}`", path, dlerror()
        ));
    sym_handle = nullptr;
}

DlLoader::DlLoader(const std::string &so_path) : path(so_path) { open(); }
DlLoader::~DlLoader() { close(); }
