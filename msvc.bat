@echo off

set CURRENT_DIR=%CD%
if not exist %CURRENT_DIR%\build mkdir %CURRENT_DIR%\build
if not exist %CURRENT_DIR%\build\MSVC mkdir %CURRENT_DIR%\build\MSVC

::lantern
if not exist %CURRENT_DIR%\build\MSVC\obj mkdir %CURRENT_DIR%\build\MSVC\obj
cd %CURRENT_DIR%\build\MSVC\obj
cl /c /EHsc /W4 /GS /D WIN32 /D _WIN32 /D WINDOWS /D _LIB %CURRENT_DIR%\lantern\src\app.cpp %CURRENT_DIR%\lantern\src\bitmap_painter.cpp %CURRENT_DIR%\lantern\src\camera.cpp %CURRENT_DIR%\lantern\src\color.cpp %CURRENT_DIR%\lantern\src\matrix3x3.cpp %CURRENT_DIR%\lantern\src\matrix4x4.cpp %CURRENT_DIR%\lantern\src\mesh.cpp %CURRENT_DIR%\lantern\src\obj_import.cpp %CURRENT_DIR%\lantern\src\pipeline.cpp %CURRENT_DIR%\lantern\src\point.cpp %CURRENT_DIR%\lantern\src\scene.cpp %CURRENT_DIR%\lantern\src\scene_object.cpp %CURRENT_DIR%\lantern\src\vector3.cpp %CURRENT_DIR%\lantern\src\vector4.cpp /I"%CURRENT_DIR%\lantern\include"
if not exist %CURRENT_DIR%\build\MSVC\lib mkdir %CURRENT_DIR%\build\MSVC\lib
lib app.obj bitmap_painter.obj camera.obj color.obj matrix3x3.obj matrix4x4.obj mesh.obj obj_import.obj pipeline.obj point.obj scene.obj scene_object.obj vector3.obj vector4.obj /out:"%CURRENT_DIR%\build\MSVC\lib\lantern.lib"

if not exist %CURRENT_DIR%\build\MSVC\examples mkdir %CURRENT_DIR%\build\MSVC\examples

::empty_app
if not exist %CURRENT_DIR%\build\MSVC\examples\empty_app mkdir %CURRENT_DIR%\build\MSVC\examples\empty_app
cl /EHsc /W4 /GS /D WIN32 /D _WIN32 /D WINDOWS %CURRENT_DIR%\examples\empty_app\main.cpp kernel32.lib user32.lib gdi32.lib glu32.lib opengl32.lib lantern.lib /I"%CURRENT_DIR%\lantern\include" /link /SafeSEH /DynamicBase /NXCompat /SUBSYSTEM:CONSOLE /LIBPATH:"%CURRENT_DIR%\build\MSVC\lib" /out:"%CURRENT_DIR%\build\MSVC\examples\empty_app\empty_app.exe"

::rotating_car_app
if not exist %CURRENT_DIR%\build\MSVC\examples\rotating_car_app mkdir %CURRENT_DIR%\build\MSVC\examples\rotating_car_app
cl /EHsc /W4 /GS /D WIN32 /D _WIN32 /D WINDOWS /D_USE_MATH_DEFINES %CURRENT_DIR%\examples\rotating_car_app\main.cpp kernel32.lib user32.lib gdi32.lib glu32.lib opengl32.lib lantern.lib /I"%CURRENT_DIR%\lantern\include" /link /SafeSEH /DynamicBase /NXCompat /SUBSYSTEM:CONSOLE /LIBPATH:"%CURRENT_DIR%\build\MSVC\lib" /out:"%CURRENT_DIR%\build\MSVC\examples\rotating_car_app\rotating_car_app.exe"
if not exist %CURRENT_DIR%\build\MSVC\examples\rotating_car_app\resources mkdir %CURRENT_DIR%\build\MSVC\examples\rotating_car_app\resources
copy %CURRENT_DIR%\examples\rotating_car_app\resources\car.obj %CURRENT_DIR%\build\MSVC\examples\rotating_car_app\resources\car.obj /Y

::tests
if not exist %CURRENT_DIR%\build\MSVC\tests mkdir %CURRENT_DIR%\build\MSVC\tests
cl /EHsc /W4 /GS /D WIN32 /D _WIN32 /D WINDOWS /D_USE_MATH_DEFINES %CURRENT_DIR%\gtest-1.7.0\src\gtest-all.cc %CURRENT_DIR%\tests\src\bitmap_painter.cpp %CURRENT_DIR%\tests\src\camera.cpp %CURRENT_DIR%\tests\src\main.cpp %CURRENT_DIR%\tests\src\matrix3x3.cpp %CURRENT_DIR%\tests\src\matrix4x4.cpp %CURRENT_DIR%\tests\src\obj_import.cpp %CURRENT_DIR%\tests\src\vector3.cpp %CURRENT_DIR%\tests\src\vector4.cpp kernel32.lib lantern.lib /I"%CURRENT_DIR%\tests\include" /I"%CURRENT_DIR%\gtest-1.7.0" /I"%CURRENT_DIR%\gtest-1.7.0\include" /I"%CURRENT_DIR%\lantern\include" /link /SafeSEH /DynamicBase /NXCompat /SUBSYSTEM:CONSOLE /LIBPATH:"%CURRENT_DIR%\build\MSVC\lib" /out:"%CURRENT_DIR%\build\MSVC\tests\tests.exe"
if not exist %CURRENT_DIR%\build\MSVC\tests\resources mkdir %CURRENT_DIR%\build\MSVC\tests\resources
copy %CURRENT_DIR%\tests\resources\unit_cube_v.obj %CURRENT_DIR%\build\MSVC\tests\resources\unit_cube_v.obj /Y
copy %CURRENT_DIR%\tests\resources\unit_cube_v_normal.obj %CURRENT_DIR%\build\MSVC\tests\resources\unit_cube_v_normal.obj /Y
copy %CURRENT_DIR%\tests\resources\unit_cube_v_uv.obj %CURRENT_DIR%\build\MSVC\tests\resources\unit_cube_v_uv.obj /Y
copy %CURRENT_DIR%\tests\resources\unit_cube_v_uv_normal.obj %CURRENT_DIR%\build\MSVC\tests\resources\unit_cube_v_uv_normal.obj /Y

cd %CURRENT_DIR%
