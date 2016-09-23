
#include "logging.h"

#ifdef ANDROID
#if __ANDROID_API__ >= 3
#include <stdarg.h>

int loggingMessage(int aPriority, const char* aTag, const char* aMessage)
{
	return __android_log_write(aPriority, aTag, aMessage);
}

int loggingMessageFormat(int aPriority, const char* aTag, const char* aMessageFormat, ...)
{
	va_list varArgs;
	va_start(varArgs, aMessageFormat);
	int ret = __android_log_vprint(aPriority, aTag, aMessageFormat, varArgs);

	if (ret < 0)
	{
		va_end(varArgs);
		return ret;
	}

	ret = __android_log_print(aPriority, aTag, "\n");
	va_end(varArgs);
	return ret;
}

void loggingAssert(const char* aConditional, const char* aTag, const char* aMessageFormat, ...)
{
	va_list varArgs;
	va_start(varArgs, aMessageFormat);
	const int ret = __android_log_vprint(ANDROID_LOG_FATAL, aTag, aMessageFormat, varArgs);

	if (ret < 0)
	{
		va_end(varArgs);
		return;
	}

	__android_log_assert(aConditional, aTag, "\n");
	va_end(varArgs);
}

#endif
#endif
