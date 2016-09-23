
#include "org_lantern_Lantern.h"
#include "AssetManager.h"
#include "empty_app.h"
#include "logging.h"

static lantern::empty_app empty_app;

void printTree(JNIEnv* env, jobject object, const char* path)
{
	const auto assets = AssetManager::list(env, object, path);

	for (const auto& asset : assets)
	{
		LOGI(asset.c_str());
		printTree(env, object, asset.c_str());
	}
}

JNIEXPORT jint JNICALL Java_org_lantern_Lantern_set_1asset_1manager
		(JNIEnv* env, jclass, jobject object)
{
	LOGI("Asset manager %p", object);

	const auto locales = AssetManager::getLocales(env, object);

	for (const auto& local : locales)
	{
		LOGI(local.c_str());
	}

	printTree(env, object, "");

	const auto fileData = AssetManager::open(env, object, "webkit/youtube.html");

	if (!fileData.empty())
	{
		std::string data(fileData.size(), '\0');
		std::memcpy(&data.front(), &fileData.front(), fileData.size());
		LOGI("%i", fileData.size());
		LOGI("%s", data.c_str());
	}

	/*for (const auto& singleByte : fileData)
	{
		LOGI("%i", singleByte);
	}*/

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
