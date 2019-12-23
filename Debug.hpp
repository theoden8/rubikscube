#pragma once

#include <stdexcept>
#include <iostream>
#include <string>

#ifdef COMPILE_GLDEBUG
#include <incgraphics.h>
#endif

#ifdef COMPILE_ALDEBUG
#include <incaudio.h>
#endif

#define STR(x) #x
#define TOSTR(x) STR(x)
#define ERROR(format) fprintf(stderr, "error: " format "\n");

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define CODE_LOCATION "\033[1m" __FILE__ "\033[0m::\033[93m" TOSTRING(__LINE__) "\033[0m"
#define CONDITION_TOSTR(CONDITION) " ( \033[1;4m" STRINGIFY(CONDITION) "\033[0m )"

#define ASSERT(CONDITION) \
  if(!(CONDITION)) { \
    Logger::Error("%s\n", "\033[1;91merror\033[0m at " CODE_LOCATION CONDITION_TOSTR(CONDITION)); \
    abort(); \
  }
#define TERMINATE(...) \
  Logger::Error(__VA_ARGS__);  \
  throw std::runtime_error("\033[1;91mterminated\033[0m at " CODE_LOCATION);

#ifndef NDEBUG

#ifdef COMPILE_GLDEBUG
  #define GLERROR { GLenum ret = glGetError(); if(ret != GL_NO_ERROR) { std::cerr << explain_glerror(ret) << std::endl << ret << std::endl; ASSERT(ret == GL_NO_ERROR); } };
#else
  #define GLERROR
#endif

#else

#define GLERROR

#endif

#ifdef COMPILE_GLDEBUG
std::string explain_glerror(GLenum code) {
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
      err = "unknown_error " + std::to_string(code);
    break;
  }
  return err;
}

/* void log_gl_params() { */
/* #ifndef NDEBUG */
/*   GLenum params[] = { */
/*     GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, */
/*     GL_MAX_CUBE_MAP_TEXTURE_SIZE, */
/*     GL_MAX_DRAW_BUFFERS, */
/*     GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, */
/*     GL_MAX_TEXTURE_IMAGE_UNITS, */
/*     GL_MAX_TEXTURE_SIZE, */
/*     GL_MAX_VARYING_FLOATS, */
/*     GL_MAX_VERTEX_ATTRIBS, */
/*     GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, */
/*     GL_MAX_VERTEX_UNIFORM_COMPONENTS, */
/*     GL_MAX_VIEWPORT_DIMS, */
/*     GL_STEREO, */
/*   }; */
/*   const char* names[] = { */
/*     "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS", */
/*     "GL_MAX_CUBE_MAP_TEXTURE_SIZE", */
/*     "GL_MAX_DRAW_BUFFERS", */
/*     "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS", */
/*     "GL_MAX_TEXTURE_IMAGE_UNITS", */
/*     "GL_MAX_TEXTURE_SIZE", */
/*     "GL_MAX_VARYING_FLOATS", */
/*     "GL_MAX_VERTEX_ATTRIBS", */
/*     "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS", */
/*     "GL_MAX_VERTEX_UNIFORM_COMPONENTS", */
/*     "GL_MAX_VIEWPORT_DIMS", */
/*     "GL_STEREO", */
/*   }; */
/*   Logger::Info("GL Context Params:\n"); */
/*   char msg[256]; */
/*   // integers - only works if the order is 0-10 integer return types */
/*   for (int i = 0; i < 10; i++) { */
/*     int v = 0; */
/*     glGetIntegerv(params[i], &v); */
/*     Logger::Info("%s %i\n", names[i], v); */
/*   } */
/*   // others */
/*   int v[2]; */
/*   v[0] = v[1] = 0; */
/*   glGetIntegerv(params[10], v); */
/*   Logger::Info("%s %i %i\n", names[10], v[0], v[1]); */
/*   unsigned char s = 0; */
/*   glGetBooleanv(params[11], &s); */
/*   Logger::Info("%s %u\n", names[11], (unsigned int)s); */
/*   Logger::Info("-----------------------------\n"); */
/* #endif */
/* } */
#endif
