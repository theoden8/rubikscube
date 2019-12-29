#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL

#include <Window.hpp>

#if !defined(_POSIX_VERSION)
#include <direct.h>
#endif

int main(int argc, char *argv[]) {
  srand(time(nullptr));
  Logger::Setup(("rubik.log"));
#if defined(_POSIX_VERSION)
  char buf[PATH_MAX + 1];
  getcwd(buf, PATH_MAX);
  Logger::Info("cwd: '%s'\n", buf);
#else
  char buf[MAX_PATH + 1];
  _getcwd(buf, MAX_PATH);
  Logger::Info("cwd: '%s'\n", buf);
#endif
  const std::string dir = sys::get_executable_directory(argc, argv);
  Logger::Info("dir '%s'\n", dir.c_str());
  Window w(960, 960, dir);
  w.run();
  Logger::Close();
}
