#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <incgraphics.h>

#include <Debug.hpp>
#include <Logger.hpp>

#include <ShaderBuffer.hpp>

namespace gl {

template <typename ShaderBuffer> struct Attrib;

template <GLenum BufferT, BufferElementType ElementT>
struct Attrib <gl::Buffer<BufferT, ElementT>> {
  using ShaderBuffer = gl::Buffer<BufferT, ElementT>;
  using self_t = Attrib<ShaderBuffer>;

  static constexpr GLenum array_type = ShaderBuffer::array_type;
  static constexpr BufferElementType element_type = ShaderBuffer::element_type;

  std::string location = "";
  ShaderBuffer *buf = nullptr;

  Attrib(){}
  Attrib(std::string loc):
    location(loc)
  {}

  Attrib(std::string loc, ShaderBuffer &shaderbuffer):
    location(loc)
  {
    select_buffer(shaderbuffer);
  }

  bool operator==(const self_t &other) const {
    return location == other.location;
  }

  GLuint id() const {
    ASSERT(buf != nullptr);
    return buf->id();
  }

  void select_buffer(ShaderBuffer &sel_buf) {
    buf = &sel_buf;
  }

  GLuint loc(GLuint program_id) const {
    ASSERT(location != "");
    GLuint lc = glGetAttribLocation(program_id, location.c_str()); GLERROR
    return lc;
  }

  static void bind(const self_t &attr) {
    attr.bind();
  }

  void bind()const {
    ASSERT(buf != nullptr);
    ShaderBuffer::bind(*buf);
  }

  bool is_active(GLuint program_id) {
    ASSERT(buf != nullptr);
    char name[81];
    GLsizei length;
    GLint size;
    GLenum t;
    glGetActiveAttrib(program_id, buf->vbo, 80, &length, &size, &t, name); GLERROR
    return t == a_cast_type<ElementT>::gltype && location == name;
  }

  decltype(auto) operator[](size_t i) {
    ASSERT(buf != nullptr);
    return (*buf)[i];
  }

  void deselect_buffer() {
    buf = nullptr;
  }

  static void unbind() {
    glBindBuffer(BufferT, 0); GLERROR
  }

  static void clear(self_t &attr) {
    attr.clear();
  }

  void clear() {
    if(buf != nullptr) {
      deselect_buffer();
    }
  }
};

} // namespace gl
