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
CubeFace<N>::CubeFace(std::array <glm::vec3, 4> corners, SIDE side):
  colors(N * N, side), quads(NULL)
{
  quads = (Quad *)malloc((N * N) * sizeof(Quad));
  glm::vec3 botleft(corners[0]);
  glm::vec3 botright(corners[1]);
  glm::vec3 topleft(corners[3]);
  glm::vec3 step_right = (botright - botleft) / (float)N;
  glm::vec3 step_up = (topleft - botleft) / (float)N;
  glm::vec3 little(.05, .05, .05);
  for(size_t y = 0; y < N; ++y) {
    for(size_t x = 0; x < N; ++x) {
      glm::vec3 new_botleft = botleft + step_right * (float)x + step_up * (float)y;
      quads[y * N + x] = Quad({
          new_botleft,
          new_botleft + step_right - little * step_right,
          new_botleft + step_right + step_up - little * step_up - little * step_right,
          new_botleft + step_up - little * step_up,
        },
        get_color(colors[y * N + x])
      );
    }
  }
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
