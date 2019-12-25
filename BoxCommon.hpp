#pragma once


#include <ShaderAttrib.hpp>
#include <Shader.hpp>


class BoxCommon {
public:
  gl::Attrib<GL_ARRAY_BUFFER, gl::AttribType::VEC3> a_position;

  using ShaderAttrib = decltype(a_position);

  gl::VertexShader vshader;
  gl::GeometryShader gshader;
  gl::FragmentShader fshader;

  BoxCommon(std::string vpos_name, const std::string &dir):
    a_position(vpos_name),
    vshader(dir + "shader.vert"),
    gshader(dir + "shader.geom"),
    fshader(dir + "shader.frag")
  {}

  void init() {
    ShaderAttrib::init(a_position);
    a_position.allocate<GL_STATIC_DRAW>(std::vector<GLfloat>{
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

    gl::VertexShader::init(vshader);
    gl::GeometryShader::init(gshader);
    gl::FragmentShader::init(fshader);
  }

  void finish_init() {
    gl::VertexShader::clear(vshader);
    gl::GeometryShader::clear(gshader);
    gl::FragmentShader::clear(fshader);
  }

  void clear() {
    ShaderAttrib::clear(a_position);
  }
};
