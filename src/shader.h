#ifndef SHADER_H
#define SHADER_H

typedef struct Shader {
  unsigned int id;
} Shader;

void shaderUse(Shader* s);

void shaderInit(Shader* s, const char* vertexPath, const char* fragmentPath);

void shaderSetInt(Shader* s, const char* name, int value);

void shaderSetFloat(Shader* s, const char* name, float value);

#endif
