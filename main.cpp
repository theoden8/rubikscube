#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL

#include <Window.hpp>

int main(int argc, char *argv[]) {
  srand(time(nullptr));
  std::string dir = "";
  Window w(960, 960, dir);
  w.run();
}
