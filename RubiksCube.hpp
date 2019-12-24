#pragma once

#include <array>

#include <Logger.hpp>
#include <Debug.hpp>

#include <Quad.hpp>
#include <Box.hpp>
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
    GRAY = glm::vec3(.8, .8, .8),
    BLACK = glm::vec3(0, 0, 0);

  using RubiksT = Rubiks<N>;

  Camera &cam;

  Quad quad;
  Box insideBox;
  std::vector<Facing> facings;

  RubiksCube(Camera &cam):
    cam(cam),
    insideBox(cam, GRAY, .5),
    quad("vposition"s)
  {
    facings.reserve(RubiksT::number_of_boxes);
    const float totalsize = .7;
    const float step = totalsize / (N - 1);
    const float scale = 0.48 * step;

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

          glm::vec3 position = glm::vec3(step * i, step * j, step * k) - totalsize / 2;
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

  void set_highlight(Face face, int value=1) {
    select_face(face, [&](glm::vec3 &c, Transformation &t, int &h) mutable -> void {
      h = value;
    });
  }

  void unset_highlight(Face face) {
    set_highlight(face, 0);
  }

  void set_secondary_highlight(Face face, int value) {
    if(face == Face::NFACE)return;
    select_face(face, [&](glm::vec3 &c, Transformation &t, int &h) mutable -> void {
      if(h != 1) {
        h = value;
      }
    });
  }

  void unset_secondary_highlight(Face face) {
    set_secondary_highlight(face, 0);
  }

  void set_color(Face face, glm::vec3 color) {
    select_face(face, [&](glm::vec3 &c, Transformation &t, int &h) mutable -> void {
      c = color;
    });
  }

  int activeFaceId = -1;
  std::vector<Face> activeFaceTimeline;
  Face lastActiveFace = Face::NFACE;
  Face lastNearest = Face::NFACE;
  Face nearestFace = Face::NFACE;
  void init() {
    quad.init();
    insideBox.init();
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

    set_new_active_face(Face::X_FRONT);
  }

  Face activeFace() {
    return activeFaceTimeline[activeFaceId];
  }

  void set_new_active_face(Face f) {
    if(activeFaceId >= 0) {
      activeFaceTimeline.resize(activeFaceId + 1);
    }
    activeFaceTimeline.push_back(f);
    set_next_active_face();
  }

  void flip_active_face() {
    Face f;
    switch(activeFace()) {
      case Face::X_FRONT:f=Face::X_BACK;break;
      case Face::X_BACK:f=Face::X_FRONT;break;
      case Face::Y_FRONT:f=Face::Y_BACK;break;
      case Face::Y_BACK:f=Face::Y_FRONT;break;
      case Face::Z_FRONT:f=Face::Z_BACK;break;
      case Face::Z_BACK:f=Face::Z_FRONT;break;
    }
    set_new_active_face(f);
  }

  void swap_selected_faces() {
    set_new_active_face(nearestFace);
  }

  void set_previous_active_face() {
    if(activeFaceId > 0) {
      --activeFaceId;
    }
  }

  void set_next_active_face() {
    if(activeFaceId != activeFaceTimeline.size() - 1) {
      ++activeFaceId;
    }
  }

  Face find_nearest_face() {
    Face nearest = Face::NFACE;
    float mindepth = 1e9;
    for(auto &f : facings) {
      if(f.no_faces() > 1)continue;
      if(f.depth < mindepth) {
        for(auto &face : f.face_types)if(face!=Face::NFACE){
          if(!is_opposite_face(face, activeFace()) && !(face == activeFace())) {
            nearest = face, mindepth = f.depth;
          }
          break;
        }
      }
    }
    return nearest;
  }

  bool changed_selection = false;
  void set_selection() {
    if(lastActiveFace != activeFace()) {
      unset_highlight(lastActiveFace);
      lastActiveFace = activeFace();
      set_highlight(activeFace());
      changed_selection = true;
    }
  }

  void set_nearest() {
    nearestFace = find_nearest_face();
    ASSERT(nearestFace != Face::NFACE);
    if(nearestFace != lastNearest || changed_selection) {
      unset_secondary_highlight(lastNearest);
      set_secondary_highlight(nearestFace, 2);
      lastNearest = nearestFace;
      changed_selection = false;
    }
  }

  void draw() {
    set_selection();
    set_nearest();
    insideBox.draw();
    for(auto &f : facings) {
      f.draw();
    }
  }

  void clear() {
    insideBox.clear();
    for(auto &f : facings) {
      f.clear();
    }
    quad.clear();
  }
};
