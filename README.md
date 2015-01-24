lantern
=======

Software renderer written for educational purposes. Uses OpenGL for rendering and Google C++ Testing Framework for tests.

###Building on Linux
* If you want to build tests target, you must extract Google Testing Framework (https://code.google.com/p/googletest/downloads/list) into source directory
* Open terminal, move to lantern folder
* Run: ```mkdir build && cd build``` to create build folder and move to it
* Run: ```cmake -G "Unix Makefiles" ..``` to generate makefile
* Run: ```make -C . lantern -j$(nproc) -s``` to build library
* Run: ```make -C . rasterized_triangle_app -j$(nproc) -s``` to build example application
* Run: ```make -C . tests -j$(nproc) -s``` to build tests executable
* Or run: ```make -C . -j$(nproc) -s``` to build all targets

###Building on Windows (using MinGW)
* If you want to build tests target, you must extract Google Testing Framework (https://code.google.com/p/googletest/downloads/list) into source directory
* Make sure your PATH environment variable contains MinGW's bin folder - CMake needs it
* Run cmd.exe
* Move to your build folder
* Run: ```<path_to_cmake_bin_folder>/cmake.exe -G "MinGW Makefiles" <path_to_lantern_source_folder>``` to generate makefile
* Run: ```mingw32-make -C . lantern -j%NUMBER_OF_PROCESSORS% -s``` to build library
* Run: ```mingw32-make -C . rotating_car -j%NUMBER_OF_PROCESSORS% -s``` to build example application
* Run: ```mingw32-make -C . tests -j%NUMBER_OF_PROCESSORS% -s``` to build tests executable
* Or run: ```mingw32-make -C . -j%NUMBER_OF_PROCESSORS% -s``` to build all targets

###Building from Eclipse (on both platforms)
* On steps of calling cmake in instuctions above you can call:
* ```cmake .. -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$(nproc) -s"``` on Linux
* ```cmake .. -G "Eclipse CDT4 - MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j%NUMBER_OF_PROCESSORS% -s"``` on Windows
* So now you can add existing project into Eclipse workspace to make and run targets from it.
* CMAKE_BUILD_TYPE can be also set to Release or RelWithDebInfo

###Building on Windows (using Visual Studio 2013)
* If you want to build tests target, you must extract Google Testing Framework (https://code.google.com/p/googletest/downloads/list) into source directory
* Run cmd.exe
* Move to your build folder
* Run: ```cmake -G "Visual Studio 12" <path_to_lantern_source_folder>``` to generate VS solution
* VS solution is ready now
* Also you can build all targets from console via MSBuild:
* ```"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" lantern.sln /maxcpucount:%NUMBER_OF_PROCESSORS% /target:lantern /property:Configuration=Debug /property:Platform=Win32```
* ```"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" lantern.sln /maxcpucount:%NUMBER_OF_PROCESSORS% /target:rasterized_triangle_app /property:Configuration=Debug /property:Platform=Win32```
* ```"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" lantern.sln /maxcpucount:%NUMBER_OF_PROCESSORS% /target:tests /property:Configuration=Debug /property:Platform=Win32```
* Or to build all targets above ```"C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" lantern.sln /maxcpucount:%NUMBER_OF_PROCESSORS% /property:Configuration=Debug /property:Platform=Win32```
* If you running on 32-bit (x86) of Windows path ```C:\Program Files (x86)``` changed to ```C:\Program Files```

###Building on Windows (using Visual Studio 2013 Command Prompt)
* Run ```Developer Command Prompt for VS2013``` from your Start Menu\Window
* Move to your source directory
* Run msvc.bat
* You got
+---build
¦   L---MSVC
¦       +---examples
¦       ¦   +---empty_app
¦       ¦   L---rasterized_triangle_app
¦       ¦       L---resources
¦       +---lib
¦       +---obj
¦       L---tests
¦           L---resources

You can also use CLion EAP (http://confluence.jetbrains.com/display/CLION/Early+Access+Program) for both platforms.
