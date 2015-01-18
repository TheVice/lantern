#include "app.h"

using namespace lantern;

class empty_app : public app
{
#ifdef ANDROID
public:
	empty_app(android_app* application) : app(application){};
protected:
	void frame(float delta_since_last_frame) override {};
	int32_t on_key_down(unsigned char key) override {return 0;};
#else
public:
	empty_app(unsigned int const width, unsigned int const height);

protected:
	void frame(float delta_since_last_frame) override;
#endif
};
#ifdef ANDROID
void android_main(android_app* application)
{
	info("android_main");

	empty_app{application}.start();
}
#else
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
