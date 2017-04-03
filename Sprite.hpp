#pragma once

#include <array>

#include "VertexBuffer.hpp"

class Sprite {
  static std::array <ColorBuffer, 6> colorbufs;
public:
  Sprite();
  ~Sprite();
  static void
    init(),
    clear();
};
