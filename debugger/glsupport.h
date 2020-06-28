#pragma once

#include "../third_party/OBJ_Loader.h"
#include "third_party/glad/glad.h"
#include "third_party/glm/glm/glm.hpp"
#include "third_party/glm/glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utils/parse_scene.hpp>
#include <vector>
namespace DR_D // DR_Debugger
{

using uint = unsigned int;

// Defines several possible options for camera movement. Used as abstraction
// to stay away from window-system specific input methods
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
constexpr static float YAW = -90.0f;
constexpr static float PITCH = 0.0f;
constexpr static float SPEED = 2.5f;
constexpr static float SENSITIVITY = 0.1f;
constexpr static float ZOOM = 45.0f;

// classes
class Camera;
class Mesh;
class Shader;
struct Vertex;
struct Texture;
struct Model {
private:
  enum class Material { kDiffuse, kMirror, kDielectric };
  std::unique_ptr<objl::Loader> loader_;
  struct Mesh {
    std::vector<objl::Vertex> vertices_;
    std::vector<GLuint> indices_;
    unsigned int vao, vbo;
    size_t BufferDataSize() const {
      return sizeof(objl::Vertex) * vertices_.size();
    }
    size_t size() const { return indices_.size(); }
  };

public:
  Material mat_ = Material::kDiffuse;
  glm::vec3 kd{1.0f};
  std::vector<Mesh> meshes_;
  void draw(const Shader &shader) const;
  Model(const std::string &path);
};

// funcs
unsigned int TextureFromFile(const char *path,
                             const std::string &directory = "",
                             bool gamma = false, bool flip = false);
void renderQuad();
void renderCube();
void renderSphere();
inline float lerp(float a, float b, float t) { return a * (1 - t) + b * t; }

inline float get_random_float(float min=0.0f,float max=1.0f)
{
  static std::mt19937 generator;
  std::uniform_real_distribution<float> dis(min,max);
  return dis(generator);
}

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
  // Camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 LookAt;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // Euler Angles
  float Yaw;
  float Pitch;
  // Camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  // Constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
         glm::vec3 lookat = glm::vec3(0.0f, 0.0f, -1.0f), float fov = ZOOM,
         float yaw = YAW, float pitch = PITCH);

  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 GetViewMatrix();

  // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
  void ProcessKeyboard(Camera_Movement direction, float deltaTime);

  // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true);

  // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset);

private:
  // Calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors();
};

class Shader
{
public:
  unsigned int ID;
  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  Shader(const char *vertexPath, const char *fragmentPath,
         const char *geometryPath = nullptr);
  Shader(const std::string &vertexPath, const std::string &fragmentPath)
      : Shader(vertexPath.c_str(), fragmentPath.c_str()) {}
  Shader(const std::string &vertexPath, const std::string &fragmentPath,
         const std::string &geometryPath)
      : Shader(vertexPath.c_str(), fragmentPath.c_str(), geometryPath.c_str()) {
  }
  // activate the shader
  // ------------------------------------------------------------------------
  void use() const;
  // utility uniform functions
  // ------------------------------------------------------------------------
  void setBool(const std::string &name, bool value) const;
  // ------------------------------------------------------------------------
  void setInt(const std::string &name, int value) const;
  // ------------------------------------------------------------------------
  void setFloat(const std::string &name, float value) const;
  // ------------------------------------------------------------------------
  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec2(const std::string &name, float x, float y) const;
  // ------------------------------------------------------------------------
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  // ------------------------------------------------------------------------
  void setVec4(const std::string &name, const glm::vec4 &value) const;
  void setVec4(const std::string &name, float x, float y, float z, float w);
  // ------------------------------------------------------------------------
  void setMat2(const std::string &name, const glm::mat2 &mat) const;
  // ------------------------------------------------------------------------
  void setMat3(const std::string &name, const glm::mat3 &mat) const;
  // ------------------------------------------------------------------------
  void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
  // utility function for checking shader compilation/linking errors.
  // ------------------------------------------------------------------------
  void checkCompileErrors(GLuint shader, std::string type);
};

struct Vertex {
  // position
  glm::vec3 Position;
  // normal
  glm::vec3 Normal;
  // texCoords
  glm::vec2 TexCoords;
  // tangent
  glm::vec3 Tangent;
  // bitangent
  glm::vec3 Bitangent;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

struct Object // DR_Debugger_Object
{
  glm::vec3 centroid;
  // May cause error. Be careful
  // For example: Sampling a sphere to estimate the normal will get a near zero
  // normal vector and the estimated value can never represent the true normal
  // value for the sphere surface. It only works for plane.
  glm::vec3 normal;
  std::string name;
  std::string mat_type;
  std::string path;
  glm::mat4 model_mat4;
  glm::vec3 albedo;
  glm::vec3 emission{0.0f};
  float ior = 1.0f;
  bool has_emission = false;
  std::unique_ptr<Model> model;
  void load(std::string model_path) {
    model = std::make_unique<Model>(model_path);
    this->centroid = glm::vec3(0);
    this->normal = glm::vec3(0);

    // sample 20 points to estimate the model centroid and normal
    int mesh_num = model->meshes_.size();
    for (int i = 0; i < 20; i++) {
      int mesh_index = get_random_float(0.0, 0.99) * mesh_num;
      int mesh_vertex_size = model->meshes_[mesh_index].size();
      auto vertex =
          model->meshes_[mesh_index]
              .vertices_[mesh_vertex_size * get_random_float(0.0, 0.99)]
              .Position;
      auto normal =
          model->meshes_[mesh_index]
              .vertices_[mesh_vertex_size * get_random_float(0.0, 0.99)]
              .Normal;
      this->centroid += glm::vec3(vertex.X, vertex.Y, vertex.Z) / 20.0f;
      this->normal += glm::vec3(normal.X, normal.Y, normal.Z) / 20.0f;
    }
    this->normal = normalize(normal);
  }
};

} // namespace DR_D

namespace DR::impl {

template <typename T>
// This is a wrapper to be called by the debugger
// return value:
std::vector<std::shared_ptr<DR_D::Object>>
parse_objects_wrapper(const T &data) {
  const auto &objects_toml = toml::find(data, "objects");
  std::vector<std::shared_ptr<DR_D::Object>> objects;
  try {
    const auto &names_vec =
        toml::find<std::vector<toml::table>>(objects_toml, "name");
    const auto &transforms_vec =
        toml::find<std::vector<toml::table>>(objects_toml, "transform");
    const auto &materials_vec =
        toml::find<std::vector<toml::table>>(objects_toml, "material");
    const auto &shapes_vec =
        toml::find<std::vector<toml::table>>(objects_toml, "shape");

    // parse objects
    for (uint i = 0; i < names_vec.size(); i++) {
      auto obj_ptr = std::make_shared<DR_D::Object>();
      // obj_name
      obj_ptr->name = names_vec[i].at("name").as_string();
      auto mat_toml =
          toml::get<std::vector<float>>(transforms_vec[i].at("matrix4"));

      // obj_mat
      for (int j = 0; j < 16; j++) {
        // column major
        obj_ptr->model_mat4[j % 4][j / 4] = mat_toml.at(j);
      }

      auto material_toml = materials_vec[i];
      std::string mat_type = toml::get<std::string>(material_toml.at("type"));
      static std::array<std::string, 3> supported_materials{"matte", "glass",
                                                            "dielectric"};
      if (std::find(supported_materials.begin(), supported_materials.end(),
                    mat_type) != supported_materials.end()) {
        std::string texture_type = material_toml.at("texture").as_string();
        vec3 obj_albedo;
        if (texture_type == "constant") {
          auto albedo =
              toml::get<std::vector<float>>(material_toml.at("albedo"));
          obj_ptr->albedo = glm::vec3{albedo[0], albedo[1], albedo[2]};
        } else {
          throw std::runtime_error("Debugger: Unsupported texture type:" +
                                   texture_type);
          std::exit(EXIT_FAILURE);
        }
        vec3 obj_emission{0};
        if (material_toml.count("emission")) {
          auto tmp =
              toml::get<std::vector<float>>(material_toml.at("emission"));
          obj_ptr->emission = {tmp[0], tmp[1], tmp[2]};
          obj_ptr->has_emission = true;
        }

        if (material_toml.count("ior")) {
          obj_ptr->ior = toml::get<float>(material_toml.at("ior"));
        }
        obj_ptr->mat_type = mat_type;
      } else {
        throw std::runtime_error(
            "Debugger: Unsupported material type: " +
            toml::get<std::string>(material_toml.at("type")));
        std::exit(EXIT_FAILURE);
      }
      auto shape_toml = shapes_vec[i].at("type").as_string();
      // shape parse
      std::string obj_path;
      if (shape_toml == "obj") {
        obj_ptr->path = shapes_vec[i].at("path").as_string();
      } else {
        throw std::runtime_error("Debugger currently only supports obj files");
        continue;
      }
      objects.push_back(obj_ptr);
    }
  } catch (const std::exception &e) {
    std::cerr << "Debugger: Exception captured: " << '\n'
              << e.what() << std::endl;
  }
  return objects;
}

} // namespace DR::impl
