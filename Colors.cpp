#include "Colors.hpp"

namespace color {
  glm::vec3
    black(0.0f, 0.0f, 0.0f),
    red(1.0f, 0.0f, 0.0f),
    yellow(1.0f, 1.0f, 0.0f),
    pink(1.0f, 0.0f, 1.0f),
    green(0.0f, 1.0f, 0.0f),
    blue(0.0f, 0.0f, 1.0f),
    teal(0.0f, 1.0f, 1.0f),
    white(1.0f, 1.0f, 1.0f);
}

ColorBuffer make_color_buffer(glm::vec3 &color, int no_colors) {
  /* if(colorbufs.count(color) == 0) */
  /*   colorbufs[color] = ColorBuffer(std::vector<glm::vec3>(no_colors, color)); */
  /* return colorbufs[color]; */
  return ColorBuffer(std::vector<glm::vec3>(no_colors, color));
}
