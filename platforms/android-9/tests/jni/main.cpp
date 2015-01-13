
#include "app.h"

extern int main(int argc, char** argv);

using namespace lantern;

class tests : public app
{
public:
	tests(android_app* application);

protected:
	void frame(float delta_since_last_frame) override {};
	int32_t on_key_down(unsigned char key) override {return 0};
	int32_t onActivate() override;
};

tests::tests(android_app* application) : app(application)
{
}

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
