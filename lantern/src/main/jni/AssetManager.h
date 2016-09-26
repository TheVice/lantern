#ifndef _ASSET_MANAGER_H_
#define _ASSET_MANAGER_H_

#include <jni.h>
#include <vector>
#include <string>

struct AssetManager
{
	static void close(JNIEnv* env, jobject object);
	static std::vector<std::string> getLocales(JNIEnv* env, jobject object);
	static std::vector<std::string> list(JNIEnv* env, jobject object, const char* path);
	static std::vector<int8_t> open(JNIEnv* env, jobject object, const char* fileName);
};

#endif
