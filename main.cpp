// Include standard headers
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.hpp"

int main(int argc, char *argv[]) {
  Window w(960, 960);
  w.run();
}
