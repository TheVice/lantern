/*
    SDL_android_main.c, placed in the public domain by Sam Lantinga  3/13/14
*/
#include "../../SDL_internal.h"
#include <android/log.h>

#ifdef __ANDROID__

/* Include the SDL main definition header */
#include "SDL_main.h"

/*******************************************************************************
                 Functions called by JNI
*******************************************************************************/
#include <jni.h>

/* Called before SDL_main() to initialize JNI bindings in SDL library */
extern void SDL_Android_Init(JNIEnv* env, jclass cls);


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

/* Start up the SDL app */
void Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv* env, jclass cls, jstring aDirName)
{
    /* This interface could expand with ABI negotiation, calbacks, etc. */
    SDL_Android_Init(env, cls);

    SDL_SetMainReady();

	const char* dirName = (*env)->GetStringUTFChars(env, aDirName, 0);
	info("SDLActivity - change directory onto: %s", dirName);
	changeDir(dirName);
	(*env)->ReleaseStringUTFChars(env, aDirName, dirName);

    info("--------------------------> Java_org_libsdl_app_SDLActivity_nativeInit <----------------------");
    /* Run the application code! */
    int status;
    char *argv[2];
    argv[0] = SDL_strdup("SDL_app");
    argv[1] = NULL;
    status = SDL_main(1, argv);

    /* Do not issue an exit or the whole application will terminate instead of just the SDL thread */
    /* exit(status); */
}

#endif /* __ANDROID__ */

/* vi: set ts=4 sw=4 expandtab: */
