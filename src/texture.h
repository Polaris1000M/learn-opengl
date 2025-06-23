#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct Texture {
  unsigned int ID;
  unsigned int textureUnit;
} Texture;

void textureInitOpaque(Texture* t, unsigned int textureUnit, const char* textureSource);

void textureInitTransparent(Texture* t, unsigned int textureUnit, const char* textureSource);

#endif
