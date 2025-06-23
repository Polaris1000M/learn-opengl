#ifndef SHADER_H
#define SHADER_H

typedef struct Shader {
  unsigned int ID;
} Shader;

void shaderInit(Shader* s, const char* vertexPath, const char* fragmentPath);

void shaderUse(Shader* s);

void shaderSetInt(Shader* s, const char* name, int value);

void shaderSetFloat(Shader* s, const char* name, float value);

#endif
