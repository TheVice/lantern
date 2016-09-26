#ifndef _INPUT_STREAM_H_
#define _INPUT_STREAM_H_

#include <jni.h>

struct InputStream
{
	static jint available(JNIEnv* env, jobject object);
	static void close(JNIEnv* env, jobject object);
	static jint read(JNIEnv* env, jobject object, jbyteArray buffer, jint bufferLength);
};

#endif
