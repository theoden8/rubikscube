#pragma once

#include <glm/glm.hpp>

#include <ShaderProgram.hpp>
#include <ShaderAttrib.hpp>
#include <ShaderUniform.hpp>

#include <Transformation.hpp>
#include <Camera.hpp>

using namespace std::literals::string_literals;


class Box {
  Transformation transform;
  glm::mat4 matrix;
  int highlight = 1;
  glm::vec3 color;

  gl::ShaderProgram<
    gl::VertexShader,
    gl::GeometryShader,
    gl::FragmentShader
  > prog;
  gl::Attrib<GL_ARRAY_BUFFER, gl::AttribType::VEC3> a_position;
  gl::VertexArray<
    decltype(a_position)
  > vao;
  gl::Uniform<gl::UniformType::INTEGER> u_highlight;
  gl::Uniform<gl::UniformType::VEC3> u_color;
  gl::Uniform<gl::UniformType::MAT4> u_transform;

  Camera &cam;

public:

  using ShaderProgram = decltype(prog);
  using ShaderAttrib = decltype(a_position);
  using VertexArray = decltype(vao);

  Box(Camera &cam, const glm::vec3 &color, float scale=1.):
    prog({
      "shader.vert"s,
      "shader.geom"s,
      "shader.frag"s
    }),
    a_position("vposition"),
    vao(a_position),
    u_transform("transform"),
    u_highlight("highlight"),
    u_color("color"),
    color(color),
    cam(cam)
  {
    transform.SetScale(scale);
    transform.SetPosition(0, 0, 0);
    /* transform.SetRotation(1, 0, 0, 180.f); */
  }

  void init() {
    ShaderAttrib::init(a_position);

    a_position.allocate<GL_STATIC_DRAW>(std::vector<GLfloat>{
      /* 1,1, -1,1, -1,-1, */
      /* -1,-1, 1,1, 1,-1, */

      // x fixed
      0,0,1, 0,1,0, 0,1,1,
      0,1,0, 0,0,0, 0,0,1,

      1,0,1, 1,1,0, 1,1,1,
      1,1,0, 1,0,0, 1,0,1,

      // y fixed
      0,0,1, 1,0,0, 1,0,1,
      1,0,0, 0,0,0, 0,0,1,

      0,1,1, 1,1,0, 1,1,1,
      1,1,0, 0,1,0, 0,1,1,

      // z fixed
      0,1,0, 1,0,0, 1,1,0,
      1,0,0, 0,0,0, 0,1,0,

      0,1,1, 1,0,1, 1,1,1,
      1,0,1, 0,0,1, 0,1,1,
    });

    VertexArray::init(vao);
    VertexArray::bind(vao);

    // enable all attributes
    vao.enable(a_position);

    a_position.set_access(0, 0);

    ShaderProgram::init(prog, vao);
    u_transform.set_id(prog.id());
    u_highlight.set_id(prog.id());
    u_color.set_id(prog.id());

    VertexArray::unbind();
  }

  void update_uniforms() {
    if(transform.has_changed || cam.has_changed()) {
      matrix = cam.get_matrix() * transform.get_matrix();
      u_transform.set_data(matrix);
      u_highlight.set_data(highlight);
      u_color.set_data(color);
    }
  }

  void draw() {
    ShaderProgram::use(prog);
    update_uniforms();
    VertexArray::draw<GL_TRIANGLES>(vao);
    ShaderProgram::unuse();
  }

  void clear() {
    ShaderAttrib::clear(a_position);
    VertexArray::clear(vao);
    ShaderProgram::clear(prog);
  }
};
