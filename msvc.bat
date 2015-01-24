@echo off

set CURRENT_DIR=%CD%
if not exist %CURRENT_DIR%\build mkdir %CURRENT_DIR%\build
if not exist %CURRENT_DIR%\build\MSVC mkdir %CURRENT_DIR%\build\MSVC
if not exist %CURRENT_DIR%\build\MSVC\obj mkdir %CURRENT_DIR%\build\MSVC\obj
cd %CURRENT_DIR%\build\MSVC\obj

::lantern
if not exist %CURRENT_DIR%\build\MSVC\lib mkdir %CURRENT_DIR%\build\MSVC\lib
cl /c /EHsc /W4 /GS /D NOMINMAX /D WIN32 /D _WIN32 /D WINDOWS /D _LIB %CURRENT_DIR%\lantern\src\app.cpp %CURRENT_DIR%\lantern\src\camera.cpp %CURRENT_DIR%\lantern\src\color.cpp %CURRENT_DIR%\lantern\src\matrix3x3.cpp %CURRENT_DIR%\lantern\src\matrix4x4.cpp %CURRENT_DIR%\lantern\src\mesh.cpp %CURRENT_DIR%\lantern\src\obj_import.cpp %CURRENT_DIR%\lantern\src\pipeline.cpp %CURRENT_DIR%\lantern\src\texture.cpp %CURRENT_DIR%\lantern\src\vector3.cpp %CURRENT_DIR%\lantern\src\vector4.cpp /I"%CURRENT_DIR%\lantern\include"
lib app.obj camera.obj color.obj matrix3x3.obj matrix4x4.obj mesh.obj obj_import.obj pipeline.obj texture.obj vector3.obj vector4.obj /out:"%CURRENT_DIR%\build\MSVC\lib\lantern.lib"

if not exist %CURRENT_DIR%\build\MSVC\examples mkdir %CURRENT_DIR%\build\MSVC\examples

::empty_app
if not exist %CURRENT_DIR%\build\MSVC\examples\empty_app mkdir %CURRENT_DIR%\build\MSVC\examples\empty_app
cl /EHsc /W4 /GS /D NOMINMAX /D WIN32 /D _WIN32 /D WINDOWS %CURRENT_DIR%\examples\empty_app\main.cpp kernel32.lib user32.lib gdi32.lib glu32.lib opengl32.lib lantern.lib /I"%CURRENT_DIR%\lantern\include" /link /SafeSEH /DynamicBase /NXCompat /SUBSYSTEM:CONSOLE /LIBPATH:"%CURRENT_DIR%\build\MSVC\lib" /out:"%CURRENT_DIR%\build\MSVC\examples\empty_app\empty_app.exe"

::rasterized_triangle_app
if not exist %CURRENT_DIR%\build\MSVC\examples\rasterized_triangle_app mkdir %CURRENT_DIR%\build\MSVC\examples\rasterized_triangle_app
cl /EHsc /W4 /GS /D NOMINMAX /D WIN32 /D _WIN32 /D WINDOWS /D_USE_MATH_DEFINES %CURRENT_DIR%\examples\rasterized_triangle_app\main.cpp kernel32.lib user32.lib gdi32.lib glu32.lib opengl32.lib lantern.lib /I"%CURRENT_DIR%\lantern\include" /link /SafeSEH /DynamicBase /NXCompat /SUBSYSTEM:CONSOLE /LIBPATH:"%CURRENT_DIR%\build\MSVC\lib" /out:"%CURRENT_DIR%\build\MSVC\examples\rasterized_triangle_app\rasterized_triangle_app.exe"
if not exist %CURRENT_DIR%\build\MSVC\examples\rasterized_triangle_app\resources mkdir %CURRENT_DIR%\build\MSVC\examples\rasterized_triangle_app\resources
copy %CURRENT_DIR%\examples\rasterized_triangle_app\resources\triangle.obj %CURRENT_DIR%\build\MSVC\examples\rasterized_triangle_app\resources\triangle.obj /Y

::tests
if not exist %CURRENT_DIR%\build\MSVC\tests mkdir %CURRENT_DIR%\build\MSVC\tests
cl /EHsc /W4 /GS /D NOMINMAX /D WIN32 /D _WIN32 /D WINDOWS /D_USE_MATH_DEFINES %CURRENT_DIR%\gtest-1.7.0\src\gtest-all.cc %CURRENT_DIR%\tests\src\camera.cpp %CURRENT_DIR%\tests\src\main.cpp %CURRENT_DIR%\tests\src\matrix3x3.cpp %CURRENT_DIR%\tests\src\matrix4x4.cpp %CURRENT_DIR%\tests\src\obj_import.cpp %CURRENT_DIR%\tests\src\pipeline.cpp %CURRENT_DIR%\tests\src\vector3.cpp %CURRENT_DIR%\tests\src\vector4.cpp kernel32.lib lantern.lib /I"%CURRENT_DIR%\tests\include" /I"%CURRENT_DIR%\gtest-1.7.0" /I"%CURRENT_DIR%\gtest-1.7.0\include" /I"%CURRENT_DIR%\lantern\include" /link /SafeSEH /DynamicBase /NXCompat /SUBSYSTEM:CONSOLE /LIBPATH:"%CURRENT_DIR%\build\MSVC\lib" /out:"%CURRENT_DIR%\build\MSVC\tests\tests.exe"
if not exist %CURRENT_DIR%\build\MSVC\tests\resources mkdir %CURRENT_DIR%\build\MSVC\tests\resources
copy %CURRENT_DIR%\tests\resources\unit_cube_pos.obj %CURRENT_DIR%\build\MSVC\tests\resources\unit_cube_pos.obj /Y
copy %CURRENT_DIR%\tests\resources\unit_cube_pos_normal.obj %CURRENT_DIR%\build\MSVC\tests\resources\unit_cube_pos_normal.obj /Y
copy %CURRENT_DIR%\tests\resources\unit_cube_pos_texcoord.obj %CURRENT_DIR%\build\MSVC\tests\resources\unit_cube_pos_texcoord.obj /Y
copy %CURRENT_DIR%\tests\resources\unit_cube_pos_texcoord_normal.obj %CURRENT_DIR%\build\MSVC\tests\resources\unit_cube_pos_texcoord_normal.obj /Y

cd %CURRENT_DIR%
