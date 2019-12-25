#pragma once


#include <ShaderAttrib.hpp>
#include <Shader.hpp>


class QuadCommon {
  const std::vector<float>
    x_front = {
      0,0,1, 0,1,0, 0,1,1,
      0,1,0, 0,0,0, 0,0,1,
    },
    x_back = {
      1,0,1, 1,1,0, 1,1,1,
      1,1,0, 1,0,0, 1,0,1,
    },
    y_front = {
      0,0,1, 1,0,0, 1,0,1,
      1,0,0, 0,0,0, 0,0,1,
    },
    y_back = {
      0,1,1, 1,1,0, 1,1,1,
      1,1,0, 0,1,0, 0,1,1,
    },
    z_front = {
      0,1,0, 1,0,0, 1,1,0,
      1,0,0, 0,0,0, 0,1,0,
    },
    z_back = {
      0,1,1, 1,0,1, 1,1,1,
      1,0,1, 0,0,1, 0,1,1,
    };

public:
  using ShaderAttrib = gl::Attrib<GL_ARRAY_BUFFER, gl::AttribType::VEC3>;
  ShaderAttrib
    vpos_xf, vpos_xb,
    vpos_yf, vpos_yb,
    vpos_zf, vpos_zb;

  gl::VertexShader vshader;
  gl::GeometryShader gshader;
  gl::FragmentShader fshader;

  QuadCommon(std::string vpos_name, const std::string &dir):
    vpos_xf(vpos_name),
    vpos_xb(vpos_name),
    vpos_yf(vpos_name),
    vpos_yb(vpos_name),
    vpos_zf(vpos_name),
    vpos_zb(vpos_name),
    vshader(dir + "shader.vert"),
    gshader(dir + "shader.geom"),
    fshader(dir + "shader.frag")
  {}

  void init() {
    ShaderAttrib::init(vpos_xf);
    vpos_xf.allocate<GL_STATIC_DRAW>(x_front);

    ShaderAttrib::init(vpos_xb);
    vpos_xb.allocate<GL_STATIC_DRAW>(x_back);

    ShaderAttrib::init(vpos_yf);
    vpos_yf.allocate<GL_STATIC_DRAW>(y_front);

    ShaderAttrib::init(vpos_yb);
    vpos_yb.allocate<GL_STATIC_DRAW>(y_back);

    ShaderAttrib::init(vpos_zf);
    vpos_zf.allocate<GL_STATIC_DRAW>(z_front);

    ShaderAttrib::init(vpos_zb);
    vpos_zb.allocate<GL_STATIC_DRAW>(z_back);

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
    ShaderAttrib::clear(vpos_xf);
    ShaderAttrib::clear(vpos_xb);
    ShaderAttrib::clear(vpos_yf);
    ShaderAttrib::clear(vpos_yb);
    ShaderAttrib::clear(vpos_zf);
    ShaderAttrib::clear(vpos_zb);
  }
};
