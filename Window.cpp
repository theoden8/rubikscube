#include <cstdlib>
#include <unistd.h>
#include <string>

#include "Cube.hpp"
#include "Window.hpp"
#include "VertexBuffer.hpp"
#include "Log.hpp"
#include "Colors.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void Window::start() {
  init_glfw();
  init_glew();
  init_controls();
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
  // open a window and create its OpenGL context
  win_ = glfwCreateWindow(width, height, "Rubiks Cube", NULL, NULL);
  ASSERT(win_ != NULL);
  glfwMakeContextCurrent(win_); GLERROR
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
  cubebuffer(glm::vec3(0., 0., 0.), .5),
  rotation(0.f, 0.f, 0.f, 0.0f)
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

void Window::idle() {
  glEnable(GL_DEPTH_TEST); GLERROR
  glDepthFunc(GL_LESS); GLERROR
  cubebuffer.init();
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

void Window::display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERROR
  GLint u_rotation_vbo = glGetUniformLocation(shader_program, "rotation"); GLERROR
  shader_program.use(); GLERROR
  /* glUniform4fv(u_rotation_vbo, glm::value_ptr(rotation)); GLERROR */
  glUniform4f(u_rotation_vbo, rotation[0], rotation[1], rotation[2], rotation[3]); GLERROR
  static int iters = 3;
  /* glPolygonMode(GL_FRONT, GL_FILL); GLERROR */
  /* glPolygonMode(GL_BACK, GL_LINE); GLERROR */
  cubebuffer.draw(); GLERROR
  glfwPollEvents(); GLERROR
  glfwSwapBuffers(win_); GLERROR
  keyboard();
}

void Window::keyboard() {
  if(glfwGetKey(win_, GLFW_KEY_ESCAPE)) {
    glfwSetWindowShouldClose(win_, 1); GLERROR
  } else if(glfwGetKey(win_, GLFW_KEY_UP)) {
    rotation[0] += 1.;
    rotation[3] += .25;
  } else if(glfwGetKey(win_, GLFW_KEY_DOWN)) {
    rotation[0] -= 1.;
    rotation[3] -= .25;
  } else if(glfwGetKey(win_, GLFW_KEY_LEFT)) {
    rotation[1] += 1.;
    rotation[3] += .25;
  } else if(glfwGetKey(win_, GLFW_KEY_RIGHT)) {
    rotation[1] -= 1.;
    rotation[3] -= .25;
  } else if(glfwGetKey(win_, GLFW_KEY_W)) {
    printf("w");
    cubebuffer.rotate(CB_FRONT, Cube<3>::CB_RT_CW);
  }
}

void Window::clear() {
  cubebuffer.clear(); GLERROR
  shader_program.clear(); GLERROR
  color::clear();
  glfwTerminate(); GLERROR
}

Window::~Window() {
  clear();
}
