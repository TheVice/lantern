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
* Device or emulator that running [Android 3.1.x (API Level 12)](https://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels) or above

##Prepare Eclipse project
* Open terminal, move to lantern folder
* Run: ```set SDL2SRCDIR=<path to source of SDL2>``` on Windows or ```export SDL2SRCDIR=<path to source of SDL2>``` on Linux
* Run: ```set SDL2IMAGESRCDIR=<path to source of SDL2 image>``` on Windows or ```export SDL2IMAGESRCDIR=<path to source of SDL2 image>``` on Linux
* Run: ```set FREETYPESRCDIR=<path to source of FreeType>``` on Windows or ```export FREETYPESRCDIR=<path to source of FreeType>``` on Linux
* Run: ```mkdir build && cd build``` to create build folder and move to it
* On Windows execute ```cmake .. -G "Eclipse CDT4 - MinGW Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j%NUMBER_OF_PROCESSORS% -s" -DCMAKE_TOOLCHAIN_FILE="<path to>android.toolchain.cmake" -DLIBRARY_OUTPUT_PATH_ROOT="." -DANDROID_NATIVE_API_LEVEL="12" -DANDROID_ABI="armeabi"```
* On Linux execute ```cmake .. -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$(nproc) -s" -DCMAKE_TOOLCHAIN_FILE="<path to>android.toolchain.cmake" -DLIBRARY_OUTPUT_PATH_ROOT="." -DANDROID_NATIVE_API_LEVEL="12" -DANDROID_ABI="armeabi"```
* ```CMAKE_BUILD_TYPE``` also can be set to ```Release``` or ```RelWithDebInfo```. Tested in ```Debug``` mode
* ```ANDROID_NATIVE_API_LEVEL``` can be great than ```12```, but not lower. Tested only with ```12```
* ```ANDROID_ABI``` dependence on NDK version can be set to: ```armeabi-v7a```, ```x86```, ```mips``` and also 64 version of various CPU architecture. With set ```armeabi``` almost all ARM devices emulator can to execute. Check your emulator setting, device configuration to determinate what type of CPU architecture you have
* You can also do not do steps with addeding SDL2SRCDIR, SDL2IMAGESRCDIR and FREETYPESRCDIR to enviroment variable and pass it like defines for cmake script: ```-DSDL2SRCDIR=<path to source of SDL2> -DSDL2IMAGESRCDIR=<path to source of SDL2 image> -DFREETYPESRCDIR=<path to source of FreeType>```
* Connect your Android device, start emulator by ```android avd``` or ```emulator -avd <avd_name>``` to run already created emulator directly
* It is also possible to run [Android x86](http://www.android-x86.org/download) on hypervisor, see [instuction](http://www.android-x86.org/documents/debug-howto) for detail
* Make sure ```adb devices``` see your Android device/emulator/OS on virtual
* Execute ```mingw32-make -j%NUMBER_OF_PROCESSORS% -s rasterized_triangle_app_run-apk``` on Windows or ```make -j$(nproc) -s rasterized_triangle_app_run-apk``` on Linux to run rasterized_triangle_app on Android
* Execute ```mingw32-make -j%NUMBER_OF_PROCESSORS% -s run_tests``` on Windows or ```make -j$(nproc) -s run_tests``` on Linux to run tests on Android
* You can also do the same from Eclipse IDE after import already exist project from ```build``` directory

##Prepare Android Studio project
* ~~Download [Android Studion 1.5](http://tools.android.com/download/studio/builds/1-5-preview-1) or above~~
* ~~If you install and using JDK 8 and above, gradle engine may need [set to compile like 7 version](http://tools.android.com/tech-docs/new-build-system/user-guide#TOC-Using-sourceCompatibility-1.7) or you can install/unpack latest from 7 version of JDK ([Java SE Development Kit 7u80](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html) at the moment when this wrote) and set it like compiler for gradle. Android Studio still can be running under JDK 8 and above~~
* ~~Known issue: in created ```apk file``` at ```assert folder``` may present not only ```resources folder``` but also ```cpp file(s)``` and all files and folders placed in ```examples/<example name>``` at the moment when apk start to create~~

###Known issue
* From Android Studio running of apk on device do not support during some bug at one of developer tools - [experimental gradle plugin doesn't use the right platform version for ndk modules](https://code.google.com/p/android/issues/detail?id=177530)
