#pragma once

#include <tuple>

#include <glm/glm.hpp>

#include <ShaderProgram.hpp>
#include <ShaderAttrib.hpp>
#include <ShaderUniform.hpp>

#include <Transformation.hpp>
#include <Camera.hpp>

using namespace std::literals::string_literals;


class Box {
public:
  Transformation transform;

  using ShaderAttrib = BoxCommon::ShaderAttrib;
  using VertexArray = BoxCommon::VertexArray;
  using ShaderProgram = BoxCommon::ShaderProgram;

private:
  glm::mat4 matrix;
  int highlight = 1;
  glm::vec3 color;

  ShaderProgram &prog;
  VertexArray &vao;

  gl::Uniform<gl::UniformType::INTEGER> u_highlight;
  gl::Uniform<gl::UniformType::VEC3> u_color;
  gl::Uniform<gl::UniformType::MAT4> u_transform;

  Camera &cam;
  BoxCommon &boxcommon;

public:
  Box(BoxCommon &boxcommon, Camera &cam, const glm::vec3 &color, float scale=1.):
    prog(boxcommon.program),
    vao(boxcommon.vao),
    u_transform("transform"),
    u_highlight("highlight"),
    u_color("color"),
    color(color),
    cam(cam),
    boxcommon(boxcommon)
  {
    transform.SetScale(scale);
    transform.SetPosition(0, 0, 0);
  }

  void init() {
    u_transform.set_id(prog.id());
    u_highlight.set_id(prog.id());
    u_color.set_id(prog.id());
  }

  template <typename... Ts>
  void update_uniforms(std::tuple<const Ts&... >transforms) {
    bool changed = transform.has_changed || cam.has_changed;
    Tuple::for_each(transforms, [&](auto &t) mutable -> void {
      changed = changed || t.has_changed;
    });
    if(changed) {
      matrix = cam.get_matrix();
      Tuple::for_each(transforms, [&](auto &t) mutable -> void {
        matrix *= t.get_matrix();
      });
      matrix *= transform.get_matrix();
      u_transform.set_data(matrix);
      u_highlight.set_data(highlight);
      u_color.set_data(color);
      transform.has_changed = false;
    }
  }

  template <typename... TransformTs>
  void draw(const TransformTs&... transforms) {
    ShaderProgram::use(prog);
    update_uniforms(std::tie(transforms...));
    VertexArray::draw<GL_TRIANGLES>(vao);
    ShaderProgram::unuse();
  }

  void clear() {
  }
};
