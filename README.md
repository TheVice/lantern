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
* [Google C++ Tests](https://github.com/google/googletest/releases) - used for testing

###Building for Windows and Linux

Lantern uses CMake as its build system.

Simple scripts `configure.cmd` and `configure.sh` were created to simplify building process a little, though (output goes to `<lantern>/build` folder). Using:
* `configure.sh Android` - creates Eclipse project for building Android apks (on Linux host)
* `configure.cmd Android` - creates Eclipse project for building Android apks (on Windows host)
* `configure.cmd MSVC18` - creates VS solution for Visual Studio 2013
* `configure.cmd MSVC19` - creates VS solution for Visual Studio 2015 (only for CMake 3.1 and above)
* `configure.cmd MinGW` - creates Eclipse project (on Windows host
* `configure.sh gcc` - creates Eclipse project (on Linux host)

On Linux you probably have to `chmod +x configure.sh` before running them or run it directly by bash: `bash configure.sh gcc` or `bash configure.sh Android`

You also have to have all the dependencies and tools installed (obviously). As an example for Ubuntu, here are the commands for installing all of them (just skip what you don't need):
 * `sudo apt-get install cmake`
 * `sudo apt-get install g++`
 * `sudo apt-get install libsdl2-dev`
 * `sudo apt-get install libsdl2-image-dev`
 * `sudo apt-get install libfreetype6-dev`
 * `sudo apt-get install libgtest-dev`

On Windows you have to specify a few environment variables for dependencies so that CMake's `find_package` will be able to find them:
 * `SDL2DIR` - path to SDL2 development library or source of this library
 * `SDL2IMAGEDIR` - path to SDL2_Image development library or source of this library
 * `FREETYPEDIR` - path to a folder containing FreeType's headers and library or source of this library
 * `GTEST_ROOT` - path to Google Tests source code folder (required only if you're going to build tests target)

On Linux it also required to set `GTEST_ROOT` path to Google Tests source code folder if you're going to build tests target

###Building for Android

##Build requires
* Download source of SDL2 ([https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php)), SDL2Image ([https://www.libsdl.org/projects/SDL_image/](https://www.libsdl.org/projects/SDL_image/)) and [FreeType](http://www.freetype.org/download.html)
* Install\unpack [Java Platform (JDK)](http://www.oracle.com/technetwork/java/javase/downloads/index.html) version 6 or above
* Unpack [Apache Ant](http://ant.apache.org/bindownload.cgi) (Only for build through CMake/Eclipse)
* Unpack [Android SDK](http://developer.android.com/sdk/index.html). It is also may require to download some addition package via SDK Manager
* Unpack [Android NDK](http://developer.android.com/tools/sdk/ndk/index.html)
* Add bin directory from JDK (not from JRE) into PATHs
* Set environment variable ```ANDROID_HOME``` for Android SDK unpacked path
* Set environment variable ```ANDROID_NDK``` for Android NDK unpacked path. It is highly recommended that path do not consist any space symbols
* Add bin directory from Apache Ant into PATHs (Only for build through CMake/Eclipse)
* Add directories ```ANDROID_NDK```, ```ANDROID_HOME/platform-tools``` and ```ANDROID_HOME/tools``` into PATHs
* Get [android.toolchain.cmake](https://github.com/taka-no-me/android-cmake/blob/master/android.toolchain.cmake) (mirror [1](https://code.google.com/p/android-cmake/source/browse/toolchain/android.toolchain.cmake), [2](http://code.opencv.org/projects/opencv/repository/revisions/master/changes/platforms/android/android.toolchain.cmake)) and save to any place you want
* Set to ```ANDROID_TOOLCHAIN``` full path to ```android.toolchain.cmake```
* Device or emulator that running [Android 3.1.x (API Level 12)](https://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels) or above

##Prepare Eclipse project
* Open terminal, move to lantern folder
* Run: ```set SDL2SRCDIR=<path to source of SDL2>``` on Windows or ```export SDL2SRCDIR=<path to source of SDL2>``` on Linux
* Run: ```set SDL2IMAGESRCDIR=<path to source of SDL2 image>``` on Windows or ```export SDL2IMAGESRCDIR=<path to source of SDL2 image>``` on Linux
* Run: ```set FREETYPESRCDIR=<path to source of FreeType>``` on Windows or ```export FREETYPESRCDIR=<path to source of FreeType>``` on Linux
* On Windows execute `configure.cmd Android`
* On Linux execute `configure.sh Android`
* Connect your Android device, start emulator by ```android avd``` or ```emulator -avd <avd_name>``` to run already created emulator directly
* It is also possible to run [Android x86](http://www.android-x86.org/download) on hypervisor, see [instuction](http://www.android-x86.org/documents/debug-howto) for detail
* Make sure ```adb devices``` see your Android device/emulator/OS on virtual
* Execute ```mingw32-make -j%NUMBER_OF_PROCESSORS% -s rasterized_triangle_app_run-apk``` on Windows or ```make -j$(nproc) -s rasterized_triangle_app_run-apk``` on Linux to run rasterized_triangle_app on Android
* Execute ```mingw32-make -j%NUMBER_OF_PROCESSORS% -s run_tests``` on Windows or ```make -j$(nproc) -s run_tests``` on Linux to run tests on Android
* You can also do the same from Eclipse IDE after import already exist project from ```build``` directory

###Known issues

* If you're facing linking problems in SDL2main library on VS 2015, you can recompile SDL2 using VS 2015 - just download source package and after unpack source directory set to `SDL2DIR` path to that folder, just the same you do for binary package, or just download SDL2 build both package here: https://buildbot.libsdl.org/sdl-builds/sdl-visualstudio/
* Lantern fails loading resources when running from a directory different than the one where the executable is
