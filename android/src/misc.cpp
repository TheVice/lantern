
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
	const char* app_dir = aEnv->GetStringUTFChars(jpath, NULL);
	// chdir in the application cache directory
	info("app cache dir: %s", app_dir);
	chdir(app_dir);
	aEnv->ReleaseStringUTFChars(jpath, app_dir);
	aVm->DetachCurrentThread();
}

void unpackResourcesFromApk(AAssetManager* aManager)
{
	const char* dir_name = "resources";
	mkdir(dir_name, S_IRWXU | S_IRWXG);
	AAssetDir* assetDir = AAssetManager_openDir(aManager, dir_name);
	const char* filename = (const char*) NULL;

	while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL)
	{
		char input_path[BUFSIZ];
		strcpy(input_path, dir_name);
		strcat(input_path, "/");
		strcat(input_path, filename);
		AAsset* asset = AAssetManager_open(aManager, input_path,
		                                   AASSET_MODE_STREAMING);
		char buf[BUFSIZ];
		int nb_read = 0;
		char output_path[BUFSIZ];
		strcpy(output_path, dir_name);
		strcat(output_path, "/");
		strcat(output_path, filename);
		FILE* out = fopen(output_path, "w");

		while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0)
		{
			fwrite(buf, nb_read, 1, out);
		}

		fclose(out);
		AAsset_close(asset);
	}

	AAssetDir_close(assetDir);
}
