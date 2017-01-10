#include "Cube.hpp"

#include <array>

#include "Log.hpp"

template <size_t N>
Cube<N>::Cube(glm::vec3 center, float h):
  center(center), sides(NULL)
{
  sides = (cb_face_t *)malloc(CB_NO_SIDES * sizeof(cb_face_t));
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
  #define C(a, b, c, d) {corners[a], corners[b], corners[c], corners[d]}
  std::cout << "init" << std::endl;
  sides[CB_FRONT] =  cb_face_t(C(0, 2, 3, 1), CB_FRONT);
  std::cout << "front" << std::endl;
  sides[CB_BACK] =   cb_face_t(C(4, 6, 7, 5), CB_BACK);
  std::cout << "back" << std::endl;
  sides[CB_LEFT] =   cb_face_t(C(1, 3, 7, 5), CB_LEFT);
  std::cout << "left" << std::endl;
  sides[CB_RIGHT] =  cb_face_t(C(0, 2, 6, 4), CB_RIGHT);
  std::cout << "right" << std::endl;
  sides[CB_TOP] =    cb_face_t(C(0, 1, 5, 4), CB_TOP);
  std::cout << "top" << std::endl;
  sides[CB_BOTTOM] = cb_face_t(C(0, 2, 6, 4), CB_BOTTOM);
  std::cout << "bottom" << std::endl;
  #undef C
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
