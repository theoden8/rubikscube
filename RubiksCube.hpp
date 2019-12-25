#pragma once

#include <cmath>

#include <array>
#include <map>

#include <Logger.hpp>
#include <Debug.hpp>

#include <QuadCommon.hpp>
#include <BoxCommon.hpp>
#include <Facing.hpp>
#include <Manipulation.hpp>
#include <Rubiks.hpp>

template <size_t N = 3>
struct RubiksCube {
  static constexpr glm::vec3
    WHITE = glm::vec3(1, 1, 1),
    RED = glm::vec3(1, .5, .5),
    GREEN = glm::vec3(.48, .898, .572),
    BLUE = glm::vec3(.63, .66, 1),
    YELLOW = glm::vec3(.98, .92, .365),
    ORANGE = glm::vec3(1, .6, .2),
    CYAN = glm::vec3(.2, 1, 1),
    GRAY = glm::vec3(.8, .8, .8),
    BLACK = glm::vec3(0, 0, 0);

  using RubiksT = Rubiks<N>;

  Camera &cam;

  QuadCommon quadcommon;
  BoxCommon boxcommon;
  std::vector<Facing> facings;

  ManipulationPerformer manip;
  std::queue<Manipulation> manipulationQueue;
  std::vector<Manipulation> manipulations;
  int currentManipulation = -1;

  RubiksCube(Camera &cam, const std::string &dir):
    cam(cam),
    quadcommon("vposition"s, dir),
    boxcommon("vposition"s, dir)
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
          facings.push_back(Facing(cam, face_types, scale, position, quadcommon, boxcommon));
        }
      }
    }
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
    quadcommon.init();
    boxcommon.init();

    for(auto &f : facings) {
      Logger::Info("%s\n", f.str().c_str());
      f.init();
    }
    quadcommon.finish_init();

    set_color(Face::X_FRONT, WHITE);
    set_color(Face::X_BACK,  YELLOW);
    set_color(Face::Y_FRONT, ORANGE);
    set_color(Face::Y_BACK,  RED);
    set_color(Face::Z_FRONT, BLUE);
    set_color(Face::Z_BACK,  GREEN);

    set_new_active_face(Face::X_BACK);
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
    Face f = Face::NFACE;
    switch(activeFace()) {
      case Face::X_FRONT:f=Face::X_BACK;break;
      case Face::X_BACK:f=Face::X_FRONT;break;
      case Face::Y_FRONT:f=Face::Y_BACK;break;
      case Face::Y_BACK:f=Face::Y_FRONT;break;
      case Face::Z_FRONT:f=Face::Z_BACK;break;
      case Face::Z_BACK:f=Face::Z_FRONT;break;
      case Face::NFACE:break;
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

  inline void set_manipulation_cw() {
    manip.set_new(Manipulation(activeFace(), Rotation::CLOCKWISE));
  }

  inline void set_manipulation_ccw() {
    manip.set_new(Manipulation(activeFace(), Rotation::COUNTERCLOCKWISE));
  }

  inline void update_face_types(const Manipulation &m) {
    /* Logger::Info("%s   %s\n", face_to_string(m.face).c_str(), rotation_to_string(m.rot).c_str()); */
    std::map<Face, int> counts, newcounts;
    for(auto &f : facings) {
      if(f.has_face(m.face)) {
        for(const auto ft : f.face_types)++counts[ft];
        f.update_face_types(m.rot, m.face);
        for(const auto ft : f.face_types)++newcounts[ft];
      }
    }

    for(Face face : {
      Face::X_FRONT, Face::X_BACK,
      Face::Y_FRONT, Face::Y_BACK,
      Face::Z_FRONT, Face::Z_BACK,
      Face::NFACE
    })
    {
      if(counts[face] != newcounts[face]) {
        std::vector<char> buf(1000);
        sprintf(buf.data(), "different numbers of faces: %s - [%d] vs [%d]\n", face_to_string(face).c_str(), counts[face], newcounts[face]);
        TERMINATE(std::string(buf.begin(), buf.end()).c_str());
      }
    }
    ASSERT(counts[m.face] == N * N);
  }

  void shuffle(int no_steps=40) {
    for(int i = 0; i < no_steps; ++i) {
      manip.set_new(Manipulation::make_random());
    }
  }

  void solve() {
    while(manip.count()) {
      manip.take_back();
    }
  }

  void draw() {
    manip.perform_step(*this);
    set_selection();
    set_nearest();
    for(auto &f : facings) {
      f.draw();
    }
    /* cam.has_changed = false; */
  }

  void clear() {
    for(auto &f : facings) {
      f.clear();
    }
    quadcommon.clear();
    boxcommon.clear();
  }
};
