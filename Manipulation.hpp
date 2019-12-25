#pragma once


#include <vector>
#include <queue>

#include <Logger.hpp>
#include <Debug.hpp>

#include <Face.hpp>


struct Manipulation {
  const Face face;
  const Rotation rot;
  float progress = 0.;

  /* inline Manipulation() */
  /* {} */

  inline Manipulation(Face f, Rotation r, float progress=0.):
    face(f), rot(r), progress(progress)
  {}

  static Manipulation make_random() {
    return Manipulation(get_random_face(), get_random_rotation());
  }

  inline bool is_complete() const {
    /* Logger::Info("progress: %.4f, %.4f\n", progress, std::abs(1.-progress)); */
    return std::abs(progress - 1.0) < 1e-5;
  }

  inline bool is_close() const {
    return std::abs(progress - 1.0) < 1e-1;
  }

  inline decltype(auto) inverse() const {
    return Manipulation(
      face,
      rot == Rotation::CLOCKWISE ?
        Rotation::COUNTERCLOCKWISE
        : Rotation::CLOCKWISE,
      1. - progress
    );
  }
};

class ManipulationPerformer {
  std::vector<Manipulation> timeline;
  std::queue<Manipulation> performQueue;
  int currentIndex = -1;
  float speed = 1.;

  template <typename RubiksCubeT>
  void rotate_face(RubiksCubeT &rb, const Manipulation &m, float deg) {
    static constexpr glm::vec3
      axis_z(0, 0, 1),
      axis_y(0, 1, 0),
      axis_x(1, 0, 0);

    glm::vec3 axis;
    float sign = 0.;

    const Face &f = m.face;
    const Rotation &r = m.rot;

    if((f == Face::Z_FRONT && r == Rotation::CLOCKWISE)
      || (f == Face::Z_BACK && r == Rotation::COUNTERCLOCKWISE))
    {
      axis = axis_z;
      sign = 1.;
    } else if((f == Face::Z_BACK && r == Rotation::CLOCKWISE)
      || (f == Face::Z_FRONT && r == Rotation::COUNTERCLOCKWISE))
    {
      axis = axis_z;
      sign = -1.;
    } else if((f == Face::Y_FRONT && r == Rotation::CLOCKWISE)
      || (f == Face::Y_BACK && r == Rotation::COUNTERCLOCKWISE))
    {
      axis = axis_y;
      sign = 1.;
    } else if((f == Face::Y_BACK && r == Rotation::CLOCKWISE)
      || (f == Face::Y_FRONT && r == Rotation::COUNTERCLOCKWISE))
    {
      axis = axis_y;
      sign = -1.;
    } else if((f == Face::X_FRONT && r == Rotation::CLOCKWISE)
      || (f == Face::X_BACK && r == Rotation::COUNTERCLOCKWISE))
    {
      axis = axis_x;
      sign = 1.;
    } else if((f == Face::X_BACK && r == Rotation::CLOCKWISE)
      || (f == Face::X_FRONT && r == Rotation::COUNTERCLOCKWISE))
    {
      axis = axis_x;
      sign = -1.;
    }

    rb.select_face(f, [&](glm::vec3 &c, Transformation &t, int &h) mutable -> void {
      t.Rotate(axis, sign * deg);
    });
  }

public:
  ManipulationPerformer()
  {}

  template <typename RubiksCubeT>
  void rotate_face(RubiksCubeT &rb, Manipulation &m) {
    if(m.is_complete())return;

    float deg = std::sin((0.5 + m.progress * .5) * M_PI) * 90. * .01 * speed * std::sqrt(performQueue.size());
    if(m.is_close()) {
      deg = (1. - m.progress) * 90.;
    }
    rotate_face(rb, m, deg);

    m.progress = (m.progress * 90. + deg) / 90.;
  }

  void set_new(Manipulation m) {
    ++currentIndex;
    if(currentIndex == timeline.size()) {
      timeline.push_back(m);
      performQueue.push(m);
    } else {
      while(currentIndex < timeline.size()) {
        performQueue.push(timeline.back().inverse());
        performQueue.pop();
      }
      timeline.push_back(m);
      performQueue.push(m);
    }
  }

  void take_back() {
    if(currentIndex == -1)return;
    Manipulation m = timeline[currentIndex--].inverse();
    m.progress = 0.;
    performQueue.push(m);
  }

  int count() const {
    return currentIndex + 1;
  }

  template <typename RubiksCubeT>
  void perform_step(RubiksCubeT &rb) {
    if(performQueue.empty())return;
    while(performQueue.front().is_complete()) {
      rb.update_face_types(performQueue.front());
      performQueue.pop();
      if(performQueue.empty())return;
    }
    rotate_face(rb, performQueue.front());
  }
};
