#pragma once


#include <ShaderAttrib.hpp>
#include <Shader.hpp>
#include <ShaderProgram.hpp>


class BoxCommon {
public:

  using ShaderAttrib = gl::Attrib<GL_ARRAY_BUFFER, gl::AttribType::VEC3>;
  using VertexArray = gl::VertexArray<ShaderAttrib>;
  using ShaderProgram = gl::ShaderProgram<
    gl::VertexShader,
    gl::GeometryShader,
    gl::FragmentShader
  >;

  ShaderProgram program;
  ShaderAttrib a_position;
  VertexArray vao;

  BoxCommon(std::string vpos_name, const std::string &dir):
    program({
      dir + "shaders/shader.vert",
      dir + "shaders/shader.geom" ,
      dir + "shaders/shader.frag"
    }),
    a_position(vpos_name),
    vao(a_position)
  {}

  void init() {
    ShaderAttrib::init(a_position);
    a_position.allocate<GL_STATIC_DRAW>(std::vector<GLfloat>{
      0,0,1, 0,1,1, 0,1,0,
      0,1,0, 0,0,0, 0,0,1,

      1,0,1, 1,1,0, 1,1,1,
      1,1,0, 1,0,1, 1,0,0,

      0,0,1, 1,0,0, 1,0,1,
      1,0,0, 0,0,1, 0,0,0,

      0,1,1, 1,1,1, 1,1,0,
      1,1,0, 0,1,0, 0,1,1,

      0,1,0, 1,1,0, 1,0,0,
      1,0,0, 0,0,0, 0,1,0,

      0,1,1, 1,0,1, 1,1,1,
      1,0,1, 0,1,1, 0,0,1,
    });

    VertexArray::init(vao);
    VertexArray::bind(vao);

    vao.set_access(a_position);

    // enable all attributes
    vao.enable(a_position);

    ShaderProgram::init(program, vao);

    VertexArray::unbind();
  }

  void clear() {
    ShaderAttrib::clear(a_position);
    VertexArray::clear(vao);
    ShaderProgram::clear(program);
  }
};
