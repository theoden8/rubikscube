#include "Sprite.hpp"

#include "Colors.hpp"

std::array <ColorBuffer, 6> Sprite::colorbufs = {
    color::make_buffer(color::red), color::make_buffer(color::yellow),
    color::make_buffer(color::green), color::make_buffer(color::teal),
    color::make_buffer(color::blue), color::make_buffer(color::purple),
};

Sprite::Sprite()
{}

Sprite::~Sprite()
{}

void Sprite::init() {
  for(auto &c : colorbufs)
    c.init();
}

void Sprite::clear() {
  for(auto &c : colorbufs)
    c.clear();
}
