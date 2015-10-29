
#include <stdarg.h>
#include <android/log.h>

void info(const char* aMessage, ...)
{
	va_list varArgs;
	va_start(varArgs, aMessage);
	__android_log_vprint(ANDROID_LOG_INFO, "native", aMessage, varArgs);
	__android_log_print(ANDROID_LOG_INFO, "native", "\n");
	va_end(varArgs);
}
