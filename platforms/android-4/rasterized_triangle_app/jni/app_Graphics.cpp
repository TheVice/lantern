
#include "app_Graphics.h"
#include "internel_rasterized_color_triangle_app.h"
#include "gles1x.h"
#include "misc.h"

static internel_rasterized_color_triangle_app* gApp = nullptr;
static GLuint gTextureName = 0;

void changeDirectoryToAppCacheLocation(JNIEnv* aEnv, JavaVM* aVm,
                                       jobject aClazz);

JNIEXPORT void JNICALL Java_com_android_lantern_rasterized_1triangle_1app_Graphics_on_1surface_1created(JNIEnv* aEnv, jclass aThis)
{
}

JNIEXPORT void JNICALL Java_com_android_lantern_rasterized_1triangle_1app_Graphics_on_1surface_1changed(JNIEnv* aEnv, jclass aThis, jint aWidth, jint aHeight)
{
	info("surface changed(%i %i)", aWidth, aHeight);
	gApp = new internel_rasterized_color_triangle_app(aWidth, aHeight);
	reshape(aWidth, aHeight);
	initTexture(&gTextureName);
}

JNIEXPORT void JNICALL Java_com_android_lantern_rasterized_1triangle_1app_Graphics_on_1draw_1frame(JNIEnv* aEnv, jclass aThis)
{
	info("frame");
	gApp->start(gTextureName);
}

int on_key_down(unsigned char const key)
{
	return gApp->on_key_down_for_jni(key);
}
