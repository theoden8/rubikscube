#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <incgraphics.h>

#include <Debug.hpp>

#include <Transformation.hpp>

namespace gl {
enum class AttribType {
  INTEGER, FLOAT,
  VEC2, VEC3, VEC4,
  IVEC2, IVEC3, IVEC4,
  MAT2, MAT3, MAT4
};

#define using_gltype(gltypeval) static constexpr GLenum gltype = gltypeval
template <AttribType A> struct a_cast_type { using type = void; };
template <> struct a_cast_type <AttribType::INTEGER> { using type = GLint;    using vtype = GLint;     using_gltype(GL_INT       ); };
template <> struct a_cast_type <AttribType::FLOAT> { using type = GLfloat;    using vtype  = GLfloat;  using_gltype(GL_FLOAT     ); };
template <> struct a_cast_type <AttribType::VEC2> { using type = glm::vec2;   using vtype  = GLfloat;  using_gltype(GL_FLOAT_VEC2); };
template <> struct a_cast_type <AttribType::VEC3> { using type = glm::vec3;   using vtype  = GLfloat;  using_gltype(GL_FLOAT_VEC3); };
template <> struct a_cast_type <AttribType::VEC4> { using type = glm::vec4;   using vtype  = GLfloat;  using_gltype(GL_FLOAT_VEC4); };
template <> struct a_cast_type <AttribType::IVEC2> { using type = glm::ivec2; using vtype  = GLint;    using_gltype(GL_INT_VEC2  ); };
template <> struct a_cast_type <AttribType::IVEC3> { using type = glm::ivec3; using vtype  = GLint;    using_gltype(GL_INT_VEC3  ); };
template <> struct a_cast_type <AttribType::IVEC4> { using type = glm::ivec4; using vtype  = GLint;    using_gltype(GL_INT_VEC4  ); };
template <> struct a_cast_type <AttribType::MAT2> { using type = glm::mat2;   using vtype  = GLfloat;  using_gltype(GL_FLOAT_MAT2); };
template <> struct a_cast_type <AttribType::MAT3> { using type = glm::mat3;   using vtype  = GLfloat;  using_gltype(GL_FLOAT_MAT3); };
template <> struct a_cast_type <AttribType::MAT4> { using type = glm::mat4;   using vtype  = GLfloat;  using_gltype(GL_FLOAT_MAT4); };
#undef using_gltype

template <typename T> struct gl_scalar_enum { static constexpr GLenum value = 0; };
template <> struct gl_scalar_enum<GLint> { static constexpr GLenum value = GL_INT; };
template <> struct gl_scalar_enum<GLfloat> { static constexpr GLenum value = GL_FLOAT; };

template <typename T>
struct AttribBufferAccessor {
  T attrib;
  size_t i;

  AttribBufferAccessor(T attrib, size_t i):
    attrib(attrib), i(i)
  {}

  using atype = a_cast_type<T::attrib_type>;
  AttribBufferAccessor operator=(typename atype::type vec) {
    attrib.bind();
    glBufferSubData(
      T::array_type,
      i*sizeof(typename atype::type),
      sizeof(typename atype::vtype),
      glm::value_ptr(vec)
    ); GLERROR
  }
};

template <GLenum BufferT, AttribType AttribT>
struct Attrib {
  using self_t = Attrib<BufferT, AttribT>;

  static constexpr GLenum array_type = BufferT;
  static constexpr AttribType attrib_type = AttribT;
  std::string location = "";
  GLuint vbo = 0;

  size_t numberOfPrimitives = 0;
  size_t numberOfElements = 0;
  size_t numberOfElementsPerPrimitive = 0;
  size_t sizeOfBuffer = 0;

  Attrib(){}
  Attrib(std::string loc):
    location(loc)
  {}

  bool operator==(const self_t &other) const {
    return location == other.location && vbo == other.vbo;
  }

  static void init(GLuint &vbo) {
    glGenBuffers(1, &vbo); GLERROR
  }

  static void init(Attrib<BufferT, AttribT> &attr) {
    attr.init();
  }

  void init() {
    init(vbo);
  }

  GLuint id() const {
    return vbo;
  }

  GLuint loc(GLuint program_id) const {
    ASSERT(location != "");
    GLuint lc = glGetAttribLocation(program_id, location.c_str()); GLERROR
    return lc;
  }

  static void bind(GLuint vbo) {
    glBindBuffer(BufferT, vbo); GLERROR
  }

  static void bind(const Attrib<BufferT, AttribT> &attr) {
    attr.bind();
  }

  void bind() const {
    bind(vbo);
  }

  bool is_active(GLuint program_id) {
    char name[81];
    GLsizei length;
    GLint size;
    GLenum t;
    glGetActiveAttrib(program_id, vbo, 80, &length, &size, &t, name); GLERROR
    return t == a_cast_type<AttribT>::gltype && location == name;
  }

  template <GLenum DRAW_MODE, typename T>
  void allocate(const std::vector<T> &host_data, size_t start = 0, size_t count = SIZE_MAX) {
    if(count == SIZE_MAX) {
      count = host_data.size() - start;
    }
    constexpr size_t sizeOfElement = sizeof(typename a_cast_type<AttribT>::vtype);
    constexpr size_t sizeOfPrimitive = sizeof(typename a_cast_type<AttribT>::type);
    constexpr size_t primitive_size = sizeOfPrimitive / sizeOfElement;
    static_assert(sizeOfPrimitive / sizeOfElement * sizeOfElement == sizeOfPrimitive);
    numberOfElements = count;
    numberOfPrimitives = numberOfElements / primitive_size;
    numberOfElementsPerPrimitive = primitive_size;
    sizeOfBuffer = numberOfElements * sizeOfElement;
    ASSERT(numberOfPrimitives * numberOfElementsPerPrimitive == numberOfElements);

    this->bind();
    glBufferData(BufferT, sizeOfBuffer, &host_data[start], DRAW_MODE); GLERROR
    Logger::Info("allocated buffer data: bytes=%lu, no_elems=%lu, no_primitives=%lu, no_elems_per_primitive\n",
                 sizeOfBuffer, numberOfElements, numberOfPrimitives, numberOfElementsPerPrimitive);
    this->unbind();
  }

  template <GLenum DRAW_MODE, typename T>
  void allocate(std::vector<T> &&host_data, size_t start = 0, size_t count = SIZE_MAX) {
    allocate<DRAW_MODE>(host_data, start, count);
  }

  template <typename VecT>
  void show_transformation(Transformation &t, std::vector<float> &&points) {
    size_t no_prims = points.size() / numberOfElementsPerPrimitive;
    for(size_t i = 0; i < no_prims; ++i) {
      VecT v;
      Logger::Info("primitive %d (", i);
      for(int j = 0; j < numberOfElementsPerPrimitive; ++j) {
        v[j] = points[i * numberOfElementsPerPrimitive + j];
        Logger::Info("%.2f ", v[j]);
      }
      Logger::Info(") --> ");
      v = t * v;
      for(int j = 0; j < numberOfElementsPerPrimitive; ++j) {
        Logger::Info("%.2f ", v[j]);
      }
      Logger::Info(")\n");
    }
  }

  decltype(auto) operator[](size_t i) {
    return AttribBufferAccessor<Attrib<BufferT, AttribT>>(*this, i);
  }

  static void unbind() {
    glBindBuffer(BufferT, 0); GLERROR
  }

  static void clear(GLuint &vbo) {
    glDeleteBuffers(1, &vbo); GLERROR
  }

  static void clear(Attrib<BufferT, AttribT> &attr) {
    attr.clear();
  }

  void clear() {
    clear(vbo);
  }
};

} // namespace gl
