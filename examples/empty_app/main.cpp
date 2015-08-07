#ifdef ANDROID

#include "internalApp.h"
#include "androidApp.h"
#include "misc.h"

class internel_empty_app : public internalApp
{
public:
	internel_empty_app(unsigned int const aWidth, unsigned int const aHeight) :
		internalApp(aWidth, aHeight) {};
protected:
	void frame(float const delta_since_last_frame) override {};
	int32_t on_key_down(unsigned char const key) override {return 0;};
};

class android_empty_app : public androidApp
{
public:
	android_empty_app(android_app* aApplication) : androidApp(aApplication){};
protected:
	int32_t onActivate() override
	{
		androidApp::onActivate();
		mLanternApp = new internel_empty_app(mGraphics.getWidth(), mGraphics.getHeight());

		info("Activating App OK");
		return STATUS_OK;
	}
};

void android_main(android_app* application)
{
	info("android_main");

	android_empty_app{application}.start();
}

#else

#include "app.h"

using namespace lantern;

class empty_app : public app
{
public:
	empty_app(unsigned int const width, unsigned int const height);

protected:
	void frame(float const delta_since_last_frame) override;
};

empty_app::empty_app(unsigned int const width, unsigned int const height)
	: app(width, height)
{
}

void empty_app::frame(float delta_since_last_frame)
{
}

int main(int argc, char* argv[])
{
	return empty_app{640, 480}.start();
}

#endif
