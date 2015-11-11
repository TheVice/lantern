#include <iostream>
#include <stdexcept>
#include <SDL_image.h>
#if __ANDROID__
#include <jni.h>
#include <unistd.h>
#include <sys/stat.h>
#if __ANDROID_API__ > 8
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif
#endif
#include "app.h"

using namespace lantern;

app* app::_instance = nullptr;

#if __ANDROID__

bool isPathExist(const char* aPath)
{
	return -1 != access(aPath, F_OK);
}

void makeDir(const char* aPath)
{
	if(!isPathExist(aPath))
	{
		mkdir(aPath, S_IRWXU | S_IRWXG);
	}
}

void changeDir(const char* aPath)
{
	chdir(aPath);
}

char* getCurrentDir(char* aBuffer, size_t aBufferLength)
{
	return getcwd(aBuffer, aBufferLength);
}

#if __ANDROID_API__ > 8
void unpackResourcesFromApk(AAssetManager* aManager)
{
	const char* dirName = "resources";
	makeDir(dirName);
	AAssetDir* assetDir = AAssetManager_openDir(aManager, dirName);
	const char* filename = nullptr;

	while ((filename = AAssetDir_getNextFileName(assetDir)) != nullptr)
	{
		char inputPath[BUFSIZ];

//		strcpy(inputPath, dirName);
//		strcat(inputPath, "/");
//		strcat(inputPath, filename);

		sprintf(inputPath, "%s%s%s", dirName, "/", filename);

		AAsset* asset = AAssetManager_open(aManager, inputPath,
		                                   AASSET_MODE_STREAMING);
		char buf[BUFSIZ];
		int nbRead = 0;
		char outputPath[BUFSIZ];

//		strcpy(outputPath, dirName);
//		strcat(outputPath, "/");
//		strcat(outputPath, filename);

//		sprintf(outputPath, "%s%s%s", dirName, "/", filename);
		strcpy(outputPath, inputPath);

		FILE* out = fopen(outputPath, "w");
		while ((nbRead = AAsset_read(asset, buf, BUFSIZ)) > 0)
		{
			fwrite(buf, nbRead, 1, out);
		}
		fclose(out);
		AAsset_close(asset);
	}

	AAssetDir_close(assetDir);
}
#endif
#endif

app::app(unsigned int const width, unsigned int const height)
	: m_freetype_library{nullptr},
	  m_window{nullptr},
	  m_sdl_renderer{nullptr},
	  m_sdl_target_texture{nullptr},
	  m_target_texture{width, height},
	  m_target_framerate_delay{0},
	  m_last_fps{0}
{
	if (_instance != nullptr)
	{
		throw std::runtime_error("Another app isntance has already been created");
	}

	_instance = this;

	// Initialize FreeType library
	//
	if (FT_Init_FreeType(&m_freetype_library))
	{
		throw std::runtime_error("Couldn't initialize FreeType library");
	}

	// Initialize SDL library and according objects
	//

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error(SDL_GetError());
	}

	int flags = IMG_INIT_PNG;
	int inited = IMG_Init(flags);
	if ((inited & flags) != flags)
	{
		throw std::runtime_error(IMG_GetError());
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

	m_sdl_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_sdl_renderer == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_sdl_target_texture = SDL_CreateTexture(
		m_sdl_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);
	if (m_sdl_target_texture == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	set_target_framerate(60);
#if __ANDROID__
	// <===
	SDL_Log("__ANDROID__");
	JNIEnv *env = (JNIEnv*)SDL_AndroidGetJNIEnv();
	SDL_Log("env %p", env);
	jobject activity = (jobject)SDL_AndroidGetActivity();
	SDL_Log("activity %p", activity);
	jclass clazz(env->GetObjectClass(activity));
	SDL_Log("clazz %p", clazz);

	//<=== Print cacheDir
	jmethodID getCacheDirId = env->GetMethodID (clazz, "getCacheDir", "()Ljava/io/File;");
	SDL_Log("getCacheDirId %p", getCacheDirId);
	jobject appCacheDirFO = env->CallObjectMethod(activity, getCacheDirId);
	SDL_Log("appCacheDirFO %p", appCacheDirFO);
	jclass fileClass = env->FindClass("java/io/File");
	SDL_Log("fileClass %p", fileClass);
	jmethodID getAbsolutePathId = env->GetMethodID(fileClass, "getAbsolutePath", "()Ljava/lang/String;");
	SDL_Log("getAbsolutePathId %p", getAbsolutePathId);
	jstring appCacheDirJs = (jstring)env->CallObjectMethod(appCacheDirFO, getAbsolutePathId);
	SDL_Log("appCacheDirJs %p", appCacheDirJs);

	char buffer[BUFSIZ];
	SDL_Log("Current dir %s", getCurrentDir(buffer, sizeof(buffer)/sizeof(*buffer)));
	const char* appCacheDir = env->GetStringUTFChars(appCacheDirJs, NULL);
	SDL_Log("appCacheDir %s", appCacheDir);

	strcpy(buffer, appCacheDir);

	env->ReleaseStringUTFChars(appCacheDirJs, appCacheDir);
	SDL_Log("appCacheDir released");
	//===> Print cacheDir

	//<== Change dir
	SDL_Log("isPathExist %i", isPathExist(buffer));
	changeDir(buffer);
	SDL_Log("Current dir %s", getCurrentDir(buffer, sizeof(buffer)/sizeof(*buffer)));
	//==> Change dir

#if __ANDROID_API__ > 8
    jmethodID getAssetsId = env->GetMethodID(clazz, "getAssets", "()Landroid/content/res/AssetManager;");
    SDL_Log("getAssetsId %p", getAssetsId);
    jobject assetManager = env->CallObjectMethod(activity, getAssetsId); // activity.getAssets();
    SDL_Log("assetManager %p", assetManager);

    //jobject localAssetManager = env->NewLocalRef(assetManager);

    AAssetManager* pAssetManager = AAssetManager_fromJava(env, assetManager);//localAssetManager);

    SDL_Log("Before unpackResourcesFromApk");
    unpackResourcesFromApk(pAssetManager);
    SDL_Log("After unpackResourcesFromApk");

    env->DeleteLocalRef(assetManager);
    //env->DeleteLocalRef(localAssetManager);
#endif

	env->DeleteLocalRef(activity);
	env->DeleteLocalRef(clazz);

	SDL_Log("__ANDROID__");
	// ===>
#endif
}

app::~app()
{
	// Clean up FreeType library
	//

	if (m_freetype_library != nullptr)
	{
		FT_Done_FreeType(m_freetype_library);
		m_freetype_library = nullptr;
	}

	// Clean up SDL library
	//

	if (m_sdl_target_texture != nullptr)
	{
		SDL_DestroyTexture(m_sdl_target_texture);
		m_sdl_target_texture = nullptr;
	}

	if (m_sdl_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_sdl_renderer);
		m_sdl_renderer = nullptr;
	}

	if (m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

	IMG_Quit();

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
		SDL_RenderCopy(m_sdl_renderer, m_sdl_target_texture, nullptr, nullptr);
		SDL_RenderPresent(m_sdl_renderer);

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
			m_last_fps = frames_accumulator;
			time_accumulator = 0;
			frames_accumulator = 0;
		}
	}

	return 0;
}

FT_Library app::get_freetype_library() const
{
	return m_freetype_library;
}

unsigned int app::get_last_fps() const
{
	return m_last_fps;
}

app const* app::get_instance()
{
	return _instance;
}

texture& app::get_target_texture()
{
	return m_target_texture;
}

renderer& app::get_renderer()
{
	return m_renderer;
}

void app::on_key_down(SDL_Keysym const)
{
	// Does not handle any key by default
}

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
