
#include "AssetManager.h"
#include "InputStream.h"

std::vector<std::string> AssetManager::getLocales(JNIEnv* env, jobject object)
{
	std::vector<std::string> locales;
	//
	const jclass clazz = env->GetObjectClass(object);
	const jmethodID methodID = env->GetMethodID(clazz, "getLocales", "()[Ljava/lang/String;");
	//
	const jobjectArray array = (jobjectArray)env->CallObjectMethod(object, methodID);

	if (array)
	{
		const jsize count = env->GetArrayLength(array);
		locales.reserve(count);
		locales.resize(count);

		for (jsize i = 0; i < count; ++i)
		{
			jboolean isCopy;
			const jstring pointer = (jstring)env->GetObjectArrayElement(array, i);
			const char *localString = env->GetStringUTFChars(pointer, &isCopy);
			locales[i] = std::string(localString);
			env->ReleaseStringUTFChars(pointer, localString);
		}
	}

	return locales;
}

std::vector<std::string> AssetManager::list(JNIEnv* env, jobject object, const char* path)
{
	std::vector<std::string> assets;
	//
	const jclass clazz = env->GetObjectClass(object);
	const jmethodID methodID = env->GetMethodID(clazz, "list", "(Ljava/lang/String;)[Ljava/lang/String;");
	//
	const jstring jstringPath = env->NewStringUTF(path);
	const jobjectArray array = (jobjectArray)env->CallObjectMethod(object, methodID, jstringPath);
	env->DeleteLocalRef(jstringPath);

	if (array)
	{
		const jsize count = env->GetArrayLength(array);
		assets.reserve(count);
		assets.resize(count);

		for (jsize i = 0; i < count; ++i)
		{
			jboolean isCopy;
			const jstring pointer = (jstring)env->GetObjectArrayElement(array, i);
			const char *localString = env->GetStringUTFChars(pointer, &isCopy);
			assets[i] = std::string(localString);
			env->ReleaseStringUTFChars(pointer, localString);
		}
	}

	return assets;
}

std::vector<int8_t> AssetManager::open(JNIEnv* env, jobject object, const char* fileName)
{
	std::vector<int8_t> fileContent;
	//
	const jclass clazz = env->GetObjectClass(object);
	const jmethodID methodID = env->GetMethodID(clazz, "open", "(Ljava/lang/String;)Ljava/io/InputStream;");
	//
	const jstring jstringPath = env->NewStringUTF(fileName);
	const jobject inputStreamObject = env->CallObjectMethod(object, methodID, jstringPath);

	if (env->ExceptionCheck())
	{
		env->ExceptionClear();
		env->DeleteLocalRef(jstringPath);
		return fileContent;
	}

	env->DeleteLocalRef(jstringPath);

	if (inputStreamObject)
	{
		const jint bufferLength = InputStream::available(env, inputStreamObject);
		jbyteArray buffer = env->NewByteArray(bufferLength);
		jint readCount = 0;

		while (-1 != (readCount = InputStream::read(env, inputStreamObject, buffer, bufferLength)))
		{
			jbyte* pointer = env->GetByteArrayElements(buffer, 0);
			fileContent.insert(fileContent.end(), pointer, pointer + readCount);
		}

		env->DeleteLocalRef(buffer);

		InputStream::close(env, inputStreamObject);
	}

	return fileContent;
}
