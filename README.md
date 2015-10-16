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

* [SDL2](http://www.libsdl.org/download-2.0.php) - used for creating windows, copying resulting texture data to a screen, handling input and system events
* [SDL2_Image](https://www.libsdl.org/projects/SDL_image/) - used for loading images files
* [FreeType](http://www.freetype.org/download.html) - used for loading truetype fonts, calculating their metrics and rendering glyphs bitmaps

###Building

Lantern uses [Gradle](http://gradle.org/) as its build system.

Simple scripts were created by `gradle wrapper` command to simplify building process a little, though (output goes to `<lantern>/build` folder):
* `gradlew` - for processing project tasks on Unix like OS
* `gradlew.bat` - for processing project tasks on Windows OS

On Linux and MacOS you probably have to `chmod +x gradlew` before running they.

You also have to have all the dependencies and tools installed (obviously). As an example for Ubuntu, here are the commands for installing all of them (just skip what you don't need):
 * `sudo apt-get install g++`
 * `sudo apt-get install libsdl2-dev`
 * `sudo apt-get install libsdl2-image-dev`
 * `sudo apt-get install libfreetype6-dev`
 * `sudo apt-get install libgtest-dev`

~~On Windows you have to specify a few environment variables for dependencies so that Gradle will be able to find them:~~
 * ~~`SDL2SRCDIR` - path to SDL2 development library~~
 * ~~`SDL2IMAGESRCDIR` - path to SDL2_Image development library~~
 * ~~`FREETYPESRCDIR` - path to a folder containing FreeType's headers and library~~

* For build lantern library type and execute: `gradlew lanternStaticLibrary --daemon -PSDL2SRCDIR=<Path to SDL2 source directory> -PSDL2IMAGESRCDIR=<Path to SDL2 image source directory> -PFREETYPESRCDIR=<Path to FreeType source directory>`
* For list all avaliable tasks of this project type and execute: `gradlew tasks --daemon -PSDL2SRCDIR=<Path to SDL2 source directory> -PSDL2IMAGESRCDIR=<Path to SDL2 image source directory> -PFREETYPESRCDIR=<Path to FreeType source directory>`

Please note `daemon` switch is optional and only provide timesaving in regular building at developing process.
