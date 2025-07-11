#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

// reads shader specified by filepath into C-string
char* readShader(const char* shaderPath) {
  FILE* fp;
  char* buffer;
  
  fp = fopen(shaderPath, "rb");
  if(!fp) {
    printf("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: %s\n", shaderPath);
    buffer = 0; 
    return buffer;
  }

  // find length of file
  fseek(fp, 0L, SEEK_END);
  long shaderSize = ftell(fp);
  rewind(fp);

  buffer = calloc(1, shaderSize + 1);

  if(!buffer) {
    printf("ERROR::SHADER::FAILED_TO_ALLOCATE_BUFFER\n");
    buffer = 0;
    return buffer;
  }

  if(fread(buffer, shaderSize, 1, fp) != 1) {
    printf("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: %s\n", shaderPath);
    buffer = 0; 
    return buffer;
  }

  return buffer;
}

void shaderInit(Shader* s, const char* vertexPath, const char* fragmentPath) {
  // read vertex shader source
  char* vertexShaderSource = readShader(vertexPath);
  if(!vertexShaderSource) {
    return;
  }

  // read fragment shader source
  char* fragmentShaderSource = readShader(fragmentPath);
  if(!fragmentShaderSource) {
    return;
  }

  // compile shader programs
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, (const char**) &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
  }

  unsigned int fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, (const char**) &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
  }

  // link shaders into shader program
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
  }

  glDeleteShader(vertexShader);

  glDeleteShader(fragmentShader);
  free(vertexShaderSource);
  free(fragmentShaderSource);
  
  s->id = shaderProgram;
}

void shaderUse(Shader* s) {
  glUseProgram(s->id);
}

void shaderSetInt(Shader* s, const char* name, int value) {
  glUniform1i(glGetUniformLocation(s->id, name), value); 
}

void shaderSetFloat(Shader* s, const char* name, float value) {
  glUniform1f(glGetUniformLocation(s->id, name), value); 
}
