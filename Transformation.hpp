#pragma once

#include "Logger.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// is affine
struct Transformation {
  glm::mat4 initial;
  glm::mat4 translation;
  glm::mat4 rotation;
  glm::mat4 scaling;
  bool has_changed = true;

  Transformation():
    initial(1.0f),
    translation(1.0f),
    rotation(1.0f),
    scaling(1.0f)
  {}

  Transformation(glm::mat4 translation, glm::mat4 rotation, glm::mat4 scaling, glm::mat4 initial=glm::mat4(1.0f)):
    translation(translation),
    rotation(rotation),
    scaling(scaling),
    initial(initial)
  {}

  Transformation operator*(const Transformation &other) {
    return Transformation(
      translation * other.translation,
      rotation * other.rotation,
      scaling * other.scaling,
      other.get_matrix()
    );
  }

  glm::mat4 get_matrix() const {
    return translation * rotation * scaling * initial;
  }

  decltype(auto) inverse() const {
    return glm::affineInverse(get_matrix());
  }

  void SetPosition(float x, float y, float z) {
    translation = glm::translate(glm::vec3(x, y, z));
    has_changed = true;
  }

  void SetPosition(glm::vec3 p) {
    SetPosition(p.x, p.y, p.z);
  }

  void MovePosition(float x, float y, float z) {
    translation = glm::translate(glm::vec3(x, y, z)) * translation;
    has_changed = true;
  }

  void MovePosition(glm::vec3 p) {
    MovePosition(p.x, p.y, p.z);
  }

  glm::vec4 GetPosition() const {
    return translation * glm::vec4(0.f);
  }

  void Rotate(float x, float y, float z, float deg) {
    rotation = glm::rotate(glm::radians(deg), glm::vec3(x, y, z)) * rotation;
    has_changed = true;
  }

  void Rotate(glm::vec3 axis, float deg) {
    Rotate(axis.x, axis.y, axis.z, deg);
  }

  void SetRotation(float x, float y, float z, float deg) {
    rotation = glm::rotate(glm::radians(deg), glm::vec3(x, y, z));
    has_changed = true;
  }

  void Scale(float scale) {
    scaling = glm::scale(glm::vec3(scale, scale, scale)) * scaling;
    has_changed = true;
  }

  void Scale(float sx, float sy, float sz) {
    scaling = glm::scale(glm::vec3(sx, sy, sz)) * scaling;
    has_changed = true;
  }

  void SetScale(float scale) {
    scaling = glm::scale(glm::vec3(scale, scale, scale));
    has_changed = true;
  }

  void SetScale(float sx, float sy, float sz) {
    scaling = glm::scale(glm::vec3(sx, sy, sz));
    has_changed = true;
  }

  void SetScale(glm::vec3 s) {
    SetScale(s.x, s.y, s.z);
  }

  glm::vec3 GetScale() const {
    return glm::vec3(scaling * glm::vec4(1.f));
  }

  Transformation &operator=(const Transformation &other) {
    if(this != &other) {
      initial = other.initial;
      translation = other.translation;
      rotation = other.rotation;
      scaling = other.scaling;
      has_changed = true;
    }
    return *this;
  }
};
