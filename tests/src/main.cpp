#if defined (ANDROID) && (TESTS_VIA_APK)

#include "app.h"
using namespace lantern;

#endif

#include <gtest/gtest.h>

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#if defined (ANDROID) && (TESTS_VIA_APK)

class tests : public app
{
public:
	tests(android_app* application) : app(application) {};

protected:
	void frame(float delta_since_last_frame) override {};
	int32_t on_key_down(unsigned char key) override {return 0;};
	int32_t onActivate() override;
};

int32_t tests::onActivate()
{
	int argc = 0;
	char** argv = NULL;
	info("Start tests");
	main(argc, argv);
	info("Tests finish");
	return STATUS_KO;
}

void android_main(android_app* app)
{
	info("android_main");

	changeDirectoryToAppCacheLocation(app->activity->env, app->activity->vm,
	                                  app->activity->clazz);
	unpackResourcesFromApk(app->activity->assetManager);

	tests{app}.start();
}

#endif
