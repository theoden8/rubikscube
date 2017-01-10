#pragma once

#include <cassert>
#include <string>
#include <vector>

#include "glew_config.h"
#include "glfw3_config.h"

class ShaderProgram {
  GLuint
    program = 0;
  GLuint
    vert = 0,
    frag = 0;
  const std::string
    vert_fname, frag_fname;

  static char *load_text_file(const char *filename);
  void compile_vert_shader();
  void compile_frag_shader();
  void compile_program();
  void _print_info_log();
  void print_all();

public:
  ShaderProgram(const std::string &vert, const std::string &frag);

  void use();
  void compile();
  operator GLuint();
  void bind(const std::vector <std::string> &locations);
  void clear();
  ~ShaderProgram();
  bool is_valid();
};
