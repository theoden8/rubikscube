#include "CubeFace.hpp"

#include "VertexBuffer.hpp"
#include "Log.hpp"
#include "Colors.hpp"

template <size_t N>
glm::vec3 CubeFace<N>::get_color(SIDE side) {
  static glm::vec3 palette[6] = {
    color::white, color::yellow, color::red,
    color::green, color::blue, color::teal
  };
  return palette[side];
}

template <size_t N>
CubeFace<N>::CubeFace(std::array <glm::vec3 *, 4> corners, SIDE side):
  colors(N * N, side),
  corners(corners)
{}

template <size_t N>
void CubeFace<N>::construct() {
  std::cout << "starting cubeface::cubeface" << std::endl;
  quads = (Quad *)malloc((N * N) * sizeof(Quad));
  glm::vec3
    botleft(*corners[0]);
  glm::vec3
    step_right = (*corners[1] - botleft) / (float)N,
    step_up = (*corners[3] - botleft) / (float)N;
  glm::vec3
    little(.05, .05, .05),
    xlittle = glm::vec3(1., 1., 1.) - little;
  for(size_t y = 0; y < N; ++y) {
    botleft = *corners[0] + step_up * (float)y;
    for(size_t x = 0; x < N; ++x) {
      quads[y * N + x] = Quad({
          botleft,
          botleft + step_right * xlittle,
          botleft + step_right*xlittle + step_up*xlittle,
          botleft + step_up*xlittle
        },
        get_color(colors[y * N + x])
      );
      botleft += step_right;
    }
  }
  std::cout << "finished initializing cubeface" << std::endl;
}

template <size_t N>
void CubeFace<N>::init() {
  for(size_t i = 0; i < N * N; ++i)
    quads[i].init();
}

template <size_t N>
void CubeFace<N>::draw() {
  for(size_t i = 0; i < N * N; ++i)
    quads[i].draw();
}

template <size_t N>
void CubeFace<N>::clear() {
  for(size_t i = 0; i < N * N; ++i)
    quads[i].clear();
  free(quads);
}

template <size_t N>
CubeFace<N>::~CubeFace()
{}

template class CubeFace <3>;
