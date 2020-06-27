

// GLAD
#include "glsupport.h"
#include "parse_ray_log.hpp"
#include "third_party/glad/glad.h"
#include "third_party/imgui/imgui.h"
#include "third_party/imgui/imgui_impl_glfw.h"
#include "third_party/imgui/imgui_impl_opengl3.h"
#include <utils/di_global.h>
#include <utils/parse_scene.hpp>
#include <utils/resource_path_searcher.h>

#include <fstream>
#include <iostream>
#include <sstream>
using namespace DR_D;
static const GLuint SCR_WIDTH = 800;
static const GLuint SCR_HEIGHT = 800;

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
void processInput(GLFWwindow *window, Camera &cam);
void framebuffer_size_callback(GLFWwindow *, int width, int height);
void mouse_callback(GLFWwindow *, double width, double height);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);
static bool firstMouse = true;
static double lastX, lastY;
static Camera cam;
static bool AllowMouseMove = true;
int main() {
  if (!glfwInit()) {
    std::cerr << "FATAL INIT FAILED" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "DiDebugger", NULL, NULL);

  if (window == NULL) {
    std::cerr << "GLFW INIT FAILED" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetKeyCallback(window, key_callback);

  // glad loads
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "FAILED TO INIT GLAD" << std::endl;
    return -1;
  }

  // config global OpenGL state
  glEnable(GL_DEPTH_TEST);
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SCR_WIDTH, SCR_HEIGHT, 0,
               GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         texture, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  const char *glsl_verson = "#version 330";
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_verson);

  // IMGUI
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  float Blinn_Shininess = 8.0f;

  // shader
  DR::ResourcePathSearcher resourcesPath;
  resourcesPath.add_path(
      (DR::ResourcePathSearcher::root_path / "models").u8string());
  resourcesPath.add_path(
      (DR::ResourcePathSearcher::root_path / "debugger" / "shaders")
          .u8string());
  resourcesPath.add_path(
      (DR::ResourcePathSearcher::root_path / "models" / "cornellbox")
          .u8string());
  DR_D::Shader SceneShader(resourcesPath.find_path("simple.vert"),
                           resourcesPath.find_path("BlinnPhong.frag"));

  DR_D::Shader LightsShader(resourcesPath.find_path("simple.vert"),
                            resourcesPath.find_path("lights.frag"));
  DR_D::Shader CamShader(resourcesPath.find_path("simple.vert"),
                         resourcesPath.find_path("simple.frag"));

  DR_D::Shader RayShader(resourcesPath.find_path("line.vert"),
                         resourcesPath.find_path("simple.frag"),
                         resourcesPath.find_path("line.geom"));
  auto ray_data = parse_ray_log("Dirender_ray.txt");
  std::vector<float> ray_data_pod;
  ray_data_pod.reserve(9 * ray_data.size());
  for (const auto &line : ray_data) {
    for (int j = 0; j < 3; j++) {
      ray_data_pod.push_back(line.at(j).x);
      ray_data_pod.push_back(line.at(j).y);
      ray_data_pod.push_back(line.at(j).z);
    }
  }
  int rays_begin_index = 0;
  int rays_to_draw = 10;

  unsigned int ray_vbo, ray_vao;
  glGenBuffers(1, &ray_vbo);
  glGenVertexArrays(1, &ray_vao);
  glBindVertexArray(ray_vao);
  glBindBuffer(GL_ARRAY_BUFFER, ray_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ray_data_pod.size(),
               ray_data_pod.data(), GL_STATIC_DRAW);
  // ray.origin
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
  // ray.isect.coords
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  // ray.isect.normal
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glBindVertexArray(0);

  auto toml_scene_data =
      toml::parse(resourcesPath.find_path("cornel_box.toml"));
  auto cam_data = DR::impl::parse_camera_data_impl(toml_scene_data);
  auto objects = DR::impl::parse_objects_wrapper(toml_scene_data);
  std::vector<std::shared_ptr<DR_D::Object>> models;
  std::vector<std::shared_ptr<DR_D::Object>> lights;
  for (const auto &obj : objects) {
    obj->load(resourcesPath.find_path(obj->path));
    if (obj->has_emission) {
      lights.push_back(obj);
    } else {
      models.push_back(obj);
    }
  }
  // Now we only cares 0th camera
  auto [origin, up, lookat, fov, height, width, type] = cam_data.at(0);

  auto tmp = DR_D::Camera(glm::vec3(origin.x, origin.y, origin.z),
                          glm::vec3(up.x, up.y, up.z),
                          glm::vec3(lookat.x, lookat.y, lookat.z), fov);
  cam = tmp;

  // shader uniform set
  //  shader.use();

  // main loop
  while (!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput(window, cam);

    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
      ImGui::Begin("Background Color"); // Create a window called "Hello,
                                        // world!" and append into it.
      ImGui::ColorEdit3(
          "clear color",
          (float *)&clear_color); // Edit 3 floats representing a color
      if (ImGui::CollapsingHeader("Lights")) {
        for (uint i = 0; i < lights.size(); i++) {
          {
            std::string lightname = "lights" + std::to_string(i);
            if (ImGui::TreeNode(lightname.c_str())) {
              ImGui::SliderFloat("R", &lights[i]->emission.x, 0.0f, 100.0f);
              ImGui::SliderFloat("G", &lights[i]->emission.y, 0.0f, 100.0f);
              ImGui::SliderFloat("B", &lights[i]->emission.z, 0.0f, 100.0f);
              ImGui::TreePop();
            }
          }
        }
      }

      if (ImGui::CollapsingHeader("Camera")) {
        ImGui::SliderFloat("Blinn Shininess", &Blinn_Shininess, 0.0f, 128.0f);

        ImGui::SliderFloat("Camera Movementspeed", &cam.MovementSpeed, 0.0f,
                           100.0f);
        ImGui::SliderFloat("Camera MouseSensitivity", &cam.MouseSensitivity,
                           0.0f, 1.0f);
        ImGui::Text("Cam: (%.3f,%.3f,%.3f)", cam.Position.x, cam.Position.y,
                    cam.Position.z);
      }

      ImGui::SliderInt("Rays begin index", &rays_begin_index, 0,
                       ray_data.size() - rays_to_draw);
      ImGui::SliderInt("Rays num", &rays_to_draw, 0,
                       ray_data.size() - rays_begin_index - 1);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }
    ImGui::Render();

    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projection = glm::perspective(
        glm::radians(cam.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 2000.0f);
    auto view = cam.GetViewMatrix();
    SceneShader.use();
    for (const auto &obj : models) {
      SceneShader.setMat4("model", obj->model_mat4);
      SceneShader.setMat4("view", view);
      SceneShader.setMat4("projection", projection);
      SceneShader.setVec3("ambient_color", obj->albedo);
      SceneShader.setVec3("viewPos", cam.Position);
      for (uint i = 0; i < lights.size(); i++) {
        SceneShader.setVec3("lights[" + std::to_string(i) + "].Position",
                            lights[i]->centroid);
        SceneShader.setVec3("lights[" + std::to_string(i) + "].Color",
                            lights[i]->emission);
        SceneShader.setVec3("lights[" + std::to_string(i) + "].Normal",
                            lights[i]->normal);
        SceneShader.setFloat("lights[" + std::to_string(i) + "].Linear", 0.007);
        SceneShader.setFloat("lights[" + std::to_string(i) + "].Quadratic",
                             0.0002);
      }
      SceneShader.setFloat("shininess", Blinn_Shininess);
      SceneShader.setInt("lightNum", lights.size());
      obj->model->draw(SceneShader);
    }
    // Draw lights
    LightsShader.use();
    for (const auto &obj : lights) {
      LightsShader.setMat4("model", obj->model_mat4);
      LightsShader.setMat4("view", view);
      LightsShader.setMat4("projection", projection);
      LightsShader.setVec3("lightColor", obj->emission);
      obj->model->draw(LightsShader);
    }

    // Draw camera
    CamShader.use();
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(origin.x, origin.y, origin.z));
    CamShader.setMat4("model", model);
    CamShader.setMat4("view", view);
    CamShader.setMat4("projection", projection);
    renderCube();

    // render rays
    RayShader.use();
    RayShader.setMat4("model", glm::mat4(1.0));
    RayShader.setMat4("view", view);
    RayShader.setMat4("projection", projection);
    glBindVertexArray(ray_vao);
    glDrawArrays(GL_POINTS, rays_begin_index, rays_to_draw);
    //
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Camera &cam) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cam.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cam.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cam.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cam.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (AllowMouseMove) {
    if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset =
        lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    cam.ProcessMouseMovement(xoffset, yoffset);
  } else {
    firstMouse = true;
  }
};

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {

  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    AllowMouseMove = !AllowMouseMove;
    std::cout << (AllowMouseMove ? "AllowMouseMove" : "DisallowMouseMove")
              << std::endl;
  }
}
