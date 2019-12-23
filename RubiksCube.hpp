#pragma once

#include <array>

#include <Logger.hpp>
#include <Debug.hpp>

#include <Box.hpp>
#include <Facing.hpp>
#include <Rubiks.hpp>

struct RubiksCube {
  static constexpr size_t N = 3;
  static constexpr glm::vec3
    WHITE = glm::vec3(1, 1, 1),
    RED = glm::vec3(1, .2, .2),
    GREEN = glm::vec3(.2, 1, .2),
    BLUE = glm::vec3(.2, .2, 1),
    YELLOW = glm::vec3(1, 1, .2),
    PINK = glm::vec3(1, .2, .5),
    CYAN = glm::vec3(.2, 1, 1),
    GRAY = glm::vec3(.4, .4, .4),
    BLACK = glm::vec3(0, 0, 0);

  using RubiksT = Rubiks<N>;

  Transformation transform;

  Box box;
  std::vector<Facing> facings;

  Camera &cam;

  RubiksCube(Camera &cam):
    cam(cam),
    box(cam, GRAY, .45)
  {
    facings.reserve(RubiksT::number_of_boxes);
    const float step = .5 / N;
    const float scale = 0.48 * step;

    for(int i = 0; i < N; ++i) {
      bool i_edge = (i == 0 || i == N);
      for(int j = 0; j < N; ++j) {
        bool j_edge = (j == 0 || j == N);
        for(int k = 0; k < N; ++k) {
          bool k_edge = (k == 0 || k == N);
          if(i_edge || j_edge || k_edge) {
            std::array<Face, 3> face_types = {Face::NFACE, Face::NFACE, Face::NFACE};
            if(i_edge)face_types[0] = (i == 0) ? Face::X_FRONT : Face::X_BACK;
            if(j_edge)face_types[0] = (j == 0) ? Face::Y_FRONT : Face::Y_BACK;
            if(k_edge)face_types[0] = (k == 0) ? Face::Z_FRONT : Face::Z_BACK;
            glm::vec3 position = glm::vec3(float(i) / N, float(j) / N, float(k) / N);
            facings.push_back(Facing(cam, face_types, scale, position));
          }
        }
      }
    }
    Logger::Info("facing sizes: %lu\n", facings.size());
    Logger::Info("expected: %lu\n", RubiksT::number_of_boxes);
    /* ASSERT(facings.size() == RubiksT::number_of_boxes); */
  }

  template <typename F>
  void select_face(Face face, F &&func) {
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

  void init() {
    box.init();
    for(auto &f : facings) {
      Logger::Info("facing init\n");
      f.init();
    }
    set_highlight(Face::Z_BACK);
  }

  void draw() {
    box.draw();
    for(auto &f : facings) {
      f.draw();
    }
  }

  void clear() {
    box.clear();
    for(auto &f : facings) {
      f.clear();
    }
  }
};
