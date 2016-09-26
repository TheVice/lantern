
#include "org_lantern_examples_RasterizedTriangleApp.h"
#include "AssetManager.h"
#include "rasterized_triangle_app.h"
#include "logging.h"

extern void printTree(JNIEnv* env, jobject object, const char* path);

static lantern::rasterized_triangle_app rasterized_triangle_app;

JNIEXPORT void JNICALL Java_org_lantern_examples_RasterizedTriangleApp_set_1asset_1manager
(JNIEnv* env, jclass, jobject object)
{
	printTree(env, object, "");

	const auto fileData = AssetManager::open(env, object, "triangle.obj");

	if (!fileData.empty())
	{
		LOGI("Asset size - %i", fileData.size());
		//
		std::string data(fileData.size(), '\0');
		std::memcpy(&data.front(), &fileData.front(), fileData.size());
		//
		lantern::rasterized_triangle_app::mResources["triangle.obj"] = data;
	}
	else
	{
		LOGW("Could not locate asset resource");
	}
}

JNIEXPORT void JNICALL Java_org_lantern_examples_RasterizedTriangleApp_initialize
(JNIEnv*, jclass, jint width, jint height)
{
	rasterized_triangle_app.initialize(width, height);
}

JNIEXPORT void JNICALL Java_org_lantern_examples_RasterizedTriangleApp_frame
(JNIEnv* env, jclass, jfloat dt, jint width, jint height, jintArray area)
{
	rasterized_triangle_app.frame(dt);
	//
	jint* elements = env->GetIntArrayElements(area, 0);
	std::memcpy(elements, rasterized_triangle_app.get_target_texture().get_data(), 4 * width * height);
	//
	env->SetIntArrayRegion(area, 0, width * height, elements);
	env->ReleaseIntArrayElements(area, elements, 0);
}

JNIEXPORT void JNICALL Java_org_lantern_examples_RasterizedTriangleApp_on_1key_1down
(JNIEnv *, jclass, jshort key)
{
	rasterized_triangle_app.on_key_down(static_cast<unsigned char>(key));
}
