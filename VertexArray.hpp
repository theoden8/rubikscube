#pragma once


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
    int ind = Tuple::lfind(attributes, attrib);
    ASSERT(ind != -1);
    glEnableVertexAttribArray(ind); GLERROR
  }

  template <GLenum PRIMITIVES>
  void draw(size_t start=0, size_t no_primitives=SIZE_MAX) {
    this->bind();
    if(no_primitives == SIZE_MAX) {
      size_t maxprimitives = 0;
      Tuple::for_each(attributes, [&](const auto &attr) mutable -> void {
        if(attr.numberOfPrimitives > maxprimitives) {
          maxprimitives = attr.numberOfPrimitives;
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
