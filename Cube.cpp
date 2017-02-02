#include "Cube.hpp"

#include <array>

#include "Log.hpp"

template <size_t N>
Cube<N>::Cube(glm::vec3 center, float h):
  center(center), sides(NULL)
{
  sides = (cbface_t *)malloc(sizeof(cbface_t) * (size_t)CB_NO_SIDES);
  sides = (cbface_t *)malloc(1e6);
  assert(sides != NULL);
  ASSERT(h < 1.);
  glm::vec3 corners[8] = {
    glm::vec3( h, h, h),
    glm::vec3(-h, h, h),
    glm::vec3( h,-h, h),
    glm::vec3(-h,-h, h),

    glm::vec3( h, h,-h),
    glm::vec3(-h, h,-h),
    glm::vec3( h,-h,-h),
    glm::vec3(-h,-h,-h),
  };
  #define C(a, b, c, d) {&corners[a], &corners[b], &corners[c], &corners[d]}
  sides[CB_FRONT] =  cbface_t(C(0, 2, 3, 1), CB_FRONT);
  sides[CB_BACK] =   cbface_t(C(5, 7, 6, 4), CB_BACK);
  sides[CB_LEFT] =   cbface_t(C(1, 3, 7, 5), CB_LEFT);
  sides[CB_RIGHT] =  cbface_t(C(4, 6, 2, 0), CB_RIGHT);
  sides[CB_TOP] =    cbface_t(C(0, 1, 5, 4), CB_TOP);
  sides[CB_BOTTOM] = cbface_t(C(6, 7, 3, 2), CB_BOTTOM);
  #undef C
  for(int s = 0; s < CB_NO_SIDES; ++s)
    sides[s].construct();
}

template <size_t N>
void Cube<N>::rotate(CB_SIDE side, CB_CW_CCW direction) {
  for(size_t i = 0; i < N; ++i) {
    for(size_t j = 0; j < N; ++j) {
      sides[side].quads[i * N + j].change_color(color::black);
    }
  }
}

template <size_t N>
void Cube<N>::init() {
  for(size_t i = 0; i < CB_NO_SIDES; ++i)
    sides[i].init();
}

template <size_t N>
void Cube<N>::draw() {
  for(size_t i = 0; i < CB_NO_SIDES; ++i)
    sides[i].draw();
}

template <size_t N>
void Cube<N>::clear() {
  for(size_t i = 0; i < CB_NO_SIDES; ++i)
    sides[i].clear();
  free(sides);
}

template <size_t N>
Cube<N>::~Cube()
{}

template class Cube <3>;
