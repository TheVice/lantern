
#include "org_lantern_Lantern.h"
#include "empty_app.h"

static lantern::empty_app empty_app;

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
