#ifndef _LOGGING_H_
#define _LOGGING_H_

#ifdef ANDROID
#include <android/api-level.h>
#if __ANDROID_API__ >= 3
#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif

int loggingMessage(int aPriority, const char* aTag, const char* aMessage);
int loggingMessageFormat(int aPriority, const char* aTag, const char* aMessageFormat, ...);
void loggingAssert(const char* aConditional, const char* aTag, const char* aMessageFormat, ...);

#ifdef __cplusplus
}
#endif

#define TAG "Native"
#define LOGI(...) loggingMessageFormat(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) loggingMessageFormat(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) loggingMessageFormat(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

#endif
#endif

#endif
