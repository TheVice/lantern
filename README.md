Lantern is a cross-plaftorm 3D software renderer written for educational purposes. It depends of multiple libraries (full list is given below) but it doesn't use them for rendering (except using FreeType library for getting glyphs bitmaps), only for common tasks (such as presenting color buffer on a screen, handling input, loading textures from disk etc.). The aim is to create feature-full renderer from scratch, only using ability to set texture's pixel color.

Note that because it doesn't use GPU, it is much slower comparing to DirectX or OpenGL, and, obviously, is not intended for using in real projects (unless you're targeting users without video cards!).

###Implemented features

* Loading .obj files
* Rasterization using three different algorithms: inversed slope, traversal (and its subtypes: aabb, backtracking, zigzag), homogeneous
* Programmable vertex and pixel shaders
* Perspective-correct attributes interpolation
* Texture mapping
* Alpha-blending
* Truetype fonts rendering

###Dependencies

* SDL2 - used for creating windows, copying resulting texture data to a screen, handling input and system events
* SDL2_Image - used for loading images files
* FreeType - used for loading truetype fonts, calculating their metrics and rendering glyphs bitmaps
* Google C++ Tests - used for testing

###Building

Lantern uses CMake as its build system.

Simple scripts `configure.cmd` and `configure.sh` were created to simplify building process a little, though (output goes to `<lantern>/build` folder). Using:
* `configure.cmd MSVC18` - creates VS solution for Visual Studio 2013
* `configure.cmd MSVC19` - creates VS solution for Visual Studio 2015
* `configure.cmd MinGW` - creates MinGW makefiles
* `configure.sh gcc` - creates Linux makefiles

On Linux you probably have to `chmod +x configure.sh` before running them or run it directly by bash: `bash configure.sh gcc`

You also have to have all the dependencies and tools installed (obviously). As an example for Ubuntu, here are the commands for installing all of them (just skip what you don't need):
 * `sudo apt-get install cmake`
 * `sudo apt-get install g++`
 * `sudo apt-get install libsdl2-dev`
 * `sudo apt-get install libsdl2-image-dev`
 * `sudo apt-get install libfreetype6-dev`
 * `sudo apt-get install libgtest-dev`

On Windows you have to specify a few environment variables for dependencies so that CMake's `find_package` will be able to find them:
 * `SDL2DIR` - path to SDL2 development library or source of this library (only for Visual Studio 2015)
 * `SDL2IMAGEDIR` - path to SDL2_Image development library or source of this library (only for Visual Studio)
 * `FREETYPEDIR` - path to a folder containing FreeType's headers and library
 * `GTEST_ROOT` - path to Google Tests source code folder (required only if you're going to build tests target)

~~Note for Windows FreeType library: if you're building it by yourself, make sure that output library's name is `freetype2.lib` and not `freetype26.lib` (that's what bundled FindFreeType.cmake looks for).~~

###Known issues

* If you're facing linking problems in SDL2main library on VS 2015, you can recompile SDL2 using VS 2015 - just download source package and after unpack source directory set to `SDL2DIR` path to that folder, just the same you do for binary package, or just download SDL2 build both package here: https://buildbot.libsdl.org/sdl-builds/sdl-visualstudio/

* Lantern fails loading resources when running from a directory different than the one where the executable is
