#pragma once

#include <cstdio>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>

#if __unix__ || __linux__ || __APPLE__
#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>
#else
#include <io.h>
#include <windows.h>
#endif

namespace sys {

struct Path {
  const std::string p;
  inline explicit Path(const std::string &&p):
    p(p)
  {}

  inline explicit Path(const std::string &p):
    p(p)
  {}

#if defined _WIN32 || defined __CYGWIN__
  static constexpr char separator = '/';//'\\';
#else
  static constexpr char separator = '/';
#endif

  inline operator std::string() const noexcept {
    return p;
  }

  Path operator/(const Path &pp) const {
    std::string s = p;
    std::string ss = pp;
    if(s.length() && s.back() == separator) {
      return Path(s + ss);
    }
    return Path(s + separator + ss);
  }
};

std::string get_executable_directory(int argc, char *argv[]) {
#ifdef __linux__
  std::vector<char> buf(PATH_MAX);
  readlink("/proc/self/exe", buf.data(), buf.size());
  const std::string exec(buf.begin(), buf.end());
#elif !defined(_POSIX_VERSION)
  // windows
  std::vector<char> buf(MAX_PATH);
  GetModuleFileName(nullptr, buf.data(), MAX_PATH);
  const std::string exec(buf.begin(), buf.end());
#else
  std::string x = "";
  if(argv[0][0] == '/') {
    x = argv[0];
  }
  const std::string exec = x;
#endif
#if defined _WIN32 || defined __CYGWIN__
#define FILE_SEPARATOR '\\'
#else
#define FILE_SEPARATOR '/'
#endif
  std::string::size_type n = exec.rfind('/');
  std::string folder;
  if(n == std::string::npos) {
    folder = "./";
  } else {
    folder = exec.substr(0, n + 1);
  }
  return folder;
}


class File {
  std::string filename;

public:
  class Lock {
    int fd;
    bool dropped = true;
  public:
    Lock(FILE *file):
      #if defined(_POSIX_VERSION)
      fd(fileno(file))
      #else
      fd(0)
      #endif
    {
      #if defined(_POSIX_VERSION)
      assert(File::is_open(fd));
      if(flock(fd, LOCK_EX) < 0) {
        perror("flock[ex]:");
        abort();
      }
      #endif
      dropped = false;
    }

    void drop() noexcept {
      #if defined(_POSIX_VERSION)
      if(flock(fd, LOCK_UN) < 0) {
        perror("flock[un]:");
        abort();
      }
      #endif
      dropped = true;
    }

    ~Lock() {
      if(!dropped) {
        drop();
      }
    }
  };
public:
  File(const char *filename):
    filename(filename)
  {}

  size_t length() {
    /* struct stat st; */
    /* stat(filename.c_str(), &st); */
    /* return st.st_size; */
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
  }

  const std::string &name() const {
    return filename;
  }

  std::string name() {
    return filename;
  }

#if defined(_POSIX_VERSION)
  static bool is_open(FILE *file) {
    return sys::File::is_open(fileno(file));
  }

  static bool is_open(int fd) {
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
  }
#endif

  bool is_ext(const std::string &&ext) {
    if(ext.length() > filename.length())
      return false;
    size_t f=filename.length(),e=ext.length();
    return filename.substr(f-e, e) == ext;
  }

  bool exists() {
    /* return access(filename.c_str(), F_OK) != -1; */
    return bool(std::ifstream(filename));
  }

  static void truncate(const std::string &filename) {
    fclose(fopen(filename.c_str(), "w+"));
  }

  std::string load_text() {
    size_t size = length() + 1;
    std::string text;
    text.resize(size);

    FILE *file = fopen(filename.c_str(), "r");
    if(file == nullptr) {
      fprintf(stderr, "unable to open file '%s' for reading\n", filename.c_str());
      abort();
    }

    #if defined(_POSIX_VERSION)
    assert(sys::File::is_open(file));
    #endif
    sys::File::Lock fl(file);

    int i = 0;
    while((text[i] = fgetc(file)) != EOF)
      ++i;
    text[i] = '\0';

    fl.drop();

    fclose(file);
    return text;
  }

  inline operator Path() const noexcept {
    return Path(filename);
  }
};

} // namespace sys
