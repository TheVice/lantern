
#include <android/asset_manager_jni.h>
#include <android_native_app_glue.h>
#include <android/log.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>

extern int main(int argc, char** argv);

void info(const char* message, ...)
{
	va_list varArgs;
	va_start(varArgs, message);
	__android_log_vprint(ANDROID_LOG_INFO, "lantern tests", message, varArgs);
	__android_log_print(ANDROID_LOG_INFO, "lantern tests", "\n");
	va_end(varArgs);
}

void changeDirectoryToAppCacheLocation(JNIEnv* env, JavaVM* vm, jobject clazz)
{
	vm->AttachCurrentThread(&env, NULL);
	jclass activityClass = env->GetObjectClass(clazz);
	// Get path to cache dir (/data/data/appPackageName/cache)
	jmethodID getCacheDir = env->GetMethodID(activityClass, "getCacheDir",
	                        "()Ljava/io/File;");
	jobject file = env->CallObjectMethod(clazz, getCacheDir);
	jclass fileClass = env->FindClass("java/io/File");
	jmethodID getAbsolutePath = env->GetMethodID(fileClass, "getAbsolutePath",
	                            "()Ljava/lang/String;");
	jstring jpath = (jstring)env->CallObjectMethod(file, getAbsolutePath);
	const char* app_dir = env->GetStringUTFChars(jpath, NULL);
	// chdir in the application cache directory
	info("app cache dir: %s", app_dir);
	chdir(app_dir);
	env->ReleaseStringUTFChars(jpath, app_dir);
	vm->DetachCurrentThread();
}

void unpackResourcesFromApk(AAssetManager* mgr)
{
	const char* dir_name = "resources";
	mkdir(dir_name, S_IRWXU | S_IRWXG);
	AAssetDir* assetDir = AAssetManager_openDir(mgr, dir_name);
	const char* filename = (const char*)NULL;

	while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL)
	{
		char input_path[BUFSIZ];
		strcpy(input_path, dir_name);
		strcat(input_path, "/");
		strcat(input_path, filename);
		AAsset* asset = AAssetManager_open(mgr, input_path, AASSET_MODE_STREAMING);
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

void android_main(android_app* app)
{
	bool runTest = true;
	app_dummy();
	info("Starting event loop");

	while (true)
	{
		int32_t events;
		android_poll_source* source;

		while (ALooper_pollAll(-1, NULL, &events, (void**)&source) >= 0)
		{
			if (source != NULL)
			{
				info("Processing an event");
				source->process(app, source);

				if (runTest)
				{
					info("Start work");
					info("Extracting resources");
					changeDirectoryToAppCacheLocation(app->activity->env, app->activity->vm,
					                                  app->activity->clazz);
					unpackResourcesFromApk(app->activity->assetManager);
					info("Extracting resources done");
					int argc = 0;
					char** argv = NULL;
					info("Start tests");
					main(argc, argv);
					info("Tests finish");
					runTest = false;
					ANativeActivity_finish(app->activity);
					info("Finish work");
				}
			}

			if (app->destroyRequested)
			{
				info("Exiting event loop");
				return;
			}
		}
	}
}
