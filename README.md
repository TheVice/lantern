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

* ~~[SDL2](http://www.libsdl.org/download-2.0.php) - used for creating windows, copying resulting texture data to a screen, handling input and system events~~
* ~~[SDL2_Image](https://www.libsdl.org/projects/SDL_image/) - used for loading images files~~
* [FreeType](http://www.freetype.org/download.html) - used for loading truetype fonts, calculating their metrics and rendering glyphs bitmaps

###Building for Android

Lantern uses [Gradle](http://gradle.org/) as its build system.

* Set environment variable FREETYPEDIR path to source of FreeType library
* Run [Android Studio](http://developer.android.com/tools/studio/index.html) from console(terminal) where you set environment variable

###Known issues
* Second run of app on device will fail
