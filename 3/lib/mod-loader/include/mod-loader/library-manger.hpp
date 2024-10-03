#pragma once

#include <dlfcn.h>
#include <vector>

#include "dl-loader.hpp"

class LibraryManager {
  public:
    void scan(const std::string &path, const std::string &extension);

    void clear();

    const std::vector<DlLoader> &get_libraries() const;

  private:
    std::vector<DlLoader> libraries;
};
