//
//* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
//

#pragma once
#include "di_global.h"
#include <accelerator/bvh.h>
#include <cameras/pinhole_camera.h>
#include <cores/scene.h>
#include <deque>
#include <fstream>
#include <iostream>
#include <material/matte_material.h>
#include <material/phong_material.h>
#include <math/matrix.hpp>
#include <shapes/sphere.h>
#include <sstream>
#include <stack>
#include <string>
#include <texture/constant_texture.h>

// NAMESPACE_BEGIN(DR)
// NAMESPACE_BEGIN(IMPL)
namespace DR {
namespace IMPL {
inline constexpr int kMaxNumLights = 10;
inline constexpr int kMaxNumObjects = 20;

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.
bool readvals(std::stringstream &s, const int numvals, float *values) {
  for (int i = 0; i < numvals; i++) {
    s >> values[i];
    if (s.fail()) {
      std::cout << "Failed reading value " << i << " will skip\n";
      return false;
    }
  }
  return true;
}

inline void parse_scene_txt(std::string filename, Scene *scene, int *spp) {
  std::string str, cmd;
  std::ifstream in;
  *spp = 8;
  in.open(filename);
  int numlights = 0;
  int numobjects = 0;
  int width = 0, height = 0;
  if (in.is_open()) {

    // I need to implement a matrix stack to store transforms.
    // This is done using standard STL Templates
    // trans, trans_inv
    std::stack<std::pair<mat4, mat4>> trans_stack;
    trans_stack.push({mat4(1.0), mat4(1.0)}); // identity
    {
      auto trans = std::make_shared<Transform>(mat4(1.0));
      auto trans_inv = std::make_shared<Transform>(Transform::Inverse(*trans));
      Transform::TransformTable.insert({*trans, trans});
      Transform::TransformTable.insert({*trans_inv, trans_inv});
    }

    // keep state during parsing
    struct MaterialProperty {
      vec3 diffuse;
      vec3 specular;
      vec3 emission;
      vec3 ambient;
      float shininess;
    } mp{};

    std::vector<std::shared_ptr<Primitive>> objects;
    getline(in, str);
    while (in) {
      if ((str.find_first_not_of(" \t\r\n") != std::string::npos) &&
          (str[0] != '#')) {
        // Ruled out comment and blank lines

        std::stringstream s(str);
        s >> cmd;
        int i;
        float values[10]; // Position and color for light, colors for others
        // Up to 10 params for cameras.
        bool validinput; // Validity of input

        // Process the light, add it to database.
        // Lighting Command
        if (cmd == "light") {
          if (numlights >= kMaxNumLights) { // No more Lights
            std::cerr << "Reached Maximum Number of Lights " << numlights
                      << " Will ignore further lights\n";
          } else {
            validinput = readvals(s, 8, values); // Position/color for lts.
            if (validinput) {

              auto translate_mat =
                  Matrix4::Translate({values[0], values[1], values[2]});

              auto trans = std::make_shared<Transform>(translate_mat);
              auto trans_inv =
                  std::make_shared<Transform>(Transform::Inverse(*trans));
              // check if it is in Transform table, if not insert it
              if (!Transform::TransformTable.count(*trans)) {
                Transform::TransformTable.insert({*trans, trans});
                Transform::TransformTable.insert({*trans_inv, trans_inv});
              }
              // FIXME: potential self-assign
              trans = Transform::TransformTable.at(*trans);
              trans_inv = Transform::TransformTable.at(*trans_inv);
              auto emission = Vector3f{values[4], values[5], values[6]};
              auto texture_ptr =
                  std::make_shared<DR::ConstantTexture>(emission);
              std::shared_ptr<Material> mat_ptr =
                  std::make_shared<DR::MatteMaterial>(texture_ptr, emission);
              // Point light is not currently supported by my tracer
              // we need to create a shape for it
              float radius = 0.0001f;
              std::shared_ptr<Shape> shape_ptr =
                  std::make_shared<Sphere>(trans, trans_inv, false, radius);
              std::shared_ptr<Primitive> obj_ptr =
                  std::make_shared<GeometricPrimitive>(shape_ptr, mat_ptr);
              scene->light_shapes_.push_back(obj_ptr);
              // YOUR CODE FOR HW 2 HERE.
              // Note that values[0...7] shows the read in values
              ++numlights;
            }
          }
        }

        // Material Commands
        // Ambient, diffuse, specular, shininess properties for each object.
        // Filling this in is pretty straightforward, so I've left it in
        // the skeleton, also as a hint of how to do the more complex ones.
        // Note that no transforms/stacks are applied to the colors.

        else if (cmd == "ambient") {
          validinput = readvals(s, 4, values); // colors
          if (validinput) {
            // FIXME: support alpha channel
            for (i = 0; i < 3; i++) {
              mp.ambient[i] = values[i];
            }
          }
        } else if (cmd == "diffuse") {
          validinput = readvals(s, 4, values);
          if (validinput) {
            for (i = 0; i < 3; i++) {
              mp.diffuse[i] = values[i];
            }
          }
        } else if (cmd == "specular") {
          validinput = readvals(s, 4, values);
          if (validinput) {
            for (i = 0; i < 3; i++) {
              mp.specular[i] = values[i];
            }
          }
        } else if (cmd == "emission") {
          validinput = readvals(s, 4, values);
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
            auto cam = std::make_shared<PinholeCamera>(origin, worldup, lookat,
                                                       fov, height, width);
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
                auto model_mat =
                    Matrix4::Translate({values[0], values[1], values[2]});
                auto model_mat_ptr = std::make_shared<Transform>(
                    trans_stack.top().first * model_mat);
                //逆矩阵是右乘
                auto model_mat_inv_ptr = std::make_shared<Transform>(
                    model_mat * trans_stack.top().second);
                Transform::TransformTable.insert(
                    {*model_mat_ptr, model_mat_ptr});
                Transform::TransformTable.insert(
                    {*model_mat_inv_ptr, model_mat_inv_ptr});

                auto trans = Transform::TransformTable.at({*model_mat_ptr});
                auto trans_inv =
                    Transform::TransformTable.at({*model_mat_inv_ptr});

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
            auto translate_inv = Matrix4::Inverse(translate);
            auto &top = trans_stack.top();
            top.first = translate * top.first;
            top.second = top.second * translate_inv;
          }
        } else if (cmd == "scale") {
          validinput = readvals(s, 3, values);
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE.
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file.
            // Also keep in mind what order your matrix is!

            auto scale = Matrix4::Scale({values[0], values[1], values[2]});
            auto scale_inv = Matrix4::Inverse(scale);
            auto &top = trans_stack.top();
            top.first = scale * top.first;
            top.second = top.second * scale_inv;
          }
        } else if (cmd == "rotate") {
          validinput = readvals(s, 4, values);
          if (validinput) {
            float rad = deg2rad(values[3]);
            auto rotation =
                Matrix4::Rotate(rad, {values[0], values[1], values[2]});
            auto rotation_inv = Matrix4::Inverse(rotation);
            auto &top = trans_stack.top();
            top.first = rotation * top.first;
            top.second = top.second * rotation_inv;

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
          if (trans_stack.size() <= 1) {
            std::cerr << "Stack has no elements.  Cannot Pop\n";
          } else {
            trans_stack.pop();
          }
        }

        else {
          std::cerr << "Unknown Command: " << cmd << " Skipping \n";
        }
      }
      getline(in, str);
    }
    auto bvh_tree = std::make_shared<BVHTree>(objects);
    scene->add(bvh_tree);
  } else {
    std::stringstream oss;
    oss << "Unable to Open Input Data File " << filename << "\n";
    throw std::runtime_error(oss.str());
  }
}
} // namespace IMPL

} // namespace DR

// NAMESPACE_END(IMPL)
// NAMESPACE_END(DR)
