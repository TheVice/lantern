#ifndef GLES1X_H
#define GLES1X_H

#include <GLES/gl.h>

//#define REVERT_IMAGE 1

#ifdef REVERT_IMAGE
void revertImage(const GLubyte* aSrcImage, GLubyte* aDestImage);
#endif
void reshape(GLsizei aWidth, GLsizei aHeight);
void initTexture(GLuint* aTextureName);
void dispalyTexture(GLuint aTextureName, GLuint aWidth, GLuint aHeight,
                    const GLubyte* aImage);
void releaseTexture(GLuint* aTextureName);

#endif
