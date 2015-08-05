lantern
=======

Software renderer written for educational purposes. Uses SDL2 for rendering and Google C++ Testing Framework for tests.

###Building on Windows (MinGW)
* Download SDL ([https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php), select MinGW version of development library). If you want to build tests you must also download Google C++ Testing Framework source code ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list))
* Open cmd.exe, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```set SDL2DIR=<path to SDL2>```. Note that it should be either i686-w64-mingw32 (for 32-bit) or x86_64-w64-mingw32 (for 64-bit) folder
* Run: ```set GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target
* Run: ```cmake -G "MinGW Makefiles" ..``` to generate makefile
* Run: ```mingw32-make lantern``` to build library
* Run: ```mingw32-make rasterized_triangle_app``` to build example application
* Run: ```mingw32-make tests``` to build tests executable

###Building on Windows (Visual Studio 2013)
* Download SDL ([https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php), select VC++ version of development library). If you want to build tests you must also download Google C++ Testing Framework source code ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list))
* Open cmd.exe, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```set SDL2DIR=<path to SDL2>```
* Run: ```set GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target
* Run: ```cmake -G "Visual Studio 12" ..``` to generate VS solution
* Visual Studio solution is ready now

###Building on Linux
* Install SDL2 ([https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php), or just ```sudo apt-get install libsdl2-dev``` for apt). If you want to build tests you must also download Google C++ Testing Framework ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list) or just ```sudo apt-get install libgtest-dev``` for apt)
* Open terminal, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```export GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target. If you installed it via apt the path is /usr/src/gtest
* Run: ```cmake -G "Unix Makefiles" ..``` to generate makefile
* Run: ```make lantern``` to build library
* Run: ```make rasterized_triangle_app``` to build example application
* Run: ```make tests``` to build tests executable

###Building on Mac OS X
* Install SDL2 ([https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php)) to ```/Library/Frameworks/``` or ```~/Library/Frameworks/```. If you want to build tests you must also download Google C++ Testing Framework ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list))
* Open terminal, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```export GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target
* Run: ```cmake -G Xcode ..``` to generate project
* XCode project is ready now

###Building for Android

Uses [OpenGL ES 1.X](https://www.khronos.org/opengles/1_X/) for rendering and [Google C++ Testing Framework](https://code.google.com/p/googletest) for tests.

##Build requires
* Install\unpack [Java Platform (JDK)](http://www.oracle.com/technetwork/java/javase/downloads/index.html) version 6 or above
* Unpack [Apache Ant](http://ant.apache.org/bindownload.cgi)
* Unpack [Android SDK](http://developer.android.com/sdk/index.html)
* It is also may require to download some addition package via SDK Manager
* Unpack [Android NDK](http://developer.android.com/tools/sdk/ndk/index.html)
* Add bin directory from JDK (not from JRE) into PATHs
* Set environment variable ```ANDROID_HOME``` for Android SDK unpacked path
* Set environment variable ```ANDROID_NDK``` for Android NDK unpacked path. It is highly recommended that path do not consist any space symbols
* Add bin directory from Apache Ant into PATHs
* Add directories ```ANDROID_NDK```, ```ANDROID_HOME/platform-tools``` and ```ANDROID_HOME/tools``` into PATHs
* Get [android.toolchain.cmake](https://github.com/taka-no-me/android-cmake/blob/master/android.toolchain.cmake) (mirror [1](https://code.google.com/p/android-cmake/source/browse/toolchain/android.toolchain.cmake), [2](http://code.opencv.org/projects/opencv/repository/revisions/master/changes/platforms/android/android.toolchain.cmake)) and save to any place you want
* Device or emulator that running [Android 2.3 (API Level 9)](https://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels) or above

##Prepare Eclipse project
* Open terminal, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* On Windows execute ```cmake .. -G "Eclipse CDT4 - MinGW Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j%NUMBER_OF_PROCESSORS% -s" -DCMAKE_TOOLCHAIN_FILE="<path to>android.toolchain.cmake" -DLIBRARY_OUTPUT_PATH_ROOT="." -DANDROID_NATIVE_API_LEVEL="9" -DANDROID_ABI="armeabi"```
* On Linux execute ```cmake .. -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$(nproc) -s" -DCMAKE_TOOLCHAIN_FILE="<path to>android.toolchain.cmake" -DLIBRARY_OUTPUT_PATH_ROOT="." -DANDROID_NATIVE_API_LEVEL="9" -DANDROID_ABI="armeabi"```
* ```CMAKE_BUILD_TYPE``` also can be set to ```Release``` or ```RelWithDebInfo```. Tested in ```Debug``` mode
* ```ANDROID_NATIVE_API_LEVEL``` can be great than ```9```, but not lower. Tested only with ```9```
* ```ANDROID_ABI``` dependence on NDK version can be set to: ```armeabi-v7a```, ```x86```, ```mips``` and also 64 version of various CPU architecture. With set ```armeabi``` almost all ARM devices emulator can to execute. Check your emulator setting, device configuration to determinate what type of CPU architecture you have
* Connect your Android device, start emulator by ```android avd``` or ```emulator -avd <avd_name>``` to run already created emulator directly
* It is also possible to run [Android x86](http://www.android-x86.org/download) on hypervisor, see [instuction](http://www.android-x86.org/documents/debug-howto) for detail
* Make sure ```adb devices``` see your Android device/emulator/OS on virtual
* Execute ```mingw32-make -j%NUMBER_OF_PROCESSORS% -s rasterized_triangle_app[run-apk]``` on Windows or ```make -j$(nproc) -s rasterized_triangle_app[run-apk]``` on Linux to run rasterized_triangle_app on Android
* Execute ```mingw32-make -j%NUMBER_OF_PROCESSORS% -s run_tests``` on Windows or ```make -j$(nproc) -s run_tests``` on Linux to run tests on Android
* You can also do the same from Eclipse IDE after import already exist project from ```build``` directory

##Prepare Android Studio project
* Download [Android Studion 1.3](http://tools.android.com/download/studio/canary/1-3) or above
* Download [gradle 2.5](https://services.gradle.org/distributions/gradle-2.5-all.zip) or above and unpack it into ```<path to android-studio>/gradle``` near with ```gradle-2.4```. Note: if folder gradle-2.5 exist, for example with later release of Android Studio, consult documentation, may be you do not need to do this step
* Open in Android Studio sub folder android-studio in lantern folder
* When IDE ask to download or not gradle press Cancel and set manual path to ```<path to android-studio>/gradle/gradle-2.5```. Note: in later release of Android Studio gradle version may be upper
* Now you able to work with project