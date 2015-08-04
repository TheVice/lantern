lantern
=======

Software renderer written for educational purposes. Uses OpenGL for rendering and Google C++ Testing Framework for tests.

###Building on Windows (MinGW)
* Download [PicoPng](http://lodev.org/lodepng/picopng.cpp)
* If you want to build tests you must download Google C++ Testing Framework source code ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list))
* Open cmd.exe, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```set PICO_PNG_CPP=<path to picopng.cpp>``` so cmake include this file into build process
* Run: ```set GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target
* Run: ```cmake -G "MinGW Makefiles" ..``` to generate makefile
* Run: ```mingw32-make lantern``` to build library
* Run: ```mingw32-make rasterized_triangle_app``` to build example application
* Run: ```mingw32-make tests``` to build tests executable

###Building on Windows (Visual Studio 2013 / 2015)
* Download [PicoPng](http://lodev.org/lodepng/picopng.cpp)
* If you want to build tests you must download Google C++ Testing Framework source code ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list))
* Open cmd.exe, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```set PICO_PNG_CPP=<path to picopng.cpp>``` so cmake include this file into build process
* Run: ```set GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target
* Run: ```cmake -G "Visual Studio 12" ..``` for VS 2013 (or ```cmake -G "Visual Studio 14" ..``` for VS 2015) to generate VS solution
* Visual Studio solution is ready now

###Building on Linux
* Download [PicoPng](http://lodev.org/lodepng/picopng.cpp)
* If you want to build tests you must download Google C++ Testing Framework ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list) or just ```sudo apt-get install libgtest-dev``` for apt)
* Open terminal, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```export PICO_PNG_CPP=<path to picopng.cpp>``` so cmake include this file into build process
* Run: ```export GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target. If you installed it via apt the path is ```/usr/src/gtest```
* Run: ```cmake -G "Unix Makefiles" ..``` to generate makefile
* Run: ```make lantern``` to build library
* Run: ```make rasterized_triangle_app``` to build example application
* Run: ```make tests``` to build tests executable

###Building on Mac OS X
* Download [PicoPng](http://lodev.org/lodepng/picopng.cpp)
* If you want to build tests you must download Google C++ Testing Framework ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list))
* Open terminal, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```export PICO_PNG_CPP=<path to picopng.cpp>``` so cmake include this file into build process
* Run: ```export GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target
* Run: ```cmake -G Xcode ..``` to generate project
* XCode project is ready now

###Known issues
* Lantern might fail loading resources when running from directory different then the one where the executable is
