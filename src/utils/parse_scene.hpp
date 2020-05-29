#pragma once
#include "../../third_party/toml11/toml.hpp"
#include <accelerator/bvh.h>
#include <accelerator/linear_list.h>
#include <cameras/pinhole_camera.h>
#include <cores/scene.h>
#include <material/matte_material.h>
#include <math/geometry.hpp>
#include <shapes/sphere.h>

NAMESPACE_BEGIN(DR)
inline void parse_scene(std::string filename, Scene *scene, int *spp) {
  auto data = toml::parse(filename);

  auto title = toml::find<std::string>(data, "title");
  std::cout << "Rendering: " << title << std::endl;

  *spp = toml::find<toml::integer>(data, "spp");
  std::cout << "spp: " << *spp << std::endl;

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
      if (material_toml.at("type").as_string() == "matte") {
        auto albedo = toml::get<std::vector<float>>(material_toml.at("albedo"));
        Vector3f emission{};
        if (material_toml.count("emission")) {
          auto tmp =
              toml::get<std::vector<float>>(material_toml.at("emission"));
          emission = {tmp[0], tmp[1], tmp[2]};
        }
        mat_ptr = std::make_shared<MatteMaterial>(
            Vector3f{albedo[0], albedo[1], albedo[2]}, emission);
      }
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
      }

      auto object_ptr =
          std::make_shared<GeometricPrimitive>(shape_ptr, mat_ptr);
      objects.push_back(object_ptr);
    }
    //  for (int i = 0; i < 100; i++) {
    //    // clang-format off
    //    Matrix4 mat4(1.0, 0.0, 0.0, 0.0,
    //                 0.0, 1.0, 0.0, 0.0,
    //                 0.0, 0.0, 1.0, -2.0,
    //                 0.0, 0.0, 0.0, 1.0);

    //    mat4.m[0][3] += (get_random_float() * 2  - 1) * 10;
    //    mat4.m[1][3] += (get_random_float() * 2  - 1) * 10;
    //    mat4.m[2][3] += (get_random_float() * 2  - 1) * 10;
    //  auto trans = std::make_shared<Transform>(mat4);
    //  auto trans_inv =
    //  std::make_shared<Transform>(Transform::Inverse(*trans)); auto shape_ptr
    //  = std::make_shared<Sphere>(trans, trans_inv, 0, get_random_float() * 5);
    //  auto mat_ptr = std::make_shared<MatteMaterial>(
    //          Vector3f{get_random_float(),get_random_float(),get_random_float()});
    //  auto object_ptr = std::make_shared<GeometricPrimitive>(shape_ptr,
    //  mat_ptr);
    //   objects.push_back(object_ptr);
    //  }
    auto bvh_tree = std::make_shared<BVHTree>(objects);
    scene->add(bvh_tree);
  } catch (...) {

    return;
  }
}
NAMESPACE_END(DR)
