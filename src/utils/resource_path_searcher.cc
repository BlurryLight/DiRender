#include <iostream>
#include <utils/cmake_vars.h>
#include <utils/resource_path_searcher.h>
using namespace DR;
fs::path ResourcePathSearcher::root_path = Path(
    ROOT_DIR); // ROOT_DIR is defined by CMake, which is the project root dir
ResourcePathSearcher::ResourcePathSearcher() {
  search_paths_.push_back(root_path);
}

void ResourcePathSearcher::add_path(const std::string &path) {
  Path p(path);
  if (!p.is_absolute())
    p = fs::absolute(p);
  search_paths_.push_back(p);
}

std::string ResourcePathSearcher::find_path(const std::string &filename) const {
  for (const Path &p : search_paths_) {
    auto path = p / filename;
    if (fs::exists(path)) {
      std::cout << "Resource: " << path << " found!" << std::endl;
      // It may cause problems in Windows because Windows native path is encoded
      // in UTF16LE To Handle this problem will need complex machanism like
      // writing wstring overloads for all related functions.I won't bother to
      // do that.
      return path.is_absolute() ? path.u8string()
                                : fs::absolute(path).u8string();
    }
  }
  throw std::runtime_error("ResourcePathSearch cannot find " +
                           std::string(filename));
}

std::string
ResourcePathSearcher::find_path(std ::vector<std::string> filenames) const {
  Path ps;
  for (const auto &i : filenames) {
    ps /= i;
  }

  for (const Path &p : search_paths_) {
    auto path = p / ps;
    if (fs::exists(path)) {
      std::cout << "Resource: " << path << " found!" << std::endl;
      return path.is_absolute() ? path.u8string()
                                : fs::absolute(path).u8string();
    }
  }
  throw std::runtime_error("ResourcePathSearch cannot find " +
                           *filenames.rbegin());
}
