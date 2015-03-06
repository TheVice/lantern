#include <iostream>
#include <stdexcept>
#include "app.h"

using namespace lantern;
#ifndef ANDROID
app::app(unsigned int const width, unsigned int const height)
	: m_window{nullptr},
	  m_renderer{nullptr},
	  m_sdl_target_texture{nullptr},
	  m_target_texture{width, height},
	  m_target_framerate_delay(0)
{
	// Initialize SDL library and according objects
	//

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_window = SDL_CreateWindow(
		"lantern",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_SHOWN);
	if (m_window == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_sdl_target_texture = SDL_CreateTexture(
		m_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);
	if (m_sdl_target_texture == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	set_target_framerate(60);
}

app::~app()
{
	// Clean up SDL library
	//

	if (m_sdl_target_texture != nullptr)
	{
		SDL_DestroyTexture(m_sdl_target_texture);
		m_sdl_target_texture = nullptr;
	}

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}

	if (m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

	SDL_Quit();
}

int app::start()
{
	bool running{true};
	SDL_Event event;

	// Time when last frame was executed
	Uint32 last_frame_time{0};

	Uint32 time_accumulator{0};
	unsigned int frames_accumulator{0};

	while(running)
	{
		// Calculate time since last frame
		//
		Uint32 current_time{SDL_GetTicks()};
		Uint32 delta_since_last_frame{current_time - last_frame_time};

		// Save last frame time
		last_frame_time = SDL_GetTicks();

		// Process events
		//
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				on_key_down(event.key.keysym);
			}
		}

		// Clear texture with black
		m_target_texture.clear(0);

		// Execute frame
		frame(delta_since_last_frame / 1000.0f);

		// Sum up passed time
		time_accumulator += delta_since_last_frame;

		// Present texture on a screen
		//
		SDL_UpdateTexture(m_sdl_target_texture, nullptr, m_target_texture.get_data(), m_target_texture.get_pitch());
		SDL_RenderCopy(m_renderer, m_sdl_target_texture, nullptr, nullptr);
		SDL_RenderPresent(m_renderer);

		// Sum up passed frames
		++frames_accumulator;

		if (m_target_framerate_delay > 0)
		{
			// Calculate delay we must have to stick to the target framerate
			//
			Uint32 time_required_for_frame = SDL_GetTicks() - last_frame_time;
			int time_to_wait = m_target_framerate_delay - time_required_for_frame;

			if (time_to_wait > 0)
			{
				SDL_Delay(static_cast<Uint32>(time_to_wait));
			}
		}

		// Drop frames and seconds counters to zero every second
		//
		if (time_accumulator >= 1000)
		{
#ifdef LANTERN_DEBUG_OUTPUT_FPS
			std::cout << "FPS: " << frames_accumulator << std::endl;
#endif
			time_accumulator = 0;
			frames_accumulator = 0;
		}
	}

	return 0;
}
#else
app::app(unsigned int const width, unsigned int const height)
	: m_target_texture{width, height},
	  m_target_framerate_delay(0)
{
	set_target_framerate(60);
}

app::~app()
{
}
#endif
texture& app::get_target_texture()
{
	return m_target_texture;
}

pipeline& app::get_pipeline()
{
	return m_pipeline;
}
#ifndef ANDROID
void app::on_key_down(SDL_Keysym const)
{
	// Does not handle any key by default
}
#endif
void app::set_target_framerate(unsigned int const fps)
{
	if (fps == 0)
	{
		m_target_framerate_delay = 0;
	}
	else
	{
		m_target_framerate_delay = 1000 / fps;
	}
}
