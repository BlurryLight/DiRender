#pragma once
#include <filesystem>
#include <string>
#include <utils/di_global.h>
#include <vector>
NAMESPACE_BEGIN(DR)
class ResourcePathSearcher {
private:
  using Path = std::filesystem::path;
  std::vector<Path> search_paths_;

public:
  static Path root_path;
  ResourcePathSearcher();
  ResourcePathSearcher(const ResourcePathSearcher &) = delete;
  ResourcePathSearcher &operator=(const ResourcePathSearcher &) = delete;
  ResourcePathSearcher(ResourcePathSearcher &&) = delete;
  void add_path(const std::string &path);
  // pass a vector such as {"src","cores","xxx.cc"}
  std::string find_path(std::vector<std::string> filenames) const;
  std::string find_path(const std::string &filename) const;
};

NAMESPACE_END(DR)
