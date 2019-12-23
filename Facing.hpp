#pragma once

#include <list>

#include <glm/glm.hpp>

#include <ShaderProgram.hpp>
#include <ShaderAttrib.hpp>
#include <ShaderUniform.hpp>

#include <Transformation.hpp>
#include <Camera.hpp>

using namespace std::literals::string_literals;

enum class Face {
  NFACE  ,
  Z_FRONT,
  Z_BACK ,
  Y_FRONT,
  Y_BACK ,
  X_FRONT,
  X_BACK ,
};


template <typename T>
decltype(auto) get(std::list<T> &l, int i) {
  int j =  0;
  for(auto &x : l) {
    if(i==j)return x;
    ++j;
  }
  TERMINATE("out of range");
}


class Facing {
  Transformation transform;
  Transformation cubeTransform;
  glm::mat4 matrix;
  int highlight = 0;
  std::array<Face, 3> face_types;
  std::list<glm::vec3> colors;

  using ShaderProgram = gl::ShaderProgram<
    gl::VertexShader,
    gl::GeometryShader,
    gl::FragmentShader
  >;
  using ShaderAttrib = gl::Attrib<GL_ARRAY_BUFFER, gl::AttribType::VEC3>;
  using VertexArray = gl::VertexArray<ShaderAttrib>;

  std::list<ShaderProgram> prog;
  std::list<ShaderAttrib> a_position;
  std::list<VertexArray> vao;
  using UniformInt = gl::Uniform<gl::UniformType::INTEGER>;
  using UniformVec3 = gl::Uniform<gl::UniformType::VEC3>;
  using UniformMat4 = gl::Uniform<gl::UniformType::MAT4>;
  std::list<UniformInt> u_highlight;
  std::list<UniformVec3> u_color;
  std::list<UniformMat4> u_transform;

  Camera &cam;

public:
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

  Facing(Camera &cam, const std::array<Face, 3> &face_types, float scale=1., glm::vec3 position=glm::vec3(0,0,0)):
    face_types(face_types),
    cam(cam)
  {
    /* prog.reserve(3); */
    /* a_position.reserve(3); */
    /* vao.reserve(3); */
    /* u_highlight.reserve(3); */
    /* u_color.reserve(3); */
    /* u_transform.reserve(3); */

    transform.SetScale(scale);
    transform.SetPosition(position);
    /* transform.SetRotation(1, 0, 0, 180.f); */
  }

  bool has_face(Face f) {
    switch(f) {
      case Face::X_FRONT:return face_types[0]==f;break;
      case Face::X_BACK :return face_types[0]==f;break;
      case Face::Y_FRONT:return face_types[1]==f;break;
      case Face::Y_BACK :return face_types[1]==f;break;
      case Face::Z_FRONT:return face_types[2]==f;break;
      case Face::Z_BACK :return face_types[2]==f;break;
      default:break;
    }
    return false;
  }

  template <typename F>
  void iterate_point_sets(F &&func) {
    if(has_face(Face::X_FRONT))func(x_front);
    if(has_face(Face::X_BACK)) func(x_back);
    if(has_face(Face::Y_FRONT))func(y_front);
    if(has_face(Face::Y_BACK)) func(y_back);
    if(has_face(Face::Z_FRONT))func(z_front);
    if(has_face(Face::Z_BACK)) func(z_back);
  }

  int no_faces() {
    int q = 0;
    if(face_types[0] != Face::NFACE)++q;
    if(face_types[1] != Face::NFACE)++q;
    if(face_types[2] != Face::NFACE)++q;
    return q;
  }


  void init() {
    iterate_point_sets([&](const std::vector<float> &points) mutable -> void {
      Logger::Info("  face\n");
      colors.push_back(glm::vec3(1,1,1));

      a_position.push_back(ShaderAttrib("vposition"));
      auto &current_a_position = a_position.back();
      current_a_position.allocate<GL_STATIC_DRAW>(points);

      vao.push_back(VertexArray(current_a_position));
      auto &current_vao = vao.back();

      prog.push_back(ShaderProgram({
        "shader.vert"s,
        "shader.geom"s,
        "shader.frag"s
      }));
      auto &current_prog = prog.back();

      VertexArray::init(current_vao);
      VertexArray::bind(current_vao);

      current_vao.enable(current_a_position);

      current_a_position.set_access(0, 0);

      ShaderProgram::init(current_prog, current_vao);

      u_highlight.push_back(UniformInt("highlight"));
      auto &current_u_highlight = u_highlight.back();
      current_u_highlight.set_id(current_prog.id());

      u_color.push_back(UniformVec3("color"));
      auto &current_u_color = u_color.back();
      current_u_color.set_id(current_prog.id());

      u_transform.push_back(UniformMat4("transform"));
      auto &current_u_transform = u_transform.back();
      current_u_transform.set_id(current_prog.id());
    });
  }

  void update_uniforms(int id) {
    if(transform.has_changed || cubeTransform.has_changed || cam.has_changed()) {
      matrix = cam.get_matrix() * cubeTransform.get_matrix() * transform.get_matrix();
      get(u_transform, id).set_data(matrix);
    }
    get(u_highlight, id).set_data(highlight);
    get(u_color, id).set_data(get(colors, id));
  }

  int face_to_index(Face &f) {
    int q = 0;
    if(f == Face::X_FRONT || f == Face::X_BACK)return q;
    if(face_types[0] != Face::NFACE)++q;
    if(f == Face::Y_FRONT || f == Face::Y_BACK)return q;
    if(face_types[1] != Face::NFACE)++q;
    if(f == Face::Z_FRONT || f == Face::Z_BACK)return q;
    return -1;
  }

  template <typename F>
  void select_face(Face &f, F &&func) {
    if(has_face(f)) {
      int id = face_to_index(f);
      func(
        get(colors, id),
        cubeTransform,
        highlight
      );
    }
  }

  void draw() {
    for(int i = 0; i < prog.size(); ++i) {
      ShaderProgram::use(get(prog, i));
      update_uniforms(i);
      VertexArray::draw<GL_TRIANGLES>(get(vao, i));
      ShaderProgram::unuse();
    }
    transform.has_changed = false;
    cubeTransform.has_changed = false;
  }

  void clear() {
    for(int i = 0; i < prog.size(); ++i) {
      ShaderAttrib::clear(get(a_position, i));
      VertexArray::clear(get(vao, i));
      ShaderProgram::clear(get(prog, i));
    }
  }
};
