#pragma once
#include "../../third_party/toml11/toml.hpp"

namespace DR {
struct Scene;
struct Material;
class Point3f;
using vec3 = class Vector3f;
void parse_scene(std::string filename, Scene *scene);
namespace impl {

using cam_return_type =
std::vector<std::tuple<Point3f, vec3, vec3, float, float, float, std::string,bool,bool>>;
// This function is also called by debugger to render in OpenGL
cam_return_type  parse_camera_data_impl(const toml::value &data);
void parse_camera_data(Scene *scene, const toml::value &data);
void parse_material_data(const toml::value &material_toml,
                         std::shared_ptr<Material> &mat_ptr,
                         bool &has_emission);

// TODO: add more type
void make_render(Scene *scene, int spp, const std::string &type = "pathtracer");

} // namespace impl
} // namespace DR
