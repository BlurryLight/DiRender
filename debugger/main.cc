

// GLAD
#include "glsupport.h"
#include "third_party/glad/glad.h"
#include "third_party/imgui/imgui.h"
#include "third_party/imgui/imgui_impl_glfw.h"
#include "third_party/imgui/imgui_impl_opengl3.h"
#include <utils/di_global.h>
#include <utils/resource_path_searcher.h>

#include <fstream>
#include <iostream>
#include <sstream>
using namespace DR;
static const GLuint SCR_WIDTH = 800;
static const GLuint SCR_HEIGHT = 600;
DR::Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *, int width, int height);
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

  // glad loads
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "FAILED TO INIT GLAD" << std::endl;
    return -1;
  }

  // config global OpenGL state
  glEnable(GL_DEPTH_TEST);

  const char *glsl_verson = "#version 330";
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_verson);
  bool show_demo_window = true;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  // shader
  ResourcePathSearcher resourcesPath;
  resourcesPath.add_path(
      (ResourcePathSearcher::root_path / "models").u8string());
  resourcesPath.add_path(
      (ResourcePathSearcher::root_path / "debugger" / "shaders").u8string());
  DR::Shader shader(resourcesPath.find_path("simple.vert"),
                    resourcesPath.find_path("simple.frag"));
  DR::Model model(resourcesPath.find_path("cube.obj"));
  // data
  // set
  shader.use();
  shader.setMat4("model", glm::mat4(1.0f));

  while (!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput(window);

    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
                                     // and append into it.

      ImGui::ColorEdit3(
          "clear color",
          (float *)&clear_color); // Edit 3 floats representing a color

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }
    ImGui::Render();

    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    shader.setMat4("view", cam.GetViewMatrix());
    auto projection = glm::perspective(
        glm::radians(cam.Zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projection", projection);
    model.draw(shader);
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

void processInput(GLFWwindow *window) {
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
