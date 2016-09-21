#include "app.h"

namespace lantern
{

class empty_app : public app
{
public:
	empty_app(unsigned int const width, unsigned int const height);

protected:
	void frame(float const delta_since_last_frame) override;
};

}
