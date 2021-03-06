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
  using ShaderBuffer = QuadCommon::ShaderBuffer;
  using ShaderAttrib = QuadCommon::ShaderAttrib;
  using VertexArray = QuadCommon::VertexArray;

  ShaderProgram &prog;
  ShaderAttrib &a_position;
  VertexArray &vao;
  std::vector<ShaderBuffer *> face_buffers;

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
    a_position(quadcommon.a_position),
    vao(quadcommon.vao),
    u_highlight("highlight"),
    u_color("color"),
    u_transform("transform"),
    box(boxcommon, cam, glm::vec3(0.1, 0.1, 0.1)),
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
  void iterate_face_buffers(F &&func) {
    int i = 0;
    if(has_face(Face::X_FRONT))func(quadcommon.vpos_xf, i++);
    if(has_face(Face::X_BACK)) func(quadcommon.vpos_xb, i++);
    if(has_face(Face::Y_FRONT))func(quadcommon.vpos_yf, i++);
    if(has_face(Face::Y_BACK)) func(quadcommon.vpos_yb, i++);
    if(has_face(Face::Z_FRONT))func(quadcommon.vpos_zf, i++);
    if(has_face(Face::Z_BACK)) func(quadcommon.vpos_zb, i++);
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
    iterate_face_buffers([&](auto &buf, int i) mutable -> void {
      /* Logger::Info("  face\n"); */
      colors.push_back(glm::vec3(1,1,1));
      face_buffers.push_back(&buf);
    });
    u_transform.set_id(prog.id());
    u_highlight.set_id(prog.id());
    u_color.set_id(prog.id());
  }

  void update_uniforms(int id) {
    if(transform.has_changed || cubeTransform.has_changed || cam.has_changed) {
      u_transform.set_data(matrix);
    }
    u_highlight.set_data(highlight);
    u_color.set_data(get(colors, id));
  }

public:
  float depth = 0.5;
  void update_depth() {
    depth = (matrix * glm::vec4(.5, .5, .5, 1.)).z;
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
  int face_to_index(const Face &f) {
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
  void select_face(const Face &f, F &&func) {
    if(has_face(f)) {
      int id = face_to_index(f);
      func(*this, f);
    }
  }

  int getHighlight() const {
    return highlight;
  }

  void setHighlight(int h) {
    if(h == 0) {
      box.color = glm::vec3(.1, .1, .1);
    } else if(h == 1) {
      box.color = glm::vec3(.8, .8, .8);
    } else if(h == 2) {
      box.color = glm::vec3(.8, .8, .8);
    }
    highlight = h;
  }

  Transformation &getCubeTransform() {
    return cubeTransform;
  }

  void setColor(const Face f, const glm::vec3 &color) {
    get(colors, face_to_index(f)) = color;
  }

  void update_transforms() {
    if(cam.has_changed || cubeTransform.has_changed || transform.has_changed) {
      matrix = cam.get_matrix() * cubeTransform.get_matrix() * transform.get_matrix();
    }
  }

  void draw() {
    update_transforms();
    update_depth();

    Transformation boxCubeTransform = cubeTransform;
    boxCubeTransform.Scale(.98);
    /* box.draw(boxCubeTransform); */

    ShaderProgram::use(prog);
    for(int i = 0; i < face_buffers.size(); ++i) {
      auto &buf = *face_buffers[i];

      a_position.select_buffer(buf);

      VertexArray::bind(vao);

      vao.set_access(a_position);
      vao.enable(a_position);
      vao.bind();
      a_position.bind();

      update_uniforms(i);

      VertexArray::draw<GL_TRIANGLES>(vao);

      vao.disable(a_position);
      VertexArray::unbind();

      a_position.deselect_buffer();
    };
    ShaderProgram::unuse();
    transform.has_changed = cubeTransform.has_changed = false;
  }

  void clear() {
    box.clear();
  }
};
