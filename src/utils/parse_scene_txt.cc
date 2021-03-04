//
// Created by panda on 2021/3/4.
//

#include <accelerator/bvh.h>
#include <cameras/pinhole_camera.h>
#include <cores/scene.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <material/matte_material.h>
#include <material/phong_material.h>
#include <math/matrix.hpp>
#include <shapes/quad.h>
#include <shapes/sphere.h>
#include <sstream>
#include <stack>
#include <string>
#include <texture/constant_texture.h>
#include <utils/OBJ_Loader_wrapper.h>
#include <utils/parse_scene_txt.hh>
using namespace DR;

bool impl::readvals(std::stringstream &s, const int numvals, float *values) {
  for (int i = 0; i < numvals; i++) {
    s >> values[i];
    if (s.fail()) {
      std::cout << "Failed reading value " << i << " will skip" << s.str()
                << "\n";
      return false;
    }
  }
  return true;
}
std::vector<std::string> impl::split(const std::string &str,
                                     const std::string &delim) {
  std::vector<std::string> tokens;
  size_t prev = 0, pos = 0;
  do {
    pos = str.find(delim, prev);
    if (pos == std::string::npos)
      pos = str.length();
    std::string token = str.substr(prev, pos - prev);
    if (!token.empty())
      tokens.push_back(token);
    prev = pos + delim.length();
  } while (pos < str.length() && prev < str.length());
  return tokens;
}
void impl::parse_scene_txt(std::string filename, Scene *scene) {
  std::string str, cmd;
  std::ifstream in;
  int spp = 8;
  in.open(filename);
  int numlights = 0;
  int numobjects = 0;
  int width = 0, height = 0;
  if (in.is_open()) {

    // I need to implement a matrix stack to store transforms.
    // This is done using standard STL Templates
    // trans, trans_inv
    std::stack<mat4> trans_stack;
    trans_stack.push(mat4(1.0)); // identity

    // keep state during parsing
    struct MaterialProperty {
      vec3 diffuse;
      vec3 specular;
      vec3 emission;
      float shininess;
      vec3 ambient{0.2};
    } mp{};

    struct lightProperty {
      vec3 emission;
      vec3 attenuation{1, 0, 0};
    } lp;

    // parse triangles
    std::vector<std::string> vertices;
    std::vector<std::string> indices;
    std::string vertices_str;
    int obj_index = 0;
    std::vector<std::shared_ptr<Primitive>> objects;
    auto write_obj_then_load = [&]() {
      if (vertices_str.empty()) {
        for (const auto &line : vertices) {
          std::string tmp = "v ";
          auto tokens = split(line, " ");
          assert(tokens.size() > 2);
          for (size_t i = 1; i < tokens.size(); i++) {
            tmp += tokens[i];
            tmp += " ";
          }
          tmp.erase(tmp.end() - 1);
          tmp += '\n';
          vertices_str += tmp;
        }
      }
      std::string vertices_index_str;
      for (const auto &line : indices) {
        std::string tmp = "f ";
        auto tokens = split(line, " ");
        assert(tokens.size() > 2);
        for (size_t i = 1; i < tokens.size(); i++) {
          tmp += std::to_string(
              std::stoi(tokens[i]) +
              1); // the face id begins with 0 at CSE168,but 1 at obj
          tmp += " ";
        }
        tmp.erase(tmp.end() - 1);
        tmp += '\n';
        vertices_index_str += tmp;
      }
      vertices.clear();
      indices.clear();
      std::string path = "obj_" + std::to_string(obj_index++) + ".obj";
      std::ofstream obj(path);
      obj << vertices_str;
      obj << vertices_index_str;
      obj << "# Ambient: " << mp.ambient; // for debug use
      //      obj << "# Transformation" << trans_stack.top().first; // for debug
      //      use
      obj.close();

      auto [trans, trans_inv] =
          scene->trans_table.get_tf_and_inv(trans_stack.top());

      auto mat_ptr = std::make_shared<phong_material>(
          mp.diffuse, mp.specular, mp.shininess, mp.emission, mp.ambient);
      Model model(trans, trans_inv, path, mat_ptr);
      objects.push_back(model.model_ptr);
    };
    getline(in, str);
    while (in) {
      if ((str.find_first_not_of(" \t\r\n") != std::string::npos) &&
          (str[0] != '#')) {
        // Ruled out comment and blank lines

        std::stringstream s(str);
        s >> cmd;
        int i;
        float values[13]; // Position and color for light, colors for others
        // Up to 10 params for cameras.
        bool validinput; // Validity of input

        // Process the light, add it to database.
        // Lighting Command
        if (cmd == "directional" || cmd == "point" || cmd == "quadLight") {
          if (numlights >= kMaxNumLights) { // No more Lights
            std::cerr << "Reached Maximum Number of Lights " << numlights
                      << " Will ignore further lights\n";
          } else {
            if (cmd == "directional" || cmd == "point") {
              validinput = readvals(s, 6, values); // Position/color for lts.
              if (validinput) {

                auto translate_mat =
                    Matrix4::Translate({values[0], values[1], values[2]});

                auto [trans, trans_inv] = scene->trans_table.get_tf_and_inv(
                    trans_stack.top() * translate_mat);
                lp.emission = Vector3f{values[3], values[4], values[5]};
                uint8_t type;
                if (cmd == "point")
                  type = 1;
                else if (cmd == "directional")
                  type = (1 << 1);

                auto mat_ptr = std::make_shared<DR::phong_material_for_light>(
                    lp.emission, lp.attenuation, type);
                // Point light is not currently supported by my tracer
                // we need to create a shape for it
                float radius = 0.001f;
                auto shape_ptr =
                    std::make_shared<Sphere>(trans, trans_inv, false, radius);
                std::shared_ptr<Primitive> obj_ptr =
                    std::make_shared<GeometricPrimitive>(shape_ptr, mat_ptr);
                scene->light_shapes_.push_back(obj_ptr);
                objects.push_back(obj_ptr);
                // YOUR CODE FOR HW 2 HERE.
                // Note that values[0...7] shows the read in values
                ++numlights;
              }
            } else if (cmd == "quadLight") {
              validinput = readvals(s, 12, values);
              if (validinput) {
                Point3f a(values[0], values[1], values[2]);
                vec3 ab{values[3], values[4], values[5]};
                vec3 ac{values[6], values[7], values[8]};
                Point3f b = a + ab;
                Point3f c = a + ac;
                //                Point3f d = a + (ab + ac);
                auto [trans, trans_inv] =
                    scene->trans_table.get_tf_and_inv(trans_stack.top());
                lp.emission = Vector3f{values[9], values[10], values[11]};
                uint8_t type = 1 << 2;
                auto mat_ptr = std::make_shared<DR::phong_material_for_light>(
                    lp.emission, lp.attenuation, type);
                // Point light is not currently supported by my tracer
                // we need to create a shape for it
                auto shape_ptr =
                    std::make_shared<Quad>(trans, trans_inv, a, b, c, false);
                std::shared_ptr<Primitive> obj_ptr =
                    std::make_shared<GeometricPrimitive>(shape_ptr, mat_ptr);
                scene->light_shapes_.push_back(obj_ptr);
                objects.push_back(obj_ptr);
                // YOUR CODE FOR HW 2 HERE.
                // Note that values[0...7] shows the read in values
                ++numlights;
              }
            }
          }
        }

        else if (cmd == "ambient") {
          validinput = readvals(s, 3, values); // colors
          if (validinput) {
            if (!indices.empty()) {
              write_obj_then_load();
            }
            // FIXME: support alpha channel
            for (i = 0; i < 3; i++) {
              mp.ambient[i] = values[i];
            }
          }
        } else if (cmd == "diffuse") {
          validinput = readvals(s, 3, values);
          if (validinput) {
            for (i = 0; i < 3; i++) {
              mp.diffuse[i] = values[i];
            }
          }
        } else if (cmd == "specular") {
          validinput = readvals(s, 3, values);
          if (validinput) {
            for (i = 0; i < 3; i++) {
              mp.specular[i] = values[i];
            }
          }
        } else if (cmd == "emission") {
          validinput = readvals(s, 3, values);
          if (validinput) {
            for (i = 0; i < 3; i++) {
              mp.emission[i] = values[i];
            }
          }
        } else if (cmd == "shininess") {
          validinput = readvals(s, 1, values);
          if (validinput) {
            mp.shininess = values[0];
          }
        } else if (cmd == "size") {
          validinput = readvals(s, 2, values);
          if (validinput) {
            width = (int)values[0];
            height = (int)values[1];
          }
        } else if (cmd == "camera") {
          validinput = readvals(s, 10, values); // 10 values eye cen up fov
          if (validinput) {
            Point3f origin(values[0], values[1], values[2]);
            vec3 lookat(values[3], values[4], values[5]);
            vec3 worldup(values[6], values[7], values[8]);
            float fov(values[9]);
            auto cam = std::make_shared<PinholeCamera>(
                origin, worldup, lookat, fov, height, width, scene);
            scene->add(cam);
            // YOUR CODE FOR HW 2 HERE
            // Use all of values[0...9]
            // You may need to use the upvector fn in Transform.cpp
            // to set up correctly.
            // Set eyeinit upinit center fovy in variables.h
          }
        }

        // I've left the code for loading objects in the skeleton, so
        // you can get a sense of how this works.
        // Also look at demo.txt to get a sense of why things are done this
        // way.
        else if (cmd == "sphere") {
          //                   || cmd == "cube" || cmd == "teapot") {
          if (numobjects >= kMaxNumObjects) { // No more objects
            std::cerr << "Reached Maximum Number of Objects " << kMaxNumObjects
                      << " Will ignore further objects\n";

          } else {
            if (cmd == "sphere") {
              validinput = readvals(s, 4, values);
              bool reverse = false;
              if (validinput) {
                float radius = values[3];
                auto translate_mat4 =
                    Matrix4::Translate({values[0], values[1], values[2]});

                auto [trans, trans_inv] = scene->trans_table.get_tf_and_inv(
                    trans_stack.top() * translate_mat4);

                auto mat_ptr = std::make_shared<phong_material>(
                    mp.diffuse, mp.specular, mp.shininess, mp.emission,
                    mp.ambient);
                auto shape_ptr =
                    std::make_shared<Sphere>(trans, trans_inv, reverse, radius);
                auto object_ptr =
                    std::make_shared<GeometricPrimitive>(shape_ptr, mat_ptr);
                objects.push_back(object_ptr);
              }
              //                  else if (cmd == "cube") {
              //                  obj->type = cube;
              //                } else if (cmd == "teapot") {
              //                  obj->type = teapot;
              //                }
            }
            ++numobjects;
          }
        }

        else if (cmd == "translate") {
          validinput = readvals(s, 3, values);
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file.
            // Also keep in mind what order your matrix is!
            auto translate =
                Matrix4::Translate({values[0], values[1], values[2]});
            auto &top = trans_stack.top();

            std::cout << "translate:" << std::endl;
            std::cout << translate << std::endl;
            std::cout << "after translate" << '\n'
                      << top * translate << std::endl;
            top = top * translate;
          }
        } else if (cmd == "scale") {
          validinput = readvals(s, 3, values);
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file.
            // Also keep in mind what order your matrix is!

            auto scale = Matrix4::Scale({values[0], values[1], values[2]});
            auto &top = trans_stack.top();

            std::cout << "scale:" << std::endl;
            std::cout << scale << std::endl;
            std::cout << "after scale" << '\n' << top * scale << std::endl;
            top = top * scale;
          }
        } else if (cmd == "rotate") {
          validinput = readvals(s, 4, values);
          if (validinput) {
            float rad = deg2rad(values[3]);
            auto rotation =
                Matrix4::Rotate(rad, {values[0], values[1], values[2]});
            auto &top = trans_stack.top();
            std::cout << "rotation:" << std::endl;
            std::cout << rotation << std::endl;
            std::cout << "after rotation" << '\n'
                      << top * rotation << std::endl;
            top = top * rotation;

            // YOUR CODE FOR HW 2 HERE.
            // values[0..2] are the axis, values[3] is the angle.
            // You may want to normalize the axis (or in Transform::rotate)
            // See how the stack is affected, as above.
            // Note that rotate returns a mat3.
            // Also keep in mind what order your matrix is!
          }
        }

        // I include the basic push/pop code for matrix stacks
        else if (cmd == "pushTransform") {
          trans_stack.push(trans_stack.top());
        } else if (cmd == "popTransform") {
          if (!indices.empty()) {
            write_obj_then_load();
          }
          if (trans_stack.size() <= 1) {
            std::cerr << "Stack has no elements.  Cannot Pop\n";
          } else {
            trans_stack.pop();
          }
        } else if (cmd == "tri") {
          indices.push_back(str);
        } else if (cmd == "vertex") {
          vertices.push_back(str);
        } else if (cmd == "spp") {
          validinput = readvals(s, 1, values);
          spp = static_cast<int>(values[0]);
          std::cout << "spp: " << spp << std::endl;
        } else if (cmd == "integrator") {
          std::string type;
          s >> type;
          make_render(scene, spp, type);
        }

        else {
          std::cerr << "Unknown Command: " << cmd << " Skipping \n";
        }
      }
      getline(in, str);
    }
    if (!indices.empty()) {
      write_obj_then_load();
    }
    auto bvh_tree = std::make_shared<BVHTree>(objects);
    scene->add(bvh_tree);
  } else {
    std::stringstream oss;
    oss << "Unable to Open Input Data File " << filename << "\n";
    throw std::runtime_error(oss.str());
  }
}
