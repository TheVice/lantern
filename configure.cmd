
@echo off

if [%1] == [] goto using
if %1 == clean goto CLEAN_BINARIES

cmake > nul
if %ERRORLEVEL% NEQ 0 goto NO_CMAKE_IN_PATH

cmake -version 2>&1 | findstr /C:"version 3.3" > nul && goto CMAKE33
cmake -version 2>&1 | findstr /C:"version 3.2" > nul && goto CMAKE32
cmake -version 2>&1 | findstr /C:"version 3.1" > nul && goto CMAKE31
cmake -version 2>&1 | findstr /C:"version 3.0" > nul && goto CMAKE30
cmake -version 2>&1 | findstr /C:"version 2.8" > nul && goto CMAKE28

goto UNKNOWN_CMAKE_VERSION

:CMAKE33
echo Found cmake v3.3
set CMAKE_VER=cmake-3.3
goto CMAKE_VERSION_DETECTED
:CMAKE32
echo Found cmake v3.2
set CMAKE_VER=cmake-3.2
goto CMAKE_VERSION_DETECTED
:CMAKE31
echo Found cmake v3.1
set CMAKE_VER=cmake-3.1
goto CMAKE_VERSION_DETECTED
:CMAKE30
echo Found cmake v3.0
set CMAKE_VER=cmake-3.0
goto CMAKE_VERSION_DETECTED
:CMAKE28
echo Found cmake v2.8
set CMAKE_VER=cmake-2.8
goto CMAKE_VERSION_DETECTED

:UNKNOWN_CMAKE_VERSION
echo "*****************************************************************************"
echo                       Warning: Unknown CMAKE version
echo "*****************************************************************************"
set CMAKE_VER=cmake-XX

:CMAKE_VERSION_DETECTED
if %1 == MinGW goto MinGW
if %1 == MSVC19 goto MSVC19
if %1 == MSVC18 goto MSVC18

goto using

:MSVC18
echo "MSVC18 (Visual Studio 2013)"

set TOOL="MSVC18"
if NOT %CMAKE_VER% == cmake-2.8 goto MSVC18_4_CMAKE3
set CMAKE_GENERATOR="Visual Studio 12"

goto MSVC

:MSVC18_4_CMAKE3
set CMAKE_GENERATOR="Visual Studio 12 2013"
goto MSVC

:MSVC19
if CMAKE_VER == cmake-2.8 goto using
if CMAKE_VER == cmake-3.0 goto using
echo "MSVC19 (Visual Studio 2015)"

set TOOL="MSVC19"
set CMAKE_GENERATOR="Visual Studio 14 2015"

goto MSVC

:MSVC
set CURRENT_DIR=%CD%
if not exist %CURRENT_DIR%\build mkdir %CURRENT_DIR%\build
if not exist %CURRENT_DIR%\build\%CMAKE_VER% mkdir %CURRENT_DIR%\build\%CMAKE_VER%

if not exist %CURRENT_DIR%\build\%CMAKE_VER%\Win32 mkdir %CURRENT_DIR%\build\%CMAKE_VER%\Win32
if not exist %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL% mkdir %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%
if not exist %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%\Ansi mkdir %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%\Ansi

cd %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%\Ansi
echo "*****************************************************************************"
cmake %CURRENT_DIR%\. -G %CMAKE_GENERATOR%
echo "*****************************************************************************"

cd %CURRENT_DIR%
goto end

:MinGW
gcc -dumpversion > nul
if %ERRORLEVEL% NEQ 0 goto NO_GCC_IN_PATH
for /f "delims=" %%a in ('gcc -dumpversion') do @set MIGW_VERSION=%%a
echo "MinGW_%MIGW_VERSION%"
set TOOL="MinGW_%MIGW_VERSION%"
set CMAKE_GENERATOR="Eclipse CDT4 - MinGW Makefiles"

set CURRENT_DIR=%CD%
if not exist %CURRENT_DIR%\build mkdir %CURRENT_DIR%\build
if not exist %CURRENT_DIR%\build\%CMAKE_VER% mkdir %CURRENT_DIR%\build\%CMAKE_VER%
if not exist %CURRENT_DIR%\build\%CMAKE_VER%\Win32 mkdir %CURRENT_DIR%\build\%CMAKE_VER%\Win32
if not exist %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL% mkdir %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%
if not exist %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%\Debug mkdir %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%\Debug
if not exist %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%\Release mkdir %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%\Release
if not exist %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%\RelWithDebInfo mkdir %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%\RelWithDebInfo

cd %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%\Debug
echo "*****************************************************************************"
cmake %CURRENT_DIR%\. -G %CMAKE_GENERATOR% -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j%NUMBER_OF_PROCESSORS% -s"
echo "*****************************************************************************"

cd %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%\Release
echo "*****************************************************************************"
cmake %CURRENT_DIR%\. -G %CMAKE_GENERATOR% -DCMAKE_BUILD_TYPE=Release -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j%NUMBER_OF_PROCESSORS% -s"
echo "*****************************************************************************"

cd %CURRENT_DIR%\build\%CMAKE_VER%\Win32\%TOOL%\RelWithDebInfo
echo "*****************************************************************************"
cmake %CURRENT_DIR%\. -G %CMAKE_GENERATOR% -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j%NUMBER_OF_PROCESSORS% -s"
echo "*****************************************************************************"

cd %CURRENT_DIR%
goto end

:NO_CMAKE_IN_PATH
echo "*****************************************************************************"
echo Path to cmake bin directory not in the PATH enviroment variable
echo "*****************************************************************************"
goto end

:NO_GCC_IN_PATH
echo "*****************************************************************************"
echo Path to MinGW or MinGW-w64 bin directory not in the PATH enviroment variable
echo "*****************************************************************************"
goto end

:CLEAN_BINARIES
set CURRENT_DIR=%CD%
del /s /a %CURRENT_DIR%\bin\*.a %CURRENT_DIR%\bin\*.lib %CURRENT_DIR%\bin\*.exe %CURRENT_DIR%\bin\CMakeCache.txt
goto end

:using
echo "*****************************************************************************"
echo "%0 using with clean|MinGW|MSVC18|MSVC19"
echo clean for Deleting binaries files (*.a *.lib *.exe) from bin directory
echo MinGW for Eclipse project with Minimalist GNU for Windows
echo MSVC18 for Visual Studio 2013
echo MSVC19 for Visual Studio 2015 (only for cmake 3.1 and above)
echo "*****************************************************************************"
goto end

:end
