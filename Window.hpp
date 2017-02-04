#pragma once

#include <glm/glm.hpp>

#include "Cube.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "incgraphics.h"

#include <glm/glm.hpp>

class Window {
  GLFWwindow *win_ = NULL;
  size_t width, height;
  Cube <3> cubebuffer;
  ShaderProgram shader_program;
  Camera camera;

  void start();
  void init_glfw();
  void init_glew();
  void init_controls();
public:
  Window(size_t width, size_t height);
  void gl_version();
  void idle();
  void display();
  void keyboard();
  void clear();
  ~Window();
};
