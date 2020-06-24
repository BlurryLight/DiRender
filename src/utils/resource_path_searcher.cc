#include <iostream>
#include <utils/resource_path_searcher.h>
using namespace DR;
std::filesystem::path ResourcePathSearcher::root_path = Path(
    ROOT_DIR); // ROOT_DIR is defined by CMake, which is the project root dir
ResourcePathSearcher::ResourcePathSearcher() {
  search_paths_.push_back(root_path);
}

void ResourcePathSearcher::add_path(const std::string &path) {
  Path p(path);
  if (!p.is_absolute())
    p = std::filesystem::absolute(p);
  search_paths_.push_back(p);
}

std::string ResourcePathSearcher::find_path(const std::string &filename) const {
  for (const Path &p : search_paths_) {
    auto path = p / filename;
    if (std::filesystem::exists(path)) {
      std::cout << "Resource: " << path << " found!" << std::endl;
      return path.is_absolute() ? path.u8string()
                                : std::filesystem::absolute(path).u8string();
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
    if (std::filesystem::exists(path)) {
      std::cout << "Resource: " << path << " found!" << std::endl;
      return path.is_absolute() ? path.u8string()
                                : std::filesystem::absolute(path).u8string();
    }
  }
  throw std::runtime_error("ResourcePathSearch cannot find " +
                           *filenames.rbegin());
}
