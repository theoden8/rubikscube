#pragma once

#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include "VertexBuffer.hpp"

namespace color {
  extern glm::vec3
    black, red, yellow, pink,
    green, blue, teal, white,
    purple, brown, grey;

  ColorBuffer make_buffer(glm::vec3 &color, int no_colors = 3);
}
