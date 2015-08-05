
#include "gles1x.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES/glext.h>

#ifdef REVERT_IMAGE
static GLubyte gImage[4 * 4096 * 2160] = {0};
void revertImage(const GLubyte* aSrcImage, GLuint aLength, GLuint aWidth,
                 GLubyte* aDestImage)
{
	GLuint k = 0;

	for (GLuint i = aLength; i > 0; i -= aWidth)
	{
		for (GLuint j = i + aWidth - 1; j >= i; --j)
		{
			aDestImage[k++] = aSrcImage[j];
		}
	}
}
#endif

void reshape(GLsizei aWidth, GLsizei aHeight)
{
	glViewport(0, 0, aWidth, aHeight);
	glMatrixMode(GL_PROJECTION);
}

void initTexture(GLuint* aTextureName)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, aTextureName);
	glBindTexture(GL_TEXTURE_2D, *aTextureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void dispalyTexture(GLuint aTextureName, GLuint aWidth, GLuint aHeight,
                    const GLubyte* aImage)
{
#ifdef REVERT_IMAGE
	revertImage(aImage, 4 * aWidth * aHeight, aWidth, gImage);
#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, aTextureName);
#ifdef REVERT_IMAGE
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aWidth,
	             aHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, gImage);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aWidth,
	             aHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, aImage);
#endif
	GLint crop[] =
	{
		0,
		0,
		static_cast<GLint>(aWidth),
		static_cast<GLint>(aHeight)
	};
	glTexParameteriv(GL_TEXTURE_2D,
	                 GL_TEXTURE_CROP_RECT_OES,
	                 crop);
	glDrawTexfOES(0.0f, 0.0f, 0.0f, aWidth, aHeight);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFlush();
	glDisable(GL_TEXTURE_2D);
}

void releaseTexture(GLuint* aTextureName)
{
	if ((*aTextureName) != 0)
	{
		glDeleteTextures(1, aTextureName);
		(*aTextureName) = 0;
	}
}
