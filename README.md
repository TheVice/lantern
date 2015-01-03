lantern
=======

Software renderer written for educational purposes. Uses OpenGL for rendering and Google C++ Testing Framework for tests.

###Building on Windows (MinGW)
* Download SDL ([https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php), select MinGW version of development library). If you want to build tests you must also download Google C++ Testing Framework source code ([https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list))
* Open cmd.exe, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```set SDL2DIR=<path to SDL2>```. Note that it should be either i686-w64-mingw32 (for 32-bit) or x86_64-w64-mingw32 (for 64-bit) folder
* Run: ```set GTEST_ROOT=<path to gtest src>```. You need this only if you want to build tests target
* Run: ```cmake -G "MinGW Makefiles" ..``` to generate makefile
* Run: ```mingw32-make lantern``` to build library
* Run: ```mingw32-make rotating_car_app``` to build example application
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
* Open terminal, move to lantern folder* Run: ```mkdir build && cd build``` to create build folder and move to it* If you want to build tests target, you must install Google Testing Framework (https://code.google.com/p/googletest/downloads/list)* Run: ```cmake -G "Unix Makefiles" ..``` to generate makefile
* Run: ```make lantern``` to build library
* Run: ```make rotating_car_app``` to build example application
* Run: ```make tests``` to build tests executable

###Building on Windows (using MinGW)
* If you want to build tests target, you must also install Google Testing Framework (https://code.google.com/p/googletest/downloads/list) and pthreads library (you can use MinGW Installation Manager for this one)
* Make sure your PATH environment variable contains MinGW's bin folder - CMake needs it
* Run cmd.exe
* Move to your build folder
* Run: ```<path_to_cmake_bin_folder>/cmake.exe -G "MinGW Makefiles" <path_to_lantern_source_folder>``` to generate makefile
* Run: ```mingw32-make lantern``` to build library
* Run: ```mingw32-make rotating_car_app``` to build example application
* Run: ```mingw32-make tests``` to build tests executable

###Building from Eclipse (on both platforms)
* On steps of calling cmake in instuctions above you can call:
* ```cmake .. -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$(nproc) -s"``` on Linux
* ```cmake .. -G "Eclipse CDT4 - MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j%NUMBER_OF_PROCESSORS% -s"``` on Windows
* So now you can browse project to Eclipse and make and run targets from it.

###Building on Windows (using Visual Studio 2013)
* Run cmd.exe
* Move to your build folder
* Run: ```cmake -G "Visual Studio 12" <path_to_lantern_source_folder>``` to generate VS solution
* If you want to build tests target, download Google Testing Framework (https://code.google.com/p/googletest/downloads/list). Build it using solution inside of msvc folder. Note that if you want to build tests target in release configuration, then gtest library must also be built with release configuration. Then copy include files to <your_build_folder>/dependencies/include/gtest/. Copy lib file to <your_build_folder>/dependencies/lib/
* VS solution is ready now
* Also you can build all targets from console via MSBuild:
* ```"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" lantern.sln /maxcpucount:%NUMBER_OF_PROCESSORS% /target:lantern /property:Configuration=Debug```
* ```"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" lantern.sln /maxcpucount:%NUMBER_OF_PROCESSORS% /target:empty_app /property:Configuration=Debug```
* ```"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" lantern.sln /maxcpucount:%NUMBER_OF_PROCESSORS% /target:rotating_car_app /property:Configuration=Debug```
* ```"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" lantern.sln /maxcpucount:%NUMBER_OF_PROCESSORS% /target:tests /property:Configuration=Debug```
* Or to build all targets above ```"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" lantern.sln /maxcpucount:%NUMBER_OF_PROCESSORS%```

###Building on Windows (using Visual Studio 2013 Command Prompt)
* Run development console
* Move to your source directory
* Run msvc.bat
* You got
+---build
¦   L---MSVC
¦       +---examples
¦       ¦   +---empty_app
¦       ¦   L---rotating_car_app
¦       ¦       L---resources
¦       +---lib
¦       +---obj
¦       L---tests
¦           L---resources

You can also use CLion EAP (http://confluence.jetbrains.com/display/CLION/Early+Access+Program) for both platforms.
