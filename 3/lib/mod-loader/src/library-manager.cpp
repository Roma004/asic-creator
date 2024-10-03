#include "mod-loader/dl-loader.hpp"
#include "mod-loader/library-manger.hpp"
#include <filesystem>

namespace fs = std::filesystem;

void LibraryManager::scan(
    const std::string &path, const std::string &extension
) {
    fs::path dirPath(path);
    for (auto &file : fs::directory_iterator(dirPath)) {
        if ((file.is_regular_file() || file.is_symlink())
            && file.path().extension() == extension) {
            libraries.emplace_back(file.path().c_str());
        }
    }
}

void LibraryManager::clear() { libraries.clear(); }

const std::vector<DlLoader> &LibraryManager::get_libraries() const {
    return libraries;
}

