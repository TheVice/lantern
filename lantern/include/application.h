#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "renderer.h"
#include <memory>

namespace application
{

class application
{
public:
	application();
	~application();

public:
	application(const application& aRhs) = delete;
	application& operator = (const application& aRhs) = delete;

public:
	virtual void initialize(int width, int height);
	virtual void frame(float dt);
	virtual void on_key_down(unsigned char key);

protected:
	lantern::texture& get_target_texture();
	lantern::renderer& get_renderer();

private:
	std::unique_ptr<lantern::texture> m_target_texture;
	std::unique_ptr<lantern::renderer> m_renderer;
};

}

#endif
