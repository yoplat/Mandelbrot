#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#define WIDTH 800
#define HEIGHT 800
#define R_START -2.0
#define R_END 1
#define I_START -1.5
#define I_END 1.5
#define MAX_ITER 100

// clang-format on
const float vertices[] = {
    // clang-format off
  -1.0f, -5.0f, 0.0f,
  -1.0, 1.0f, 0.0f,
  5.0f, 1.0f, 0.0f
    // clang-format on
};

struct AppState {
  bool changed;
};

// Callback when windows get resized
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, double zoom[2], double offset[2],
                  size_t &max_iter);
std::string read_file(const std::string &path);
void compile_shader_from_file(GLuint shader, const std::string &path);
void shaders_to_program(uint shader_program, uint vertex_shader,
                        uint fragment_shader);
GLFWwindow *create_window(size_t width, size_t height, const char *title);

int main() {
  GLFWwindow *window = create_window(WIDTH, HEIGHT, "Mandelbrot Visualization");

  glfwMakeContextCurrent(window);
  // sets callback when window is resized
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Pointer inside window to be accessed everywhere a window is passed
  AppState state{true};
  glfwSetWindowUserPointer(window, &state);

  // load all opengl function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  uint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  compile_shader_from_file(vertex_shader, "src/shaders/vertex.glsl");

  uint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  compile_shader_from_file(fragment_shader, "src/shaders/fragment.glsl");

  uint shader_program = glCreateProgram();
  shaders_to_program(shader_program, vertex_shader, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  // Creates state machine vertex attribute object for vertex buffers and attrib
  uint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // NOTE: remember to activate program before setting uniforms
  glUseProgram(shader_program);
  int minPointLocation = glGetUniformLocation(shader_program, "u_min");
  int maxPointLocation = glGetUniformLocation(shader_program, "u_max");
  int maxIterLocation = glGetUniformLocation(shader_program, "max_iter");
  int resLocation = glGetUniformLocation(shader_program, "resolution");
  int zoomLocation = glGetUniformLocation(shader_program, "zoom");
  int offsetLocation = glGetUniformLocation(shader_program, "offset");
  glUniform2d(minPointLocation, R_START, I_START);
  glUniform2d(maxPointLocation, R_END, I_END);

  double zoom[2] = {1.0f, 1.0f};
  double offset[2] = {0.0f, 0.0f};
  size_t max_iter = MAX_ITER;

  while (!glfwWindowShouldClose(window)) {
    if (state.changed) {
      glClearColor(1.0f, 0.f, 0.f, 1.0f); // state-setting function for glClear
      glClear(
          GL_COLOR_BUFFER_BIT); // clears the color buffer (there are others)

      glUseProgram(shader_program);
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      glUniform2d(resLocation, width, height);
      glUniform2d(zoomLocation, zoom[0], zoom[1]);
      glUniform2d(offsetLocation, offset[0], offset[1]);
      glUniform1i(maxIterLocation, max_iter);

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);

      glfwSwapBuffers(window);
      state.changed = false;
    }
    processInput(window, zoom, offset, max_iter);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader_program);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window, double *zoom, double *offset,
                  size_t &max_iter) {
  auto *state = static_cast<AppState *>(glfwGetWindowUserPointer(window));
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
    zoom[0] *= 0.95;
    zoom[1] *= 0.95;
    state->changed = true;
  }
  if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
    zoom[0] *= 1.05;
    zoom[1] *= 1.05;
    state->changed = true;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    offset[0] -= 0.03 * zoom[0];
    state->changed = true;
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    offset[0] += 0.03 * zoom[0];
    state->changed = true;
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    offset[1] += 0.03 * zoom[1];
    state->changed = true;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    offset[1] -= 0.03 * zoom[1];
    state->changed = true;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    max_iter += 1;
    state->changed = true;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    max_iter -= 1;
    state->changed = true;
  }
}

GLFWwindow *create_window(size_t width, size_t height, const char *title) {

  // initialize glfw and configure opengl version
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE); // Modern version
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (window == NULL) {
    glfwTerminate();
    std::runtime_error("Failed to create GLFW window");
  }
  return window;
}

void compile_shader_from_file(GLuint shader, const std::string &path) {
  std::string src = read_file(path);
  const char *ptr = src.c_str();
  // second param is how many shaders
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

void shaders_to_program(uint shader_program, uint vertex_shader,
                        uint fragment_shader) {
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  int success;
  char infoLog[512];
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM_FAILED\n" << infoLog << std::endl;
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // Tells openGL the size of the rendering window
  // First two are coordinates of bottom-left
  // Third and fourth are the width and height of the window
  // NOTE: used by openGL to transform points from (-1, 1) to (0, WIDTH)
  auto *state = static_cast<AppState *>(glfwGetWindowUserPointer(window));
  state->changed = true;
  glViewport(0, 0, width, height);
}
