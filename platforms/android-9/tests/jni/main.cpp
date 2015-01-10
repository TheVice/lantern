
#include <android_native_app_glue.h>
#include <android/log.h>
#include <stdarg.h>

extern int main(int argc, char** argv);

void info(const char* message, ...)
{
	va_list varArgs;
	va_start(varArgs, message);
	__android_log_vprint(ANDROID_LOG_INFO, "lantern tests", message, varArgs);
	__android_log_print(ANDROID_LOG_INFO, "lantern tests", "\n");
	va_end(varArgs);
}

void android_main(android_app* app)
{
	bool runTest = true;

	app_dummy();

	info("Starting event loop");
	while(true)
	{
		int32_t events;
		android_poll_source* source;
		while(ALooper_pollAll(-1, NULL, &events, (void**)&source) >= 0)
		{
			if (source != NULL)
			{
				info("Processing an event");
				source->process(app, source);
				if (runTest)
				{
					int argc = 0;
					char** argv = NULL;
					main(argc, argv);
					runTest = false;
					ANativeActivity_finish(app->activity);
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
