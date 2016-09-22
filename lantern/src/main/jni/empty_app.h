#ifndef _EMPTY_APP_H_
#define _EMPTY_APP_H_

#include "application.h"

namespace lantern
{

class empty_app : public application::application
{
public:
	empty_app();

public:
	void frame(float dt) override;
};

}

#endif
