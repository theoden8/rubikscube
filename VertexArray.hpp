#pragma once

#include <tuple>

#include <Debug.hpp>
#include <Logger.hpp>
#include <Tuple.hpp>

#include <ShaderAttrib.hpp>

namespace gl {

template <typename... ShaderTs> class ShaderProgram;

template<typename... AttribTs>
struct VertexArray {
  using self_t = VertexArray<AttribTs...>;

  GLuint vaoId = 0;
  std::tuple<AttribTs &...> attributes;

  VertexArray(AttribTs &... attribs):
    attributes(std::tie(attribs...))
  {}

  static void init(GLuint &vao) {
    glGenVertexArrays(1, &vao); GLERROR
  }

  static void init(VertexArray &va) {
    va.init();
  }

  void init() {
    init(vaoId);
  }

  GLuint id() const {
    return vaoId;
  }

  static void bind(GLuint vao) {
    glBindVertexArray(vao); GLERROR
  }

  static void bind(self_t &vao) {
    vao.bind();
  }

  void bind() {
    bind(vaoId);
  }

  template <typename AttribT>
  void enable(const AttribT &attrib) {
    this->bind();
    glEnableVertexAttribArray(Tuple::lfind(attributes, attrib)); GLERROR
    this->unbind();
  }

  template <typename AttribT>
  void set_access(const AttribT &attrib, size_t stride=0) {
    this->bind();

    attrib.bind();

    using atype = a_cast_type<AttribT::element_type>;
    glVertexAttribPointer(Tuple::lfind(attributes, attrib),
      sizeof(typename atype::type) / sizeof(typename atype::vtype),
      gl_scalar_enum<typename atype::vtype>::value,
      GL_FALSE,
      stride,
      nullptr); GLERROR

    attrib.unbind();

    this->unbind();
  }

  template <typename AttribT>
  void disable(const AttribT &attrib) {
    this->bind();
    glDisableVertexAttribArray(Tuple::lfind(attributes, attrib)); GLERROR
    this->unbind();
  }

  template <GLenum PRIMITIVES>
  void draw(size_t start=0, size_t no_primitives=SIZE_MAX) {
    this->bind();
    if(no_primitives == SIZE_MAX) {
      size_t maxprimitives = 0;
      Tuple::for_each(attributes, [&](const auto &attr) mutable -> void {
        ASSERT(attr.buf != nullptr);
        if(attr.buf->numberOfElements > maxprimitives) {
          maxprimitives = attr.buf->numberOfElements;
        }
      });
      no_primitives = maxprimitives - start;
    }
    glDrawArrays(PRIMITIVES, start, no_primitives); GLERROR
    this->unbind();
  }

  template <GLenum PRIMITIVES, typename... ShaderTs>
  void draw(gl::ShaderProgram<ShaderTs...> &prog, size_t start=0, size_t no_primitives=SIZE_MAX) {
    gl::ShaderProgram<ShaderTs...>::use(prog);
    this->draw<PRIMITIVES>(start, no_primitives);
    gl::ShaderProgram<ShaderTs...>::unuse();
  }

  template <GLenum PRIMITIVES>
  static void draw(self_t &vao, size_t start=0, size_t no_primitives=SIZE_MAX) {
    vao.draw<PRIMITIVES>(start, no_primitives);
  }

  template <GLenum PRIMITIVES, typename... ShaderTs>
  static void draw(gl::ShaderProgram<ShaderTs...> &prog, self_t &vao, size_t start=0, size_t no_primitives=SIZE_MAX) {
    vao.draw<PRIMITIVES>(prog, start, no_primitives);
  }

  static void unbind() {
    glBindVertexArray(0); GLERROR
  }

  static void clear(GLuint &vao) {
    glDeleteVertexArrays(1, &vao); GLERROR
  }

  static void clear(VertexArray &va) {
    va.clear();
  }

  void clear() {
    clear(vaoId);
  }
};

} // namespace gl
