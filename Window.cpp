#include <cassert>
#include <cstdlib>
#include <unistd.h>
#include <string>

#include "Window.hpp"
#include "VertexBuffer.hpp"
#include "Triangle.hpp"
#include "Log.hpp"
#include "Colors.hpp"

#include <glm/glm.hpp>

void Window::start() {
  init_glfw();
  init_glew();
  /* init_controls(); */
  /* log_gl_params(); */
}

void Window::init_glfw() {
  assert(restart_gl_log());
  glfwSetErrorCallback(glfw_error_callback);
  ASSERT(glfwInit() == 1);

  /* glfwWindowHint(GLFW_SAMPLES, 4); */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
  /* GL_ERRORS */
  // open a window and create its OpenGL context
  win_ = glfwCreateWindow(width, height, "Rubiks Cube", NULL, NULL);
  assert(win_ != NULL);
  glfwMakeContextCurrent(win_);
  GLERROR
}

void Window::init_glew() {
  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  ASSERT(glewInit() == GLEW_OK);
  GLERROR
}

void Window::init_controls() {
  // ensure we can capture the escape key
  glfwSetInputMode(win_, GLFW_STICKY_KEYS, GL_TRUE); GLERROR
  glfwSetInputMode(win_, GLFW_CURSOR, GLFW_CURSOR_DISABLED); GLERROR
}

Window::Window(size_t width, size_t height):
  width(width), height(height),
  shader_program("shader.vert", "shader.frag"),
  quadbuf(
    PositionBuffer({
      glm::vec3(0.5f,  0.5f, 0.0f),
      glm::vec3(0.5f, -0.5f, 0.0f),
      glm::vec3(-0.5f, -0.5f, 0.0f),
      glm::vec3(-0.5f, 0.5f, 0.0f),
    }),
    color::red
  )
{
  start();
}

void Window::gl_version() {
  // get version info
  const GLubyte* renderer = glGetString(GL_RENDERER); GLERROR // get renderer string
  const GLubyte* version = glGetString(GL_VERSION); GLERROR // version as a string
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported %s\n", version);
}

void Window::display() {
  /* std::cout << "display" << std::endl; */
  /* sleep(1); */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERROR
  shader_program.use(); GLERROR
  quadbuf.draw(); GLERROR
  glfwPollEvents(); GLERROR
  glfwSwapBuffers(win_); GLERROR
  if(glfwGetKey(win_, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(win_, 1); GLERROR
  }
}

void Window::idle() {
  glEnable(GL_DEPTH_TEST); GLERROR
  glDepthFunc(GL_LESS); GLERROR
  quadbuf.init();
  shader_program.compile();
  shader_program.bind({"vposition", "vcolor"});
  shader_program.is_valid();
  glEnable(GL_CULL_FACE); GLERROR // cull face
  glCullFace(GL_BACK); GLERROR // cull back face
  glFrontFace(GL_CW); GLERROR // GL_CCW for counter clock-wise
  while(!glfwWindowShouldClose(win_)) {
    display();
  }
}

Window::~Window() {
  quadbuf.clear(); GLERROR
  shader_program.clear(); GLERROR
  glfwTerminate(); GLERROR
}