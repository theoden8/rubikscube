#pragma once

#include <cstdlib>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <incgraphics.h>

#include <Logger.hpp>
#include <Debug.hpp>

#include <Window.hpp>
#include <Camera.hpp>
#include <RubiksCube.hpp>


using namespace std::literals::string_literals;

class Window {
  GLFWwindow *win_ = NULL;
  size_t width, height;

  void init_glfw() {
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
    Logger::Info("initialized glfw\n");
  }

  void init_controls() {
    // ensure we can capture the escape key
    glfwSetInputMode(win_, GLFW_STICKY_KEYS, GL_TRUE); GLERROR
    glfwSetInputMode(win_, GLFW_CURSOR, GLFW_CURSOR_DISABLED); GLERROR
  }
  void gl_version() {
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); GLERROR // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); GLERROR // version as a string
    Logger::Info("Renderer: %s\n", renderer);
    Logger::Info("OpenGL version supported %s\n", version);
  }
public:
  Window(size_t width, size_t height):
    width(width), height(height)
  {
    Logger::Setup("rubik.log");
    Logger::MirrorLog(stderr);
  }

  void run() {
    init_glfw();
    init_controls();
    gl_version();
    /* glEnable(GL_DEPTH_TEST); GLERROR */
    /* glDepthFunc(GL_LESS); GLERROR */


    /* glEnable(GL_CULL_FACE); GLERROR // cull face */
    /* glCullFace(GL_BACK); GLERROR // cull back face */
    /* glFrontFace(GL_CW); GLERROR // GL_CCW for counter clock-wise */

    Camera camera;
    RubiksCube rb(camera);
    rb.init();

    Logger::Info("init fin\n");
    while(!glfwWindowShouldClose(win_)) {
      glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); GLERROR

      /* glPolygonMode(GL_FRONT, GL_FILL); GLERROR */
      /* glPolygonMode(GL_BACK, GL_LINE); GLERROR */
      rb.draw();

      glfwPollEvents(); GLERROR
      glfwSwapBuffers(win_); GLERROR
      keyboard(camera);
    }

    rb.clear();

    glfwDestroyWindow(win_); GLERROR
    glfwTerminate(); GLERROR

  }

  void keyboard(Camera &camera) {
    const float deg = 1.5;
    if(glfwGetKey(win_, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(win_, 1); GLERROR
    } else if(glfwGetKey(win_, GLFW_KEY_W)) {
      camera.rotate(1, 0, 0, deg);
    } else if(glfwGetKey(win_, GLFW_KEY_S)) {
      camera.rotate(1, 0, 0, -deg);
    } else if(glfwGetKey(win_, GLFW_KEY_A)) {
      camera.rotate(0, 1, 0, deg);
    } else if(glfwGetKey(win_, GLFW_KEY_D)) {
      camera.rotate(0, 1, 0, -deg);
    } else if(glfwGetKey(win_, GLFW_KEY_Q)) {
      camera.rotate(0, 0, 1, deg);
    } else if(glfwGetKey(win_, GLFW_KEY_E)) {
      camera.rotate(0, 0, 1, -deg);
    }
  }

  ~Window() {
    Logger::Close();
  }
};
