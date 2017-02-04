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
    white(1.0f, 1.0f, 1.0f),
    purple(0.5f, 0.0f, 0.5f),
    brown(0.7f, 0.5f, 0.0f),
    grey(0.3f, 0.3f, 0.3f);

  ColorBuffer make_buffer(glm::vec3 &color, int no_colors) {
    return ColorBuffer({color, color, color});
  }
}
