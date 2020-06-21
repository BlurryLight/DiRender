

// GLAD
#include "glad/glad.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// GLFW
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
static const GLuint SCR_WIDTH = 800;
static const GLuint SCR_HEIGHT = 600;
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
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LEARNOPENGL", NULL, NULL);

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
  // v shader
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  std::fstream fs;
  std::string vshader;
  fs.open("simple.vert");
  if (fs.good()) {
    std::stringstream ss;
    ss << fs.rdbuf();
    vshader = ss.str();
  } else {
    std::cerr << "FAILED TO INIT VSHADER" << std::endl;
    return -1;
  }
  fs.close();

  std::cout << vshader << std::endl;
  const char *vshader_source = vshader.c_str();
  glShaderSource(vertexShader, 1, &vshader_source, NULL);
  glCompileShader(vertexShader);

  int success = 0;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "FAILED TO COMPILE VSHADER" << infoLog << std::endl;
    return -1;
  }

  GLuint fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fshader;
  fs.open("simple.frag");
  if (fs.good()) {
    std::stringstream ss;
    ss << fs.rdbuf();
    fshader = ss.str();
  } else {
    std::cerr << "FAILED TO INIT fSHADER" << fs.is_open() << std::endl;
    return -1;
  }

  fs.close();

  std::cout << fshader << std::endl;
  std::cout << fshader << std::endl;
  const char *fshader_source = fshader.c_str();
  glShaderSource(fragmentShader, 1, &fshader_source, NULL);
  glCompileShader(fragmentShader);

  GLint succ;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &succ);
  if (!succ) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));
    std::cerr << "FAILED TO COMPILE FSHADER" << infoLog << std::endl;
    std::cout << infoLog << std::endl;
    return -1;
  }

  int linkedShader;
  linkedShader = glCreateProgram();
  glAttachShader(linkedShader, vertexShader);
  glAttachShader(linkedShader, fragmentShader);
  glLinkProgram(linkedShader);

  success = 0;
  glGetProgramiv(linkedShader, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(linkedShader, 512, NULL, infoLog);
    std::cerr << "FAILED TO LINK SHADERS" << infoLog << std::endl;
    return -1;
  }

  glUseProgram(linkedShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // data
  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

  unsigned int VBO, VAO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
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
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(linkedShader);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
