#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <math.h>
#include "shader.h"
#include "texture.h"

const unsigned int WINDOW_HEIGHT = 600;
const unsigned int WINDOW_WIDTH = 800;

int firstMouse = 1;
float lastX = 400.0f;
float lastY = 300.0f;
float yaw = -90.0f;
float pitch = 0.0f;

vec3 cameraPos = {0.0f, 0.0f, 3.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f, 0.0f};
float lastTime = 0.0f;
float deltaTime = 0.0f;
float fov = 45.0f;

// handle when window size changes
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

// add user interactivity
void processInput(GLFWwindow *window, mat4 view)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
  // compute camera movements
  deltaTime = glfwGetTime() - lastTime;
  lastTime = glfwGetTime();
  const float cameraSpeed = deltaTime * 2.5f;
  if(glfwGetKey(window, GLFW_KEY_W)) {
    vec3 mov;
    glm_vec3_scale(cameraFront, cameraSpeed, mov);
    glm_vec3_add(cameraPos, mov, cameraPos);
  }

  if(glfwGetKey(window, GLFW_KEY_S)) {
    vec3 mov;
    glm_vec3_scale(cameraFront, -cameraSpeed, mov);
    glm_vec3_add(cameraPos, mov, cameraPos);
  }

  if(glfwGetKey(window, GLFW_KEY_A)) {
    vec3 mov;
    glm_vec3_crossn(cameraFront, cameraUp, mov);
    glm_vec3_scale(mov, -cameraSpeed, mov);
    glm_vec3_add(cameraPos, mov, cameraPos);
  }

  if(glfwGetKey(window, GLFW_KEY_D)) {
    vec3 mov;
    glm_vec3_crossn(cameraFront, cameraUp, mov);
    glm_vec3_scale(mov, cameraSpeed, mov);
    glm_vec3_add(cameraPos, mov, cameraPos);
  }

  if(glfwGetKey(window, GLFW_KEY_SPACE)) {
    vec3 mov;
    glm_vec3_scale(cameraUp, cameraSpeed, mov);
    glm_vec3_add(cameraPos, mov, cameraPos);
  }

  if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
    vec3 mov;
    glm_vec3_scale(cameraUp, -cameraSpeed, mov);
    glm_vec3_add(cameraPos, mov, cameraPos);
  }

  vec3 cameraTarget;
  glm_vec3_add(cameraPos, cameraFront, cameraTarget);
  glm_lookat(cameraPos, cameraTarget, cameraUp, view);
}

void mouseInput(GLFWwindow *window, double xPos, double yPos) {
  if(firstMouse) {
    firstMouse = 0;
    lastX = xPos;
    lastY = yPos;
  }

  float xOffset = xPos - lastX;
  float yOffset = lastY - yPos;
  lastX = xPos;
  lastY = yPos;

  const float sensitivity = 0.1f;
  xOffset *= sensitivity;
  yOffset *= sensitivity;
  
  yaw += xOffset;
  pitch += yOffset;

  vec3 direction = {cos(glm_rad(yaw)) * cos(glm_rad(pitch)), sin(glm_rad(pitch)), sin(glm_rad(yaw)) * cos(glm_rad(pitch))};
  glm_normalize_to(direction, cameraFront);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
  fov -= (float) yOffset;
  if(fov < 1.0f) {
    fov = 1.0f;
  }

  if(fov > 45.0f) {
    fov = 45.0f;
  }
}

int main() {
  // initalize the window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // load GLAD to retrieve OpenGL driver functions
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseInput);
  glfwSetScrollCallback(window, scrollCallback);

  Shader s;
  shaderInit(&s, "../src/VS", "../src/FS");

  Texture container;
  textureInit(&container, GL_TEXTURE0, "../assets/container.jpg", 0, 0);

  Texture smiley;
  textureInit(&smiley, GL_TEXTURE1, "../assets/awesomeface.png", 0, 1);
  
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  unsigned int VBO, VAO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // specify position for coordinates
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
  glEnableVertexAttribArray(0);

  // specify position for texture coordinates
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  shaderUse(&s);
  shaderSetInt(&s, "texture1", 0);
  shaderSetInt(&s, "texture2", 1);

  glEnable(GL_DEPTH_TEST);

  unsigned int modelLoc, projectionLoc, viewLoc;

  vec3 cubePositions[] = {
    {0.0f,  0.0f,  0.0f}, 
    {2.0f,  5.0f, -15.0f}, 
    {-1.5f, -2.2f, -2.5f},  
    {-3.8f, -2.0f, -12.3f},  
    { 2.4f, -0.4f, -3.5f},  
    {-1.7f,  3.0f, -7.5f},  
    { 1.3f, -2.0f, -2.5f},  
    { 1.5f,  2.0f, -2.5f}, 
    { 1.5f,  0.2f, -1.5f}, 
    {-1.3f,  1.0f, -1.5f}  
  };

  for(unsigned int i = 0; i < 10; i++) {
    mat4 model = GLM_MAT4_IDENTITY;
    glm_translate_make(model, cubePositions[i]);
    modelLoc = glGetUniformLocation(s.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const float*) model);
  }


  while(!glfwWindowShouldClose(window)) {
    mat4 view;
    processInput(window, view);
    viewLoc = glGetUniformLocation(s.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const float*) view);

    mat4 projection = GLM_MAT4_IDENTITY;
    glm_perspective(glm_rad(fov), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f, projection); 
    projectionLoc = glGetUniformLocation(s.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (const float*) projection);

    glClearColor(0.0f, 0.0f, 0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(unsigned int i = 0; i < 10; i++) {
      mat4 model = GLM_MAT4_IDENTITY;
      glm_translate_make(model, cubePositions[i]);
      modelLoc = glGetUniformLocation(s.ID, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const float*) model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // clean up
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(s.ID);

  glfwTerminate();
  return 0;
}
