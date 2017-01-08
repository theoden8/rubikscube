#pragma once

#include <vector>
#include <map>

#include <glm/glm.hpp>

#include "VertexBuffer.hpp"

namespace color {
  extern glm::vec3
    black, red, yellow, pink,
    green, blue, teal, white;
}

static std::map <glm::vec3, ColorBuffer> colorbufs;

ColorBuffer make_color_buffer(glm::vec3 &color, int no_colors = 3);
