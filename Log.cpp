#include <ctime>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <cstdlib>

#include "Log.hpp"

const char *log_file = "rubik.log";

bool restart_gl_log() {
  FILE* file = fopen(log_file, "w");
  if(!file) {
    fprintf(stderr,
      "ERROR: could not open GL_LOG_FILE log file %s for writing\n",
      log_file);
    return false;
  }
  time_t now = time(NULL);
  char* date = ctime(&now);
  fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
  fclose(file);
  return true;
}

bool gl_log(const char* message, ...) {
  va_list argptr;
  FILE* file = fopen(log_file, "a");
  if(!file) {
    fprintf(
      stderr,
      "ERROR: could not open GL_LOG_FILE %s file for appending\n",
      log_file
      );
    return false;
  }
  va_start(argptr, message);
  vfprintf(file, message, argptr);
  va_end(argptr);
  fclose(file);
  return true;
}

bool gl_log_err(const char* message, ...) {
  va_list argptr;
  FILE* file = fopen(log_file, "a");
  if(!file) {
    fprintf(stderr,
            "ERROR: could not open GL_LOG_FILE %s file for appending\n",
            log_file);
    return false;
  }
  va_start(argptr, message);
  vfprintf(file, message, argptr);
  va_end(argptr);
  va_start(argptr, message);
  vfprintf(stderr, message, argptr);
  va_end(argptr);
  fclose(file);
  return true;
}

void glfw_error_callback(int error, const char* description) {
  gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

void log_gl_params() {
  GLenum params[] = {
    GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
    GL_MAX_CUBE_MAP_TEXTURE_SIZE,
    GL_MAX_DRAW_BUFFERS,
    GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
    GL_MAX_TEXTURE_IMAGE_UNITS,
    GL_MAX_TEXTURE_SIZE,
    GL_MAX_VARYING_FLOATS,
    GL_MAX_VERTEX_ATTRIBS,
    GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
    GL_MAX_VERTEX_UNIFORM_COMPONENTS,
    GL_MAX_VIEWPORT_DIMS,
    GL_STEREO,
  };
  const char* names[] = {
    "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
    "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
    "GL_MAX_DRAW_BUFFERS",
    "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
    "GL_MAX_TEXTURE_IMAGE_UNITS",
    "GL_MAX_TEXTURE_SIZE",
    "GL_MAX_VARYING_FLOATS",
    "GL_MAX_VERTEX_ATTRIBS",
    "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
    "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
    "GL_MAX_VIEWPORT_DIMS",
    "GL_STEREO",
  };
  gl_log("GL Context Params:\n");
  char msg[256];
  // integers - only works if the order is 0-10 integer return types
  for (int i = 0; i < 10; i++) {
    int v = 0;
    glGetIntegerv(params[i], &v);
    gl_log("%s %i\n", names[i], v);
  }
  // others
  int v[2];
  v[0] = v[1] = 0;
  glGetIntegerv(params[10], v);
  gl_log("%s %i %i\n", names[10], v[0], v[1]);
  unsigned char s = 0;
  glGetBooleanv(params[11], &s);
  gl_log("%s %u\n", names[11], (unsigned int)s);
  gl_log("-----------------------------\n");
}

const char* GL_type_to_string(GLenum type) {
  switch(type) {
    case GL_BOOL: return "bool";
    case GL_INT: return "int";
    case GL_FLOAT: return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_SAMPLER_2D: return "sampler2D";
    case GL_SAMPLER_3D: return "sampler3D";
    case GL_SAMPLER_CUBE: return "samplerCube";
    case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
    default: break;
  }
  return "other";
}

void describe_error(GLenum code) {
  std::string err;
  switch(code) {
    case GL_NO_ERROR:
      err = "no error";
    break;
    case GL_INVALID_ENUM:
      err = "invalid enum";
    break;
    case GL_INVALID_VALUE:
      err = "invalid value";
    break;
    case GL_INVALID_OPERATION:
      err = "invalid operation";
    break;
    case GL_STACK_OVERFLOW:
      err = "stack overflow";
    break;
    case GL_STACK_UNDERFLOW:
      err = "stack underflow";
    break;
    case GL_OUT_OF_MEMORY:
      err = "out of memory";
    break;
    case GL_TABLE_TOO_LARGE:
      err = "table too large";
    break;
    default:
      err = "unknown_error " + std::to_string(code) + "\n";
    break;
  }
  std::cerr << err << std::endl;
}
