#pragma once
#include "third_party/glm/glm/glm.hpp"
#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
namespace DR_D {
inline std::vector<std::array<glm::vec3, 3>> parse_ray_log(std::string path) {
  std::vector<std::array<glm::vec3, 3>> res;
  glm::vec3 origin;
  glm::vec3 isect_coords;
  glm::vec3 isect_normal;
  //  std::array<glm::vec3,3> line;
  std::ifstream f_in(path, std::ios_base::in);
  if (!f_in.is_open())
    throw std::runtime_error("Debugger: Cannot open " + path);
  std::string line;
  char seperator;
  while (std::getline(f_in, line)) {
    std::istringstream line_stream(line);
    for (int i = 0; i < 3; i++) {
      line_stream >> origin[i];
      line_stream >> seperator;
    }

    for (int i = 0; i < 3; i++) {
      line_stream >> isect_coords[i];
      line_stream >> seperator;
    }

    for (int i = 0; i < 3; i++) {
      line_stream >> isect_normal[i];
      line_stream >> seperator;
    }
    std::array<glm::vec3, 3> line;
    line[0] = origin;
    line[1] = isect_coords;
    line[2] = isect_normal;
    res.push_back(std::move(line));
  }
  return res;
}

} // namespace DR_D
