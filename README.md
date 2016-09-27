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

* Install\unpack [Java Platform (JDK)](http://www.oracle.com/technetwork/java/javase/downloads/index.html) version 8
* Unpack [Android SDK](https://developer.android.com/studio/index.html#downloads). It is also may require to download build tool release 24.0.2 via SDK Manager if that release not located in downloaded Android SDK
* Unpack [Android NDK](https://developer.android.com/ndk/downloads/index.html) not above than ```Android NDK, Revision 10e (May 2015)``` (see addition download [links](https://github.com/taka-no-me/android-cmake/blob/master/ndk_links.md))
* Create ```local.properties``` and put to it paths to sdk and ndk folders.
* On Windows host it can be next:
* ```sdk.dir=C\:\\android-sdk-windows```
* ```ndk.dir=C\:\\android-ndk-r10e-windows-x86_64```
* On Linux host it can be next:
* ```sdk.dir=/home/user/android-sdk-linux```
* ```ndk.dir=/home/user/android-ndk-r10e-linux-x86_64```

###Building for Android

Lantern uses [Gradle](http://gradle.org/) as its build system.

* To build release apk files run next commands. First command need only to generate key file that can be keep for future using, so do not need to run it again if file located at original location
* ```gradlew --daemon generateKey --q```
* ```gradlew --daemon :empty_app:assembleRelease```
* ```gradlew --daemon :rasterized_triangle_app:assembleRelease```
* To build all apk command ```gradlew --daemon build``` can be used
