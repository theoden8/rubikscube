#pragma once

#include <string>
#include <type_traits>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <incgraphics.h>
#include <Debug.hpp>
#include <Logger.hpp>

namespace gl {
enum class UniformType {
  INTEGER, FLOAT,
  VEC2, VEC3, VEC4,
  MAT2, MAT3, MAT4,
  SAMPLER2D
};

#define using_sc static constexpr GLenum
template <UniformType U> struct u_cast_type { using type = void; };
template <> struct u_cast_type <UniformType::INTEGER> {   using type = GLint;      using vtype = int;   using_sc gltype = GL_INT; };
template <> struct u_cast_type <UniformType::FLOAT> {     using type = GLfloat;    using vtype = float; using_sc gltype = GL_FLOAT; };
template <> struct u_cast_type <UniformType::VEC2> {      using type = glm::vec2;  using vtype = float; using_sc gltype = GL_FLOAT_VEC2; };
template <> struct u_cast_type <UniformType::VEC3> {      using type = glm::vec3;  using vtype = float; using_sc gltype = GL_FLOAT_VEC3; };
template <> struct u_cast_type <UniformType::VEC4> {      using type = glm::vec4;  using vtype = float; using_sc gltype = GL_FLOAT_VEC4; };
template <> struct u_cast_type <UniformType::MAT2> {      using type = glm::mat2;  using vtype = float; using_sc gltype = GL_FLOAT_MAT2; };
template <> struct u_cast_type <UniformType::MAT3> {      using type = glm::mat3;  using vtype = float; using_sc gltype = GL_FLOAT_MAT3; };
template <> struct u_cast_type <UniformType::MAT4> {      using type = glm::mat4;  using vtype = float; using_sc gltype = GL_FLOAT_MAT4; };
template <> struct u_cast_type <UniformType::SAMPLER2D> { using type = GLuint;     using vtype = int;   using_sc gltype = GL_SAMPLER_2D; };
#undef using_sc

template <UniformType U>
struct Uniform {
  using type = typename u_cast_type<U>::type;
  using vtype = typename u_cast_type<U>::vtype;
  static constexpr GLenum gltype = u_cast_type<U>::gltype;
  static constexpr size_t element_size = sizeof(type) / sizeof(vtype);
  using dtype = std::conditional_t<
    std::is_fundamental<type>::value,
      std::remove_reference_t<type>,
      std::add_const_t<std::add_lvalue_reference_t<type>
    >
  >;
  GLuint uniformId = 0;
  GLuint progId = 0;
  std::string location;
  Uniform(std::string &loc):
    location(loc)
  {}
  Uniform(std::string &&loc):
    location(loc)
  {}
  GLuint id() const {
    return uniformId;
  }
  GLuint loc() const {
    if(location == "") {
      TERMINATE("location is unset\n");
    }
    GLuint lc = glGetUniformLocation(progId, location.c_str()); GLERROR
    return lc;
  }
  void set_id(GLuint program_id) {
    if(program_id == 0) {
      TERMINATE("program id is already set\n");
    }
    if(progId == program_id) {
      return;
    }
    progId = program_id;
    uniformId = loc();
  }
  bool is_active() {
    char name[81];
    GLsizei length;
    GLint size;
    GLenum t;
    glGetActiveUniform(progId, uniformId, 80, &length, &size, &t, name); GLERROR
    return t == gltype && location == name;
  }
  void unset_id() {
    progId = 0;
  }
  void set_data(dtype data);
  void set_data(std::vector<vtype> &data);
};

#define CHECK_PROGRAM_ID \
  if(progId == 0) { \
    TERMINATE("unable to set data to a uniform without program id set\n"); \
  }

template <>
void gl::Uniform<gl::UniformType::INTEGER>::set_data(Uniform<gl::UniformType::INTEGER>::dtype data) {
  CHECK_PROGRAM_ID;
  glUniform1i(uniformId, data); GLERROR
}

template <>
void gl::Uniform<gl::UniformType::FLOAT>::set_data(Uniform<gl::UniformType::FLOAT>::dtype data) {
  CHECK_PROGRAM_ID;
  glUniform1f(uniformId, data); GLERROR
}

template <>
void gl::Uniform<gl::UniformType::VEC2>::set_data(Uniform<gl::UniformType::VEC2>::dtype data) {
  CHECK_PROGRAM_ID;
  glUniform2f(uniformId, data.x, data.y); GLERROR
}

template <>
void gl::Uniform<gl::UniformType::VEC3>::set_data(Uniform<gl::UniformType::VEC3>::dtype data) {
  CHECK_PROGRAM_ID;
  glUniform3f(uniformId, data.x, data.y, data.z); GLERROR
}

template <>
void gl::Uniform<gl::UniformType::VEC3>::set_data(std::vector<Uniform<gl::UniformType::VEC3>::vtype> &data) {
  CHECK_PROGRAM_ID;
  glUniform3fv(uniformId, data.size() / element_size, data.data()); GLERROR
}

template <>
void Uniform<UniformType::VEC4>::set_data(Uniform<UniformType::VEC4>::dtype data) {
  CHECK_PROGRAM_ID;
  glUniform4f(uniformId, data.x, data.y, data.z, data.t); GLERROR
}

template <>
void Uniform<UniformType::MAT2>::set_data(Uniform<UniformType::MAT2>::dtype data) {
  CHECK_PROGRAM_ID;
  glUniformMatrix2fvARB(uniformId, 1 , GL_FALSE, glm::value_ptr(data)); GLERROR
}

template <>
void Uniform<UniformType::MAT3>::set_data(Uniform<UniformType::MAT3>::dtype data) {
  CHECK_PROGRAM_ID;
  glUniformMatrix3fvARB(uniformId, 1 , GL_FALSE, glm::value_ptr(data)); GLERROR
}

template <>
void Uniform<UniformType::MAT4>::set_data(Uniform<UniformType::MAT4>::dtype data) {
  CHECK_PROGRAM_ID;
  glUniformMatrix4fvARB(uniformId, 1 , GL_FALSE, glm::value_ptr(data)); GLERROR
}

template <>
void Uniform<UniformType::SAMPLER2D>::set_data(Uniform<UniformType::SAMPLER2D>::dtype data) {
  CHECK_PROGRAM_ID;
  glUniform1i(uniformId, data); GLERROR
}

#undef CHECK_PROGRAM_ID
}
