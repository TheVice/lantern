
#include "InputStream.h"

jint InputStream::available(JNIEnv* env, jobject object)
{
	const jclass clazz = env->GetObjectClass(object);
	const jmethodID methodID = env->GetMethodID(clazz, "available", "()I");
	const jint availableValue = env->CallIntMethod(object, methodID);
	return availableValue;
}

void InputStream::close(JNIEnv* env, jobject object)
{
	const jclass clazz = env->GetObjectClass(object);
	const jmethodID methodID = env->GetMethodID(clazz, "close", "()V");
	env->CallVoidMethod(object, methodID);
}

jint InputStream::read(JNIEnv* env, jobject object, jbyteArray buffer, jint bufferLength)
{
	const jclass clazz = env->GetObjectClass(object);
	const jmethodID methodID = env->GetMethodID(clazz, "read", "([BII)I");
	const jint offset = 0;
	const jint readCount = env->CallIntMethod(object, methodID, buffer, offset, bufferLength);
	return readCount;
}
