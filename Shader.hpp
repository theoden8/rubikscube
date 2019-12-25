#pragma once

#include <incgraphics.h>

#include <File.hpp>
#include <Debug.hpp>
#include <Logger.hpp>

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cassert>

namespace gl {
enum class ShaderType {
  VERTEX, TESS_CNTRL, TESS_EVAL,
  GEOMETRY, FRAGMENT, COMPUTE,
  NO_TYPE
};

template <ShaderType sT>
constexpr GLenum get_gl_shader_constant() {
  switch(sT) {
    case ShaderType::VERTEX: return GL_VERTEX_SHADER;
    case ShaderType::TESS_CNTRL: return GL_TESS_CONTROL_SHADER;
    case ShaderType::TESS_EVAL: return GL_TESS_EVALUATION_SHADER;
    case ShaderType::GEOMETRY: return GL_GEOMETRY_SHADER;
    case ShaderType::FRAGMENT: return GL_FRAGMENT_SHADER;
    case ShaderType::COMPUTE: return GL_COMPUTE_SHADER;
  }
  static_assert(sT != ShaderType::NO_TYPE, "");
}

template <ShaderType ShaderT>
struct Shader {
  sys::File file;
  GLuint shaderId = 0;

  Shader(std::string filename):
    file(filename.c_str())
  {
    if(file.is_ext(".vert")) { ASSERT(ShaderT == gl::ShaderType::VERTEX); } else
    if(file.is_ext(".tesc")) { ASSERT(ShaderT == gl::ShaderType::TESS_CNTRL); } else
    if(file.is_ext(".tese")) { ASSERT(ShaderT == gl::ShaderType::TESS_EVAL); } else
    if(file.is_ext(".geom")) { ASSERT(ShaderT == gl::ShaderType::GEOMETRY); } else
    if(file.is_ext(".frag")) { ASSERT(ShaderT == gl::ShaderType::FRAGMENT); } else
    if(file.is_ext(".comp")) { ASSERT(ShaderT == gl::ShaderType::COMPUTE); } else {
      throw std::runtime_error("unknown shader type");
    }
  }

  ~Shader()
  {}

  int id() const {
    return shaderId;
  }

  void init() {
    shaderId = glCreateShader(gl::get_gl_shader_constant<ShaderT>()); GLERROR
    std::string source_code = file.load_text();
    const char *source = source_code.c_str();
    glShaderSource(shaderId, 1, &source, nullptr); GLERROR
    glCompileShader(shaderId); GLERROR
  }

  static void init(Shader<ShaderT> &shader) {
    shader.init();
  }

  void clear() {
    glDeleteShader(shaderId); GLERROR
  }

  static void clear(Shader<ShaderT> &shader) {
    shader.clear();
  }
};

using VertexShader = Shader<ShaderType::VERTEX>;
using GeometryShader = Shader<ShaderType::GEOMETRY>;
using FragmentShader = Shader<ShaderType::FRAGMENT>;
using ComputeShader = Shader<ShaderType::COMPUTE>;

} // namespace gl
