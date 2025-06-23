#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct Texture {
  unsigned int ID;
} Texture;

void textureInit(Texture* t, const char* textureSource);

#endif

