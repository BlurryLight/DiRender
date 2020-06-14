#pragma once
#include "../../third_party/toml11/toml.hpp"
#include <accelerator/bvh.h>
#include <accelerator/linear_list.h>
#include <cameras/pinhole_camera.h>
#include <cores/scene.h>
#include <material/dielectric_material.h>
#include <material/glass_material.h>
#include <material/matte_material.h>
#include <math/geometry.hpp>
#include <shapes/sphere.h>
#include <texture/checker_texture.h>
#include <texture/constant_texture.h>
#include <utils/OBJ_Loader_wrapper.h>

NAMESPACE_BEGIN(DR)
NAMESPACE_BEGIN(impl)
template <typename T>
inline void parse_camera_data(Scene *scene, const T &data) {
  const auto &cams_toml = toml::find(data, "cameras");
  const auto &cam_vec_toml =
      toml::find<std::vector<toml::table>>(cams_toml, "camera");
  for (const auto &cam_toml : cam_vec_toml) {
    const auto &origin_toml =
        toml::get<std::vector<float>>(cam_toml.at("origin"));
    const auto &up_toml = toml::get<std::vector<float>>(cam_toml.at("up"));
    const auto &lookat_toml =
        toml::get<std::vector<float>>(cam_toml.at("lookat"));
    auto fov_toml = toml::get<float>(cam_toml.at("fov"));
    uint height_toml = toml::get<toml::integer>(cam_toml.at("film_height"));
    uint width_toml = toml::get<toml::integer>(cam_toml.at("film_width"));
    Point3f origin;
    Vector3f up;
    Vector3f lookat;
    for (int i = 0; i < 3; i++) {
      origin[i] = origin_toml[i];
      up[i] = up_toml[i];
      lookat[i] = lookat_toml[i];
    }
    std::shared_ptr<Camera> cam = nullptr;
    if (cam_toml.at("type").as_string() == "pinhole") {
      cam = std::make_shared<PinholeCamera>(origin, up, lookat, fov_toml,
                                            height_toml, width_toml);
    }
    scene->add(cam);
  }
}

template <typename T>
inline void parse_material_data(const T &material_toml,
                                std::shared_ptr<Material> &mat_ptr,
                                bool &has_emission) {
  // material parse
  has_emission = false;
  std::string mat_type = toml::get<std::string>(material_toml.at("type"));
  std::cout << mat_type << std::endl;
  static std::array<std::string, 3> supported_materials{"matte", "glass",
                                                        "dielectric"};
  if (std::find(supported_materials.begin(), supported_materials.end(),
                mat_type) != supported_materials.end()) {
    // constant texture
    std::shared_ptr<Texture> texture_ptr = nullptr;
    std::string texture_type = material_toml.at("texture").as_string();
    if (texture_type == "constant") {
      auto albedo = toml::get<std::vector<float>>(material_toml.at("albedo"));
      texture_ptr = std::make_shared<ConstantTexture>(
          Vector3f{albedo[0], albedo[1], albedo[2]});
    } else if (texture_type == "checker") {
      auto albedo_list = toml::get<std::vector<std::vector<float>>>(
          material_toml.at("albedo"));
      texture_ptr = std::make_shared<CheckerTexture>(
          Vector3f{albedo_list[0][0], albedo_list[0][1], albedo_list[0][2]},
          Vector3f{albedo_list[1][0], albedo_list[1][1], albedo_list[1][2]});
    } else {
      throw std::runtime_error("Unsupported texture type:" + texture_type);
      std::exit(-1);
    }
    Vector3f emission{};
    if (material_toml.count("emission")) {
      auto tmp = toml::get<std::vector<float>>(material_toml.at("emission"));
      emission = {tmp[0], tmp[1], tmp[2]};
      has_emission = true;
    }
    float index_of_refraction = 1.0;
    if (material_toml.count("ior")) {
      index_of_refraction = toml::get<float>(material_toml.at("ior"));
    }

    //    if (material_toml.at("type").as_string() == "matte") {
    if (mat_type == "matte") {
      mat_ptr = std::make_shared<MatteMaterial>(texture_ptr, emission);
    } else if (mat_type == "glass") {
      mat_ptr = std::make_shared<GlassMaterial>(texture_ptr, emission);
    } else if (mat_type == "dielectric") {
      mat_ptr = std::make_shared<DielectricMaterial>(texture_ptr,
                                                     index_of_refraction);
    }
    return;
  }
  throw std::runtime_error("Unsupported material type: " +
                           toml::get<std::string>(material_toml.at("type")));
}
NAMESPACE_END(impl)
inline void parse_scene(std::string filename, Scene *scene, int *spp) {
  auto data = toml::parse(filename);

  auto title = toml::find<std::string>(data, "title");
  std::cout << "Rendering: " << title << std::endl;

  *spp = toml::find<toml::integer>(data, "spp");
  std::cout << "spp: " << *spp << std::endl;
  const auto &objects_toml = toml::find(data, "objects");
  try {
    const auto &names_vec =
        toml::find<std::vector<toml::table>>(objects_toml, "name");
    const auto &transforms_vec =
        toml::find<std::vector<toml::table>>(objects_toml, "transform");
    const auto &materials_vec =
        toml::find<std::vector<toml::table>>(objects_toml, "material");
    const auto &shapes_vec =
        toml::find<std::vector<toml::table>>(objects_toml, "shape");

    // parse  camera
    DR::impl::parse_camera_data(scene, data);

    // parse objects
    std::vector<std::shared_ptr<Primitive>> objects;
    for (uint i = 0; i < names_vec.size(); i++) {
      std::cout << "Objects: " << names_vec[i].at("name") << std::endl;
      auto mat_toml =
          toml::get<std::vector<float>>(transforms_vec[i].at("matrix4"));
      Matrix4 mat;
      for (int j = 0; j < 16; j++) {
        mat.m[j / 4][j % 4] = mat_toml.at(j);
      }
      auto trans = std::make_shared<Transform>(mat);
      auto trans_inv = std::make_shared<Transform>(Transform::Inverse(*trans));
      Transform::TransformTable.insert({*trans, trans});
      Transform::TransformTable.insert({*trans_inv, trans_inv});
      auto material_toml = materials_vec[i];
      std::shared_ptr<Material> mat_ptr = nullptr;
      bool has_emission = false;
      DR::impl::parse_material_data(material_toml, mat_ptr, has_emission);
      // shape parse
      auto shape_toml = shapes_vec[i].at("type").as_string();
      std::shared_ptr<Shape> shape_ptr = nullptr;
      if (shape_toml == "sphere") {
        float radius = 1.0f;
        bool reverse = false;
        if (shapes_vec[i].find("radius") != shapes_vec[i].end())
          radius = shapes_vec[i].at("radius").as_floating();
        if (shapes_vec[i].find("reverse") != shapes_vec[i].end())
          reverse = shapes_vec[i].at("reverse").as_boolean();
        trans = Transform::TransformTable.at(*trans);
        trans_inv = Transform::TransformTable.at(*trans_inv);
        shape_ptr = std::make_shared<Sphere>(trans, trans_inv, reverse, radius);
        auto object_ptr =
            std::make_shared<GeometricPrimitive>(shape_ptr, mat_ptr);
        objects.push_back(object_ptr);
        if (has_emission) {
          scene->light_shapes_.push_back(object_ptr);
        }
      } else if (shape_toml == "obj") {
        trans = Transform::TransformTable.at(*trans);
        std::string path = shapes_vec[i].at("path").as_string();
        Model model(trans, path, mat_ptr);
        objects.push_back(model.model_ptr);
        if (has_emission) {
          scene->light_shapes_.push_back(model.model_ptr);
        }
      }
    }
    auto bvh_tree = std::make_shared<BVHTree>(objects);
    scene->add(bvh_tree);
  } catch (const std::exception &e) {
    std::cerr << "Exception captured: " << '\n' << e.what() << std::endl;
    return;
  }
}
NAMESPACE_END(DR)
