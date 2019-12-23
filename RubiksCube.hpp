#pragma once

#include <array>

#include <Logger.hpp>
#include <Debug.hpp>

#include <Quad.hpp>
#include <Facing.hpp>
#include <Rubiks.hpp>

struct RubiksCube {
  static constexpr size_t N = 3;
  static constexpr glm::vec3
    WHITE = glm::vec3(1, 1, 1),
    RED = glm::vec3(1, .5, .5),
    GREEN = glm::vec3(.48, .898, .572),
    BLUE = glm::vec3(.63, .66, 1),
    YELLOW = glm::vec3(1, .8, .236),
    PINK = glm::vec3(1, .2, .5),
    CYAN = glm::vec3(.2, 1, 1),
    GRAY = glm::vec3(.4, .4, .4),
    BLACK = glm::vec3(0, 0, 0);

  using RubiksT = Rubiks<N>;

  Camera &cam;

  Quad quad;
  std::vector<Facing> facings;

  RubiksCube(Camera &cam):
    cam(cam),
    quad("vposition"s)
  {
    facings.reserve(RubiksT::number_of_boxes);
    const float step = .5 / N;
    const float scale = 0.95 * step;

    for(int i = 0; i < N; ++i) {
      bool i_edge = (i == 0 || i == N - 1);
      for(int j = 0; j < N; ++j) {
        bool j_edge = (j == 0 || j == N - 1);
        for(int k = 0; k < N; ++k) {
          bool k_edge = (k == 0 || k == N - 1);
          if(!i_edge && !j_edge && !k_edge)continue;

          std::array<Face, 3> face_types = {Face::NFACE, Face::NFACE, Face::NFACE};

          if(i_edge)face_types[0] = (i == 0) ? Face::X_FRONT : Face::X_BACK;
          if(j_edge)face_types[1] = (j == 0) ? Face::Y_FRONT : Face::Y_BACK;
          if(k_edge)face_types[2] = (k == 0) ? Face::Z_FRONT : Face::Z_BACK;

          glm::vec3 position = glm::vec3(float(i), float(j), float(k)) / float(N) - .25f;
          facings.push_back(Facing(cam, face_types, scale, position));
        }
      }
    }
    /* Logger::Info("facing sizes: %lu\n", facings.size()); */
    /* Logger::Info("expected: %lu\n", RubiksT::number_of_boxes); */
    ASSERT(facings.size() == RubiksT::number_of_boxes);
  }

  template <typename F>
  void select_face(Face face, F &&func) {
    if(face == Face::NFACE)return;
    for(auto &f : facings) {
      f.select_face(face, std::forward<F>(func));
    }
  }

  void set_highlight(Face face) {
    select_face(face, [&](glm::vec3 &c, Transformation &t, int &h) mutable -> void {
      h = 1;
    });
  }

  void unset_highlight(Face face) {
    select_face(face, [&](glm::vec3 &c, Transformation &t, int &h) mutable -> void {
      h = 0;
    });
  }

  void set_color(Face face, glm::vec3 color) {
    select_face(face, [&](glm::vec3 &c, Transformation &t, int &h) mutable -> void {
      c = color;
    });
  }

  Face lastActiveFace = Face::NFACE;
  Face activeFace = Face::Z_BACK;
  void init() {
    quad.init();
    for(auto &f : facings) {
      Logger::Info("%s\n", f.str().c_str());
      f.init(quad);
    }
    set_color(Face::X_FRONT, WHITE);
    set_color(Face::X_BACK,  YELLOW);
    set_color(Face::Y_FRONT, PINK);
    set_color(Face::Y_BACK,  RED);
    set_color(Face::Z_FRONT, BLUE);
    set_color(Face::Z_BACK,  GREEN);
  }

  void flip_active_face() {
    switch(activeFace) {
      case Face::X_FRONT:activeFace=Face::X_BACK;break;
      case Face::X_BACK:activeFace=Face::X_FRONT;break;
      case Face::Y_FRONT:activeFace=Face::Y_BACK;break;
      case Face::Y_BACK:activeFace=Face::Y_FRONT;break;
      case Face::Z_FRONT:activeFace=Face::Z_BACK;break;
      case Face::Z_BACK:activeFace=Face::Z_FRONT;break;
    }
  }

  void draw() {
    if(lastActiveFace != activeFace) {
      unset_highlight(lastActiveFace);
      lastActiveFace = activeFace;
      set_highlight(activeFace);
    }
    for(auto &f : facings) {
      f.draw();
    }
  }

  void clear() {
    for(auto &f : facings) {
      f.clear();
    }
    quad.clear();
  }
};
