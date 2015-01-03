#include "app.h"

using namespace lantern;

class empty_app : public app
{
public:
	empty_app(unsigned int const width, unsigned int const height);

protected:
	void frame(float delta_since_last_frame) override;
	void on_key_down(unsigned char key) override;
};

empty_app::empty_app(unsigned int const width, unsigned int const height)
	: app(width, height)
{
}

void empty_app::frame(float delta_since_last_frame)
{
}

void empty_app::on_key_down(unsigned char key)
{
}

int main(int argc, char* argv[])
{
	return empty_app{640, 480}.start();
}
