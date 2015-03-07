#ifndef MISC_H
#define MISC_H

#include <android/asset_manager_jni.h>

static const int32_t STATUS_OK = 0;
static const int32_t STATUS_KO = -1;

void info(const char* aMessage, ...);
void changeDirectoryToAppCacheLocation(JNIEnv* aEnv, JavaVM* aVm, jobject aClazz);
void unpackResourcesFromApk(AAssetManager* aManager);
unsigned long getTimer();

#endif
