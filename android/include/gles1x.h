#ifndef GLES1X_H
#define GLES1X_H

#include <GLES/gl.h>

void reshape(GLsizei aWidth, GLsizei aHeight);
void revertImage(const GLubyte* aSrcImage, GLubyte* aDestImage);
void initTexture(GLuint* aTextureName);
void dispalyTexture(GLuint aTextureName, GLuint aWidth, GLuint aHeight,
                    const GLubyte* aImage);
void releaseTexture(GLuint* aTextureName);

#endif
