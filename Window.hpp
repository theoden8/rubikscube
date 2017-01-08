#pragma once

#include "Quad.hpp"
#include "Shader.hpp"
#include "incgraphics.h"

class Window {
  GLFWwindow *win_ = NULL;
  size_t width, height;
  Quad quadbuf;
  ShaderProgram shader_program;

  void start();
  void init_glfw();
  void init_glew();
  void init_controls();
public:
  Window(size_t width, size_t height);
  void gl_version();
  void display();
  void idle();
  ~Window();
};
