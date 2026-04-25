#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>

#define WIDTH 800
#define HEIGHT 600

const float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                          0.0f,  0.0f,  0.5f, 0.0f};

// Callback when windows get resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
std::string read_file(const std::string &path);
void compile_shader_from_file(GLuint shader, const std::string &path);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE); // Modern version
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot Visualization", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Pass to glad the function to load the address of the opengl function
  // pointers which are OS-specific
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Tells openGL the size of the rendering window
  // First two are coordinates of bottom-left
  // Third and fourth are the width and height of the window
  // NOTE: used by openGL to transform points from (-1, 1) to (0, WIDTH)
  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Vertex buffer object stores large number of vertices
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // binds the VBO to the GL_ARRAY_BUFFER target, each call on that target will
  // use the VBO defined before (note how glBufferData doesn't mention VBO)
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  compile_shader_from_file(vertex_shader, "src/shaders/vertex.glsl");

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(1.0f, 0.f, 0.f, 1.0f); // state-setting function for glClear
    glClear(GL_COLOR_BUFFER_BIT); // clears the color buffer (there are others)

    glfwPollEvents();
    // DOUBLE BUFFERING
    // the window renders the FRONT buffer
    // all the subsequent commands draw to the BACK buffer
    glfwSwapBuffers(window); // Switches color buffer (2D buffer)
  }

  glfwTerminate();
  return 0;
}

void compile_shader_from_file(GLuint shader, const std::string &path) {
  std::string src = read_file(path);
  const char *ptr = src.c_str();
  glShaderSource(shader, 1, &ptr, NULL);
  glCompileShader(shader);
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
}

std::string read_file(const std::string &path) {
  std::ifstream f(path);
  if (!f)
    throw std::runtime_error("cannot open " + path);
  std::ostringstream ss;
  ss << f.rdbuf();
  return ss.str();
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
