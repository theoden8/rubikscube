#pragma once


#include <Debug.hpp>
#include <Logger.hpp>


template <size_t N = 3>
struct Rubiks {
  static constexpr size_t no_faces = 6;

  static constexpr size_t orbit_size = no_faces;
  static constexpr size_t number_of_orbits = N * N * no_faces;
  static constexpr size_t number_of_boxes = N * N * N - (N - 2) * (N - 2) * (N - 2);
};
