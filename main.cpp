// Include standard headers
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.hpp"

int main(int argc, char *argv[]) {
  Window w(960, 960);
  w.gl_version();
  w.idle();
}
