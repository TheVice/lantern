#ifndef MISC_H
#define MISC_H

#include <android/api-level.h>
#if __ANDROID_API__ > 8
#include <android/asset_manager_jni.h>
#else
#include <stdint.h>
#include <jni.h>
#endif

static const int32_t STATUS_OK = 0;
static const int32_t STATUS_KO = -1;

void info(const char* aMessage, ...);
void changeDir(const char* aPath);
void makeDir(const char* aPath);
void changeDirectoryToAppCacheLocation(JNIEnv* aEnv, JavaVM* aVm, jobject aClazz);
#if __ANDROID_API__ > 8
void unpackResourcesFromApk(AAssetManager* aManager);
#endif

#endif
