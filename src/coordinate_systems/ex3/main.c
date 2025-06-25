#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <math.h>
#include "shader.h"
#include "texture.h"

// handle when window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

// add user interactivity
void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
}

const unsigned int WINDOW_HEIGHT = 600;
const unsigned int WINDOW_WIDTH = 800;

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
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

  unsigned int modelLoc, viewLoc, projectionLoc;
  mat4 view = GLM_MAT4_IDENTITY;
  glm_translate_make(view, (vec3) {0.0f, 0.0f, -3.0f});
  viewLoc = glGetUniformLocation(s.ID, "view");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const float*) view);

  mat4 projection = GLM_MAT4_IDENTITY;
  glm_perspective(glm_rad(45.0f), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f, projection); 
  projectionLoc = glGetUniformLocation(s.ID, "projection");
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (const float*) projection);

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
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(unsigned int i = 0; i < 10; i++) {
      mat4 model = GLM_MAT4_IDENTITY;
      glm_translate_make(model, cubePositions[i]);
      if(i % 3 == 0) {
        glm_rotate(model, glm_rad(50.0f) * (float) glfwGetTime(), (vec3) {1.0f, 0.3f, 0.5f});
      }
      else {
        glm_rotate(model, glm_rad(20.0f * (float) i), (vec3) {1.0f, 0.3f, 0.5f});
      }
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
