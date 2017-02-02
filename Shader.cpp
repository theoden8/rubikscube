#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <sys/stat.h>

#include "Shader.hpp"
#include "Log.hpp"

size_t file_length(const char *filename) {
  struct stat st;
  stat(filename, &st);
  return st.st_size;
}

char *ShaderProgram::load_text_file(const char *filename) {
  char *text = (char *)malloc(file_length(filename));
  assert(text != NULL);

  FILE *file = fopen(filename, "r");
  assert(file != NULL);

  static char c;
  int i = 0;
  while((c = fgetc(file)) != EOF) {
    text[i] = c;
    ++i;
  }
  text[i] = '\0';

  text = (char *)realloc(text, i + 1);
  assert(text != NULL);

  assert(!fclose(file));
  return text;
}

void ShaderProgram::compile_shader(GLuint &shader, GLenum type, const char *filename) {
  shader = glCreateShader(type); GLERROR
  char *source_code = load_text_file(filename);
  glShaderSource(shader, 1, &source_code, NULL); GLERROR
  glCompileShader(shader); GLERROR
  free(source_code);
}

void ShaderProgram::compile_program() {
  compile_shader(vert, GL_VERTEX_SHADER, vert_fname.c_str());
  compile_shader(frag, GL_FRAGMENT_SHADER, frag_fname.c_str());

  program = glCreateProgram(); GLERROR
  glAttachShader(program, frag); GLERROR
  glAttachShader(program, vert); GLERROR
  glLinkProgram(program); GLERROR

  glDeleteShader(vert); GLERROR
  glDeleteShader(frag); GLERROR
}


ShaderProgram::ShaderProgram(const std::string &vert, const std::string &frag):
  vert_fname(vert), frag_fname(frag)
{}

void ShaderProgram::use() {
  glUseProgram(*this); GLERROR
}

void ShaderProgram::compile() {
  compile_program();
}

ShaderProgram::operator GLuint() {
  return program;
}

void ShaderProgram::bind(const std::vector <std::string> &locations) {
  for(size_t i = 0; i < locations.size(); ++i) {
    glBindAttribLocation(program, i, locations[i].c_str()); GLERROR
  }
}

void ShaderProgram::clear() {
  glDeleteProgram(program); GLERROR
}

ShaderProgram::~ShaderProgram()
{}
