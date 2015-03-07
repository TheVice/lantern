
#include "app_LaternRasterizedTriangleAppActivity.h"
#include "misc.h"

JNIEXPORT void JNICALL Java_com_android_lantern_rasterized_1triangle_1app_LaternRasterizedTriangleAppActivity_info
  (JNIEnv *aEnv, jclass aThis, jstring aMessage)
{
	const char* message = aEnv->GetStringUTFChars(aMessage, 0);
	info(message);
	aEnv->ReleaseStringUTFChars(aMessage, message);
}

JNIEXPORT void JNICALL Java_com_android_lantern_rasterized_1triangle_1app_LaternRasterizedTriangleAppActivity_changeDir
  (JNIEnv *aEnv, jclass aThis, jstring aDirName)
{
	const char* dirName = aEnv->GetStringUTFChars(aDirName, 0);
	info("change directory onto: %s", dirName);
	changeDir(dirName);
	aEnv->ReleaseStringUTFChars(aDirName, dirName);
}

JNIEXPORT void JNICALL Java_com_android_lantern_rasterized_1triangle_1app_LaternRasterizedTriangleAppActivity_makeDir
  (JNIEnv *aEnv, jclass aThis, jstring aDirName)
{
	const char* dirName = aEnv->GetStringUTFChars(aDirName, 0);
	info("creating directory: %s", dirName);
	makeDir(dirName);
	aEnv->ReleaseStringUTFChars(aDirName, dirName);
}
