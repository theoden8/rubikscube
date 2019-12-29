#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <tuple>

#include <incgraphics.h>
#include <Tuple.hpp>
#include <Shader.hpp>
#include <ShaderAttrib.hpp>
#include <VertexArray.hpp>

namespace gl {
template <typename... ShaderTs>
class ShaderProgram {
  using self_t = ShaderProgram<ShaderTs...>;

  GLuint programId = 0;
  std::tuple<ShaderTs...> shaders;
  bool shaderOwnership;

  /* enum class ResourceType { */
  /*   UNIFORM, */
  /*   UNIFORM_BLOCK, */
  /*   PROGRAM_INPUT, */
  /*   PROGRAM_OUTPUT, */
  /*   SUBROUTINE, */
  /*   SUBROUTINE_UNIFORM, */
  /*   TRANSFORM_FEEDBACK_VARYING, */
  /*   TRANSFORM_FEEDBACK_BUFFER, */
  /*   BUFFER_VARIABLE, */
  /*   SHADER_STORAGE_BLOCK, */
  /* }; */

/*   template <ResourceType rT, ShaderType sT = ShaderType::NO_TYPE> */
/*   static constexpr GLenum get_gl_resource_type() { */
/*     return 0; */
/*     switch(rT) { */
/*       case ResourceType::UNIFORM:return GL_UNIFORM; */
/*       case ResourceType::UNIFORM_BLOCK:return GL_UNIFORM_BLOCK; */
/*       case ResourceType::PROGRAM_INPUT:return GL_PROGRAM_INPUT; */
/*       case ResourceType::PROGRAM_OUTPUT:return GL_PROGRAM_OUTPUT; */
/*       case ResourceType::SUBROUTINE: */
/*         switch(sT) { */
/*           case ShaderType::VERTEX: return GL_VERTEX_SUBROUTINE; */
/*           case ShaderType::TESS_CNTRL: return GL_TESS_CONTROL_SUBROUTINE; */
/*           case ShaderType::TESS_EVAL: return GL_TESS_EVALUATION_SUBROUTINE; */
/*           case ShaderType::GEOMETRY: return GL_GEOMETRY_SUBROUTINE; */
/*           case ShaderType::FRAGMENT: return GL_FRAGMENT_SUBROUTINE; */
/*           case ShaderType::COMPUTE: return GL_COMPUTE_SUBROUTINE; */
/*         } */
/*       case ResourceType::SUBROUTINE_UNIFORM: */
/*         switch(sT) { */
/*           case ShaderType::VERTEX: return GL_VERTEX_SUBROUTINE_UNIFORM; */
/*           case ShaderType::TESS_CNTRL: return GL_TESS_CONTROL_SUBROUTINE_UNIFORM; */
/*           case ShaderType::TESS_EVAL: return GL_TESS_EVALUATION_SUBROUTINE_UNIFORM; */
/*           case ShaderType::GEOMETRY: return GL_GEOMETRY_SUBROUTINE_UNIFORM; */
/*           case ShaderType::FRAGMENT: return GL_FRAGMENT_SUBROUTINE_UNIFORM; */
/*           case ShaderType::COMPUTE: return GL_COMPUTE_SUBROUTINE_UNIFORM; */
/*         } */
/*       case ResourceType::TRANSFORM_FEEDBACK_VARYING:return GL_TRANSFORM_FEEDBACK_VARYING; */
/*       case ResourceType::TRANSFORM_FEEDBACK_BUFFER:return GL_TRANSFORM_FEEDBACK_BUFFER; */
/*       case ResourceType::BUFFER_VARIABLE:return GL_BUFFER_VARIABLE; */
/*       case ResourceType::SHADER_STORAGE_BLOCK:return GL_SHADER_STORAGE_BLOCK; */
/*     } */
/*   } */

public:
  struct Binary {
    int size;
    GLenum format;
    void *data = nullptr;

    Binary(ShaderProgram<ShaderTs...> &program) {
      size = program.get<GL_PROGRAM_BINARY_LENGTH>();
      data = malloc(size);
      GLint written_bytes;
      glGetProgramBinary(program.id(), size, &written_bytes, &format, data); GLERROR
      ASSERT(size == written_bytes);
    }

    ~Binary() {
      free(data);
    }
  };

  static void compile_program(ShaderProgram<ShaderTs...> &program) {
    program.compile_program();
  }

  void compile_program() {
    Tuple::for_each(shaders, [&](auto &s) mutable -> void {
      if(shaderOwnership) {
        s.init();
      }
    });

    programId = glCreateProgram(); GLERROR
    ASSERT(this->programId != 0);
    Tuple::for_each(shaders, [&](auto &s) mutable -> void {
      glAttachShader(programId, s.id()); GLERROR
    });
    glLinkProgram(programId); GLERROR

    Tuple::for_each(shaders, [&](auto &s) mutable -> void {
      if(shaderOwnership) {
        s.clear();
      }
    });
    ASSERT(this->is_valid());
  }

  void bind_attrib(int index, const std::string &location) {
    glBindAttribLocation(programId, index, location.c_str()); GLERROR
  }

  template <typename... STRINGs>
  ShaderProgram(STRINGs&&... shader_filenames):
    shaders(std::forward<STRINGs>(shader_filenames)...),
    shaderOwnership(true)
  {}

  ShaderProgram(const ShaderTs &... shaders):
    shaders(std::forward<ShaderTs>(shaders)...),
    shaderOwnership(false)
  {}

  ShaderProgram(const ShaderTs &&... shaders):
    shaders(std::forward<ShaderTs>(shaders)...),
    shaderOwnership(true)
  {}

  GLuint id() const {
    return programId;
  }

  template <GLenum Parameter>
  int get() {
    GLint res;
    glGetProgramiv(id(), Parameter, &res); GLERROR
    return res;
  }

  Binary get_binary() {
    return Binary(*this);
  }

/*   template <ResourceType ResourceT, ShaderType ShaderT> */
/*   int get_resource_index(std::string name) { */
/*     constexpr GLenum vartype = get_gl_resource_type<ResourceT, ShaderT>(); */
/*     auto ind = glGetProgramResourceIndex(id(), vartype, name.c_str()); GLERROR */
/*     return ind; */
/*   } */

/*   template <ResourceType ResourceT, ShaderType ShaderT> */
/*   std::string get_resource_name(int index) { */
/*     char name[255]; */
/*     constexpr GLenum vartype = get_gl_resource_type<ResourceT, ShaderT>(); */
/*     int len; */
/*     glGetProgramResourceIndex(id(), vartype, index, 255, &len, name); GLERROR */
/*     return name; */
/*   } */

  template <typename... AttribTs>
  static void init(self_t &program, gl::VertexArray<AttribTs ...> &vao) {
    program.init(vao);
  }

  template <typename... AttribTs>
  void init(gl::VertexArray<AttribTs...> &vao) {
    gl::VertexArray<AttribTs...>::bind(vao);
    this->compile_program();
    int i = 0;
    Tuple::for_each(vao.attributes, [&](const auto &attrib) -> void {
      this->bind_attrib(i, attrib.location);
      ++i;
    });
    gl::VertexArray<AttribTs...>::unbind();
  }


  static void use(GLuint progId) {
    glUseProgram(progId); GLERROR
  }

  static void use(self_t &program) {
    program.use();
  }

  void use() {
    this->use(id());
  }

  static void dispatch(size_t x, size_t y, size_t z) {
    /* glDispatchCompute(x, y, z); GLERROR */
  }

  static void unuse() {
    glUseProgram(0); GLERROR
  }

  static void clear(self_t &program) {
    program.clear();
  }

  void clear() {
    Tuple::for_each(shaders, [&](auto &s) {
      glDetachShader(programId, s.id()); GLERROR
    });
    glDeleteProgram(programId); GLERROR
  }

  bool is_valid() {
    glValidateProgram(programId); GLERROR
    int params = get<GL_VALIDATE_STATUS>();
    Logger::Info("programId %d GL_VALIDATE_STATUS = %d\n", programId, params);
    print_info_log();
    print_all();
    return params == GL_TRUE;
  }

  static const char* GL_type_to_string(GLenum type) {
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

  void print_info_log() {
    int max_length = 2048;
    int actual_length = 0;
    char programId_log[2048];
    glGetProgramInfoLog(programId, max_length, &actual_length, programId_log); GLERROR
    Logger::Info("programId info log for GL index %u:\n%s\n", programId, programId_log);
  }

  void print_all() {
    Logger::Info("--------------------\n");
    Logger::Info("shader programId %d info:\n", programId);
    int params = -1;
    params = get<GL_LINK_STATUS>();
    Logger::Info("GL_LINK_STATUS = %d\n", params);

    params = get<GL_ATTACHED_SHADERS>();
    Logger::Info("GL_ATTACHED_SHADERS = %d\n", params);

    params = get<GL_ACTIVE_ATTRIBUTES>();
    Logger::Info("GL_ACTIVE_ATTRIBUTES = %d\n", params);
    for (int i = 0; i < params; i++) {
      char name[64];
      int max_length = 64;
      int actual_length = 0;
      int size = 0;
      GLenum type;
      glGetActiveAttrib(programId, i, max_length, &actual_length, &size, &type, name);
      if(size > 1) {
        for(int j = 0; j < size; j++) {
          char long_name[64];
          sprintf(long_name, "%s[%d]", name, j);
          int location = glGetAttribLocation(programId, long_name); GLERROR
          Logger::Info("  %d) type:%s name:%s location:%d\n", i, GL_type_to_string(type), long_name, location);
        }
      } else {
        int location = glGetAttribLocation(programId, name); GLERROR
        Logger::Info("  %d) type:%s name:%s location:%d\n", i, GL_type_to_string(type), name, location);
      }
    }

    params = get<GL_ACTIVE_UNIFORMS>();
    Logger::Info("GL_ACTIVE_UNIFORMS = %d\n", params);
    for(int i = 0; i < params; i++) {
      char name[256];
      int max_length = 64;
      int actual_length = 0;
      int size = 0;
      GLenum type;
      glGetActiveUniform(programId, i, max_length, &actual_length, &size, &type, name); GLERROR
      if(size > 1) {
        for(int j = 0; j < size; j++) {
          char long_name[256];
          sprintf(long_name, "%s[%d]", name, j);
          int location = glGetUniformLocation(programId, long_name); GLERROR
          Logger::Info("  %d) type:%s name:%s location:%d\n", i, GL_type_to_string(type), long_name, location);
        }
      } else {
        int location = glGetUniformLocation(programId, name); GLERROR
        Logger::Info("  %d) type:%s name:%s location:%d\n", i, GL_type_to_string(type), name, location);
      }
    }
  }
};
}
