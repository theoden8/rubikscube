#pragma once


#include <ShaderAttrib.hpp>
#include <Shader.hpp>
#include <ShaderProgram.hpp>


class QuadCommon {
  const std::vector<float>
    x_front = {
      0,0,1, 0,1,1, 0,1,0,
      0,1,0, 0,0,0, 0,0,1,
    },
    x_back = {
      1,0,1, 1,1,0, 1,1,1,
      1,1,0, 1,0,1, 1,0,0,
    },
    y_front = {
      0,0,1, 1,0,0, 1,0,1,
      1,0,0, 0,0,1, 0,0,0,
    },
    y_back = {
      0,1,1, 1,1,1, 1,1,0,
      1,1,0, 0,1,0, 0,1,1,
    },
    z_front = {
      0,1,0, 1,1,0, 1,0,0,
      1,0,0, 0,0,0, 0,1,0,
    },
    z_back = {
      0,1,1, 1,0,1, 1,1,1,
      1,0,1, 0,1,1, 0,0,1,
    };

public:
  using ShaderProgram = gl::ShaderProgram<
    gl::VertexShader,
    gl::GeometryShader,
    gl::FragmentShader
  >;
  using ShaderBuffer = gl::Buffer<GL_ARRAY_BUFFER, gl::BufferElementType::VEC3>;
  using ShaderAttrib = gl::Attrib<ShaderBuffer>;
  using VertexArray = gl::VertexArray<ShaderAttrib>;

  ShaderProgram program;
  ShaderBuffer
    vpos_xf, vpos_xb,
    vpos_yf, vpos_yb,
    vpos_zf, vpos_zb;
  ShaderAttrib a_position;
  VertexArray vao;

  static constexpr int VAO_ATTRIB_VPOSITION = 0;

/*   gl::VertexShader vshader; */
/*   gl::GeometryShader gshader; */
/*   gl::FragmentShader fshader; */

  QuadCommon(std::string vpos_name, const std::string &dir):
    program({
      dir + "shaders/shader.vert",
      dir + "shaders/shader.geom",
      dir + "shaders/shader.frag"
    }),
    vpos_xf(),
    vpos_xb(),
    vpos_yf(),
    vpos_yb(),
    vpos_zf(),
    vpos_zb(),
    a_position(vpos_name),
    vao(a_position)
  {}

  void init() {
    ShaderBuffer::init(vpos_xf);
    vpos_xf.allocate<GL_STATIC_DRAW>(x_front);

    ShaderBuffer::init(vpos_xb);
    vpos_xb.allocate<GL_STATIC_DRAW>(x_back);

    ShaderBuffer::init(vpos_yf);
    vpos_yf.allocate<GL_STATIC_DRAW>(y_front);

    ShaderBuffer::init(vpos_yb);
    vpos_yb.allocate<GL_STATIC_DRAW>(y_back);

    ShaderBuffer::init(vpos_zf);
    vpos_zf.allocate<GL_STATIC_DRAW>(z_front);

    ShaderBuffer::init(vpos_zb);
    vpos_zb.allocate<GL_STATIC_DRAW>(z_back);

    VertexArray::init(vao);
    VertexArray::bind(vao);

    a_position.select_buffer(vpos_xf);
    vao.enable(a_position);

    ShaderProgram::init(program, vao);

    vao.disable(a_position);
    VertexArray::unbind();
  }

  void clear() {
    ShaderBuffer::clear(vpos_xf);
    ShaderBuffer::clear(vpos_xb);
    ShaderBuffer::clear(vpos_yf);
    ShaderBuffer::clear(vpos_yb);
    ShaderBuffer::clear(vpos_zf);
    ShaderBuffer::clear(vpos_zb);
    ShaderAttrib::clear(a_position);
    VertexArray::clear(vao);
    ShaderProgram::clear(program);
  }
};
