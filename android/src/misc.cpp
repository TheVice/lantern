
#include <android/api-level.h>
#include "misc.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>
#include <android/log.h>

void info(const char* aMessage, ...)
{
	va_list varArgs;
	va_start(varArgs, aMessage);
	__android_log_vprint(ANDROID_LOG_INFO, "lantern", aMessage, varArgs);
	__android_log_print(ANDROID_LOG_INFO, "lantern", "\n");
	va_end(varArgs);
}

void changeDir(const char* aPath)
{
	chdir(aPath);
}

void makeDir(const char* aPath)
{
	mkdir(aPath, S_IRWXU | S_IRWXG);
}

void changeDirectoryToAppCacheLocation(JNIEnv* aEnv, JavaVM* aVm,
                                       jobject aClazz)
{
	aVm->AttachCurrentThread(&aEnv, NULL);
	jclass activityClass = aEnv->GetObjectClass(aClazz);
	// Get path to cache dir (/data/data/appPackageName/cache)
	jmethodID getCacheDir = aEnv->GetMethodID(activityClass, "getCacheDir",
	                        "()Ljava/io/File;");
	jobject file = aEnv->CallObjectMethod(aClazz, getCacheDir);
	jclass fileClass = aEnv->FindClass("java/io/File");
	jmethodID getAbsolutePath = aEnv->GetMethodID(fileClass, "getAbsolutePath",
	                            "()Ljava/lang/String;");
	jstring jpath = (jstring) aEnv->CallObjectMethod(file, getAbsolutePath);
	const char* appCacheDir = aEnv->GetStringUTFChars(jpath, NULL);
	// change directory in the application cache directory
	changeDir(appCacheDir);
	aEnv->ReleaseStringUTFChars(jpath, appCacheDir);
	aVm->DetachCurrentThread();
}

#if __ANDROID_API__ > 8
void unpackResourcesFromApk(AAssetManager* aManager)
{
	const char* dirName = "resources";
	makeDir(dirName);
	AAssetDir* assetDir = AAssetManager_openDir(aManager, dirName);
	const char* filename = (const char*) NULL;

	while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL)
	{
		char inputPath[BUFSIZ];
		strcpy(inputPath, dirName);
		strcat(inputPath, "/");
		strcat(inputPath, filename);
		AAsset* asset = AAssetManager_open(aManager, inputPath,
		                                   AASSET_MODE_STREAMING);
		char buf[BUFSIZ];
		int nbRead = 0;
		char outputPath[BUFSIZ];
		strcpy(outputPath, dirName);
		strcat(outputPath, "/");
		strcat(outputPath, filename);
		FILE* out = fopen(outputPath, "w");

		while ((nbRead = AAsset_read(asset, buf, BUFSIZ)) > 0)
		{
			fwrite(buf, nbRead, 1, out);
		}

		fclose(out);
		AAsset_close(asset);
	}

	AAssetDir_close(assetDir);
}
#endif
