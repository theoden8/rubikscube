#include <cstdio>

#include "Shader.hpp"
#include "Log.hpp"

void ShaderProgram::_print_info_log() {
  int max_length = 2048;
  int actual_length = 0;
  char program_log[2048];
  glGetProgramInfoLog(program, max_length, &actual_length, program_log); GLERROR
  printf("program info log for GL index %u:\n%s", program, program_log);
}

void ShaderProgram::print_all() {
  printf("--------------------\nshader program %i info:\n", program);
  int params = -1;
  glGetProgramiv(program, GL_LINK_STATUS, &params);
  printf("GL_LINK_STATUS = %i\n", params);

  glGetProgramiv(program, GL_ATTACHED_SHADERS, &params);
  printf("GL_ATTACHED_SHADERS = %i\n", params);

  glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &params);
  printf("GL_ACTIVE_ATTRIBUTES = %i\n", params);
  for (int i = 0; i < params; i++) {
    char name[64];
    int max_length = 64;
    int actual_length = 0;
    int size = 0;
    GLenum type;
    glGetActiveAttrib (
      program,
      i,
      max_length,
      &actual_length,
      &size,
      &type,
      name
      );
    if (size > 1) {
      for(int j = 0; j < size; j++) {
        char long_name[64];
        sprintf(long_name, "%s[%i]", name, j);
        int location = glGetAttribLocation(program, long_name);
        printf("  %i) type:%s name:%s location:%i\n",
               i, GL_type_to_string(type), long_name, location);
      }
    } else {
      int location = glGetAttribLocation(program, name);
      printf("  %i) type:%s name:%s location:%i\n",
             i, GL_type_to_string(type), name, location);
    }
  }

  glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &params);
  printf("GL_ACTIVE_UNIFORMS = %i\n", params);
  for(int i = 0; i < params; i++) {
    char name[64];
    int max_length = 64;
    int actual_length = 0;
    int size = 0;
    GLenum type;
    glGetActiveUniform(
    program,
    i,
    max_length,
    &actual_length,
    &size,
    &type,
    name
    );
    if(size > 1) {
    for(int j = 0; j < size; j++) {
      char long_name[64];
      sprintf(long_name, "%s[%i]", name, j);
      int location = glGetUniformLocation(program, long_name);
      printf("  %i) type:%s name:%s location:%i\n",
         i, GL_type_to_string(type), long_name, location);
    }
    } else {
    int location = glGetUniformLocation(program, name);
    printf("  %i) type:%s name:%s location:%i\n",
         i, GL_type_to_string(type), name, location);
    }
  }
}

bool ShaderProgram::is_valid() {
  glValidateProgram(program);
  int params = -1;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &params);
  printf("program %i GL_VALIDATE_STATUS = %i\n", program, params);
  if (GL_TRUE != params) {
    _print_info_log();
    return false;
  }
  return true;
}
