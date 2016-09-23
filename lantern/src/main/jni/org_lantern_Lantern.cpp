
#include "org_lantern_Lantern.h"
#include "empty_app.h"
#include "logging.h"

static lantern::empty_app empty_app;

JNIEXPORT jint JNICALL Java_org_lantern_Lantern_set_1asset_1manager
		(JNIEnv* env, jclass, jobject object)
{
	LOGI("Asset manager %p", object);

	if (object)
	{
		jclass clazz = env->GetObjectClass(object);
		LOGI("GetObjectClass %i", clazz);

		// getLocales
		{
			jmethodID getLocalesID = env->GetMethodID(clazz, "getLocales", "()[Ljava/lang/String;");
			LOGI("GetMethodID %i of getLocales", getLocalesID);

			if (getLocalesID)
			{
				jobjectArray array = (jobjectArray) env->CallObjectMethod(object, getLocalesID);
				LOGI("CallObjectMethod %i", array);

				if (array)
				{
					jsize count = env->GetArrayLength(array);
					LOGI("Array size %i", count);

					for (jsize i = 0; i < count; ++i)
					{
						jboolean isCopy;
						jstring pointer = (jstring) env->GetObjectArrayElement(array, i);
						const char *localString = env->GetStringUTFChars(pointer, &isCopy);
						LOGI("%i: %s", i, localString);
						env->ReleaseStringUTFChars(pointer, localString);
					}
				}
			}
		}

		// list
		{
			jmethodID methodID = env->GetMethodID(clazz, "list", "(Ljava/lang/String;)[Ljava/lang/String;");
			LOGI("GetMethodID %i of list", methodID);

			if (methodID)
			{
				jstring path = env->NewStringUTF("");
				jobjectArray array = (jobjectArray) env->CallObjectMethod(object, methodID, path);
				env->DeleteLocalRef(path);
				LOGI("CallObjectMethod %i", array);

				if (array)
				{
					jsize count = env->GetArrayLength(array);
					LOGI("Array size %i", count);

					for (jsize i = 0; i < count; ++i)
					{
						jboolean isCopy;
						jstring pointer = (jstring) env->GetObjectArrayElement(array, i);
						const char *localString = env->GetStringUTFChars(pointer, &isCopy);
						LOGI("%i: %s", i, localString);
						env->ReleaseStringUTFChars(pointer, localString);
					}
				}
			}
		}
	}

	return nullptr == object;
}

JNIEXPORT void JNICALL Java_org_lantern_Lantern_initialize
(JNIEnv *, jclass, jint width, jint height)
{
	empty_app.initialize(width, height);
}

JNIEXPORT void JNICALL Java_org_lantern_Lantern_frame
(JNIEnv* env, jclass, jfloat dt, jint width, jint height, jintArray area)
{
	empty_app.frame(dt);
	//
	jint* elements = env->GetIntArrayElements(area, 0);
	std::memcpy(elements, empty_app.get_target_texture().get_data(), width * height);
	//
	env->SetIntArrayRegion(area, 0, width * height, elements);
	env->ReleaseIntArrayElements(area, elements, 0);
}
