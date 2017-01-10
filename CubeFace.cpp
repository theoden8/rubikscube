#ifndef CUBEFACE_CPP_ZPIJBQXO
#define CUBEFACE_CPP_ZPIJBQXO

#include <array>
#include <vector>

#include "Quad.hpp"
#include "VertexBuffer.hpp"
#include "Log.hpp"
#include "Colors.hpp"

#include <glm/glm.hpp>

template <size_t N>
class CubeFace {
  std::array <int, N * N> colors;
  std::array <Quad, N * N> quads;
  const Quad corners;
public:
  static glm::vec3 &get_color(int color) {
    static glm::vec3 palette[6] = {
      color::black, color::yellow, color::red,
      color::green, color::blue, color::teal
    };
    return palette[color];
    /* return make_color_buffer(palette[color]); */
  }

  CubeFace(int color, Quad &corners):
    colors(color), quads(NULL), corners(corners)
  {}

  void init() {
    const float *b = corners.position.buffer;
    glm::vec3 botleft(b[0], b[1], b[2]);
    glm::vec3 botright(b[2], b[3], b[4]);
    glm::vec3 topleft(b[8], b[9], b[10]);
    glm::vec3 step_right = (botright - botleft) / (float)N;
    glm::vec3 step_up = (topleft - botleft) / (float)N;
    for(size_t y = 0; y < N; ++y) {
      for(size_t x = 0; x < N; ++x) {
        glm::vec3 new_botleft = botleft + step_right * (float)x + step_up * (float)y;
        quads[y * N + x] = Quad(
          PositionBuffer({
            new_botleft,
            new_botleft + step_right,
            new_botleft + step_right + step_up,
            new_botleft + step_up,
          }),
          get_color(y)
        );
      }
    }
  }
};

#endif
