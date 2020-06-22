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
#include <vector>
namespace DR {
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

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
  std::vector<objl::Vertex> vertices;
  std::unique_ptr<objl::Loader> loader_;
  Model(const std::string &path);
  size_t BufferDataSize() const {
    return sizeof(objl::Vertex) * vertices.size();
  }
  size_t size() const { return vertices.size(); }
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
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH);
  // Constructor with scalar values
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
         float yaw, float pitch);

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
  // activate the shader
  // ------------------------------------------------------------------------
  void use();
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

} // namespace DR
