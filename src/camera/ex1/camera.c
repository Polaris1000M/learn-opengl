#include "camera.h"
#include <cglm/cglm.h>
#include <stdio.h>

void cameraMouseInput(GLFWwindow *window, double xPos, double yPos) {
  Camera* c = glfwGetWindowUserPointer(window);

  if(c->firstMouse) {
    c->firstMouse = 0;
    c->lastX = (float) xPos;
    c->lastY = (float) yPos;
  }

  float xOffset = xPos - c->lastX;
  float yOffset = c->lastY - yPos;
  c->lastX = xPos;
  c->lastY = yPos;

  xOffset *= c->mouseSensitivity;
  yOffset *= c->mouseSensitivity;
  
  c->yaw += xOffset;
  c->pitch += yOffset;

  vec3 direction = {cos(glm_rad(c->yaw)) * cos(glm_rad(c->pitch)), sin(glm_rad(c->pitch)), sin(glm_rad(c->yaw)) * cos(glm_rad(c->pitch))};
  glm_normalize_to(direction, c->cameraFront);
}

void cameraScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
  Camera* c = glfwGetWindowUserPointer(window);

  c->fov -= (float) yOffset;
  if(c->fov < 1.0f) {
    c->fov = 1.0f;
  }

  if(c->fov > 45.0f) {
    c->fov = 45.0f;
  }
}

void cameraInit(Camera* c, GLFWwindow* window) {
  c->cameraPos[0] = 0.0f;
  c->cameraPos[1] = 0.0f;
  c->cameraPos[2] = 3.0f;

  c->cameraFront[0] = 0.0f;
  c->cameraFront[1] = 0.0f;
  c->cameraFront[2] = -1.0f;

  c->cameraUp[0] = 0.0f;
  c->cameraUp[1] = 1.0f;
  c->cameraUp[2] = 0.0f;

  // keyboard configuration parameters
  c->deltaTime = 0.0f;
  c->lastTime = 0.0f;
  c->keySensitivity = 2.5f; 

  // mouse configuration parameters
  c->firstMouse = 1;
  c->mouseSensitivity = 0.1f;
  c->fov = 45.0f;
  c->lastX = 400.0f;
  c->lastY = 300.0f;
  c->yaw = -90.0f;
  c->pitch = 0.0f;
  
  glfwSetWindowUserPointer(window, c);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, cameraMouseInput);
  glfwSetScrollCallback(window, cameraScrollCallback);
}

void cameraProcessKeys(Camera* c, GLFWwindow* window) {
  // compute camera movements
  c->deltaTime = glfwGetTime() - c->lastTime;
  c->lastTime = glfwGetTime();

  const float cameraSpeed = c->deltaTime * c->keySensitivity;

  if(glfwGetKey(window, GLFW_KEY_W)) {
    vec3 mov;
    glm_vec3_scale(c->cameraFront, cameraSpeed, mov);
    glm_vec3_add(c->cameraPos, mov, c->cameraPos);
  }

  if(glfwGetKey(window, GLFW_KEY_S)) {
    vec3 mov;
    glm_vec3_scale(c->cameraFront, -cameraSpeed, mov);
    glm_vec3_add(c->cameraPos, mov, c->cameraPos);
  }

  if(glfwGetKey(window, GLFW_KEY_A)) {
    vec3 mov;
    glm_vec3_crossn(c->cameraFront, c->cameraUp, mov);
    glm_vec3_scale(mov, -cameraSpeed, mov);
    glm_vec3_add(c->cameraPos, mov, c->cameraPos);
  }

  if(glfwGetKey(window, GLFW_KEY_D)) {
    vec3 mov;
    glm_vec3_crossn(c->cameraFront, c->cameraUp, mov);
    glm_vec3_scale(mov, cameraSpeed, mov);
    glm_vec3_add(c->cameraPos, mov, c->cameraPos);
  }

  if(glfwGetKey(window, GLFW_KEY_SPACE)) {
    vec3 mov;
    glm_vec3_scale(c->cameraUp, cameraSpeed, mov);
    glm_vec3_add(c->cameraPos, mov, c->cameraPos);
  }

  if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
    vec3 mov;
    glm_vec3_scale(c->cameraUp, -cameraSpeed, mov);
    glm_vec3_add(c->cameraPos, mov, c->cameraPos);
  }

  c->cameraPos[1] = 0.0f;
}

void cameraLookAt(Camera* c, mat4 view) {
  vec3 cameraTarget;
  glm_vec3_add(c->cameraPos, c->cameraFront, cameraTarget);
  glm_lookat(c->cameraPos, cameraTarget, c->cameraUp, view);
}
