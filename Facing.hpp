#pragma once

#include <list>

#include <glm/glm.hpp>

#include <ShaderProgram.hpp>
#include <ShaderAttrib.hpp>
#include <ShaderUniform.hpp>

#include <Face.hpp>
#include <QuadCommon.hpp>
#include <Box.hpp>
#include <Transformation.hpp>
#include <Camera.hpp>

using namespace std::literals::string_literals;


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
public:
  std::array<Face, 3> face_types;
private:
  std::list<glm::vec3> colors;

  using ShaderProgram = QuadCommon::ShaderProgram;
  using ShaderAttrib = QuadCommon::ShaderAttrib;
  using VertexArray = QuadCommon::VertexArray;

  ShaderProgram &prog;
  std::list<ShaderAttrib> a_position;
  VertexArray &vao;

  gl::Uniform<gl::UniformType::INTEGER> u_highlight;
  gl::Uniform<gl::UniformType::VEC3> u_color;
  gl::Uniform<gl::UniformType::MAT4> u_transform;

  Box box;
  QuadCommon &quadcommon;
  Camera &cam;

public:
  Facing(Camera &cam,
    const std::array<Face, 3> &face_types,
    float scale,
    glm::vec3 position,
    QuadCommon &quadcommon,
    BoxCommon &boxcommon
  ):
    face_types(face_types),
    quadcommon(quadcommon),
    prog(quadcommon.program),
    vao(quadcommon.vao),
    u_highlight("highlight"),
    u_color("color"),
    u_transform("transform"),
    box(boxcommon, cam, glm::vec3(0, 0, 0)),
    cam(cam)
  {
    transform.SetScale(scale);
    transform.SetPosition(position);
    box.transform = transform;
    /* transform.SetRotation(1, 0, 0, 180.f); */
  }

  bool has_face(Face f) {
    return face_types[0] == f || face_types[1] == f || face_types[2] == f;
  }

  template <typename F>
  void iterate_face_attributes(F &&func) {
    if(has_face(Face::X_FRONT))func(quadcommon.vpos_xf);
    if(has_face(Face::X_BACK)) func(quadcommon.vpos_xb);
    if(has_face(Face::Y_FRONT))func(quadcommon.vpos_yf);
    if(has_face(Face::Y_BACK)) func(quadcommon.vpos_yb);
    if(has_face(Face::Z_FRONT))func(quadcommon.vpos_zf);
    if(has_face(Face::Z_BACK)) func(quadcommon.vpos_zb);
  }

  int faceCount = -1;
  int no_faces() {
    if(faceCount!=1)return faceCount;
    int q = 0;
    for(auto &f : face_types) {
      if(f != Face::NFACE) {
        ++q;
      }
    }
    faceCount = q;
    return faceCount;
  }


  void init() {
    box.init();
    iterate_face_attributes([&](auto attr) mutable -> void {
      /* Logger::Info("  face\n"); */
      colors.push_back(glm::vec3(1,1,1));

      a_position.push_back(attr);
    });
  }

  void update_uniforms(int id) {
    auto programid = prog.id();
    if(transform.has_changed || cubeTransform.has_changed || cam.has_changed) {
      u_transform.set_id(programid);
      u_transform.set_data(matrix);
    }
    u_highlight.set_id(programid);
    u_highlight.set_data(highlight);
    u_color.set_id(programid);
    u_color.set_data(get(colors, id));
  }

public:
  float depth = 0.5;
  void update_depth() {
    depth = (matrix * glm::vec4(.5, .5, .5, 1.)).x;
  }

  std::string str() {
    std::vector<char> buf(1000);
    sprintf(buf.data(), "facing (%s %s %s)",
      face_to_string(face_types[0]).c_str(),
      face_to_string(face_types[1]).c_str(),
      face_to_string(face_types[2]).c_str()
      );
    return std::string(buf.begin(), buf.end());
  }

private:
  int face_to_index(Face &f) {
    int q = 0;
    for(auto &ft : face_types) {
      if(ft == f) {
        return q;
      } else if(ft != Face::NFACE) {
        ++q;
      }
    }
    return -1;
  }

public:
  void update_face_types(Rotation rot, Face axis_face) {
    const Face
      a = map_face_rotation(face_types[0], rot, axis_face),
      b = map_face_rotation(face_types[1], rot, axis_face),
      c = map_face_rotation(face_types[2], rot, axis_face);
    face_types = {Face::NFACE, Face::NFACE, Face::NFACE};
    for(const auto &new_f : {a, b, c}){
      switch(new_f) {
        case Face::X_FRONT: case Face::X_BACK: face_types[0] = new_f; break;
        case Face::Y_FRONT: case Face::Y_BACK: face_types[1] = new_f; break;
        case Face::Z_FRONT: case Face::Z_BACK: face_types[2] = new_f; break;
        case Face::NFACE: break;
      }
    }
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

  void update_transforms() {
    matrix = cam.get_matrix() * cubeTransform.get_matrix() * transform.get_matrix();
  }

  void draw() {
    update_transforms();
    update_depth();
    Transformation boxCubeTransform = cubeTransform;
    boxCubeTransform.Scale(.98);
    box.draw(boxCubeTransform);

    ShaderProgram::use(prog);
    for(int i = 0; i < a_position.size(); ++i) {
      update_uniforms(i);

      auto &current_vpos = get(a_position, i);
      int current_vao_attrib_position = 0;

      vao.enable_d(current_vao_attrib_position);
      vao.set_access(current_vpos, 0, current_vao_attrib_position);
      vao.bind();
      current_vpos.bind();
      VertexArray::draw<GL_TRIANGLES>(vao);
      vao.disable_d(current_vao_attrib_position);
    }
    ShaderProgram::unuse();
    transform.has_changed = false;
    cubeTransform.has_changed = false;
  }

  void clear() {
    for(int i = 0; i < no_faces(); ++i) {
      ShaderAttrib::clear(get(a_position, i));
    }
    box.clear();
  }
};
