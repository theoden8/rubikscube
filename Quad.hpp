#pragma once


#include <ShaderAttrib.hpp>


class Quad {
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

  Quad(std::string name):
    vpos_xf(name),
    vpos_xb(name),
    vpos_yf(name),
    vpos_yb(name),
    vpos_zf(name),
    vpos_zb(name)
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
  }

  void clear() {
    vpos_xf.clear();
    vpos_xb.clear();
    vpos_yf.clear();
    vpos_yb.clear();
    vpos_zf.clear();
    vpos_zb.clear();
  }
};
