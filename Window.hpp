#pragma once

#include "Cube.hpp"
#include "Shader.hpp"
#include "incgraphics.h"

class Window {
  GLFWwindow *win_ = NULL;
  size_t width, height;
  Cube <3> cubebuffer;
  ShaderProgram shader_program;

  void start();
  void init_glfw();
  void init_glew();
  void init_controls();
public:
  Window(size_t width, size_t height);
  void gl_version();
  void idle();
  void display();
  ~Window();
};
