
#include "org_lantern_Lantern.h"
#include "empty_app.h"

JNIEXPORT void JNICALL Java_org_lantern_Lantern_start
  (JNIEnv* env, jclass, jintArray area, jint width, jint height)
{
	jint* elements = env->GetIntArrayElements(area, 0);
	//
	lantern::empty_app{static_cast<unsigned int>(width), static_cast<unsigned int>(height)}.start(elements);
	//
	env->SetIntArrayRegion(area, 0, width * height, elements);
	env->ReleaseIntArrayElements(area, elements, 0);
}
