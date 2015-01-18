lantern
=======

###Building for Android

##Build requires
* Install\unpack [Java Platform (JDK)](http://www.oracle.com/technetwork/java/javase/downloads/index.html) version 6 or above.
* Unpack [Apache Ant](http://ant.apache.org/bindownload.cgi).
* Unpack [Android SDK](http://developer.android.com/sdk/index.html).
* It is also may require to download some addition package via SDK Manager.
* Unpack [Android NDK](http://developer.android.com/tools/sdk/ndk/index.html).
* Add bin directory from JDK (not from JRE) into PATHs.
* Set environment variable ```ANDROID_HOME``` for Android SDK unpacked path.
* Set environment variable ```ANDROID_NDK``` for Android NDK unpacked path. It is highly recommended that path do not consist any space symbols.
* Add bin directory from Apache Ant into PATHs.
* Add directories ```ANDROID_NDK```, ```ANDROID_HOME/platform-tools``` and ```ANDROID_HOME/tools``` into PATHs.
* ~~On Windows it may require to install [MinGW](http://sourceforge.net/projects/mingw) or [MinGW-w64](http://sourceforge.net/projects/mingw-w64) and set it bin directory into PATHs.~~ Only when used old NDK.
* Get [android.toolchain.cmake](https://github.com/taka-no-me/android-cmake/blob/master/android.toolchain.cmake) (mirror [1](https://code.google.com/p/android-cmake/source/browse/toolchain/android.toolchain.cmake), [2](http://code.opencv.org/projects/opencv/repository/revisions/master/changes/platforms/android/android.toolchain.cmake) and save to any place you want.

##Prepare Eclipse project
* At root of project make ```build``` folder and open it in terminal (console)
* On Windows execute ```cmake .. -G "Eclipse CDT4 - MinGW Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j%NUMBER_OF_PROCESSORS% -s" -DCMAKE_TOOLCHAIN_FILE="<path to>android.toolchain.cmake" -DLIBRARY_OUTPUT_PATH_ROOT="." -DANDROID_NATIVE_API_LEVEL="9" -DANDROID_ABI="armeabi"```
* On Linux execute ```cmake .. -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$(nproc) -s" -DCMAKE_TOOLCHAIN_FILE="<path to>android.toolchain.cmake" -DLIBRARY_OUTPUT_PATH_ROOT="." -DANDROID_NATIVE_API_LEVEL="9" -DANDROID_ABI="armeabi"```
* ```CMAKE_BUILD_TYPE``` also can be set to ```Release``` or ```RelWithDebInfo```. Tested in ```Debug``` mode.
* ```ANDROID_NATIVE_API_LEVEL``` can be great than ```9```, but not lower. Tested only with ```9```.
* ```ANDROID_ABI``` dependence on NDK version can be set to: ```armeabi-v7a```, ```x86```, ```mips``` and also 64 version of various CPU architecture. With set ```armeabi``` almost all ARM devices emulator can to execute. Check your emulator setting, device configuration to determinate what type of CPU architecture you have.
* Connect your Android device, start emulator by ```android avd``` or ```emulator -avd <avd_name>``` to run already created emulator directly.
* ~~It is also possible to run [Android x86](http://www.android-x86.org/download) on hypervisor.~~ To be detailed.
* Make sure ```adb devices``` see your Android device/emulator~~/OS on virtual~~.
* Execute ```mingw32-make -j%NUMBER_OF_PROCESSORS% -s rotating_car_app[run-apk]``` on Windows or ```make -j$(nproc) -s rotating_car_app[run-apk]``` on Linux to run rotating_car_app on Android.
* Execute ```mingw32-make -j%NUMBER_OF_PROCESSORS% -s run_tests``` on Windows or ```make -j$(nproc) -s run_tests``` on Linux to run tests on Android.
* You can also do the same from Eclipse IDE after import already exist project from ```build``` directory.

~~##Build and run tests
* In a terminal (console) locate into ```platforms/android-9/tests```.
* Type and execute ```ndk-build``` to build tests binaries.
* Type and execute ```adb push libs/armeabi/libgnustl_shared.so /data/local/tmp/``` to copy required by tests library.
* Type and execute ```adb shell mkdir /data/local/tmp/resources``` for make folder for resorce file.
* Type and execute ```adb push ../../../tests/resources/unit_cube.obj /data/local/tmp/resources```
* Type and execute ```adb push libs/armeabi/runnablelanterntests /data/local/tmp/``` to copy tests binary onto Android.
* Type and execute ```adb shell chmod 775 /data/local/tmp/runnablelanterntests``` to make binary executabled.
* Type and execute ```adb shell "cd /data/local/tmp ; LD_LIBRARY_PATH=/data/local/tmp /data/local/tmp/runnablelanterntests"``` to run tests on Android and see it result on your host OS terminal (console).

~~##Build and run tests via apk package
* It is also possible to create and run apk, that support on [Android 2.3 (API Level 9)](https://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels) and above.
* In a terminal (console) locate into ```platforms/android-9/tests```.
* As in previous variant type and execute ```ndk-build``` to build tests binaries.
* Do not forgot to include resource to your apk package:
* ```mkdir assets```
* ```mkdir assets/resources```
* ```copy ../../../tests/resources/unit_cube.obj assets/resources/unit_cube.obj```
* And now enter ```ant debug``` to create apk package.
* To install apk on Android type and execute ```adb install -r "bin/Lantern tests-debug.apk"```. Flag -r is make sure that if apk already installed early it will be reinstalled by new.
* Now on your Android navigate to installed application and find ```Lantern tests```.
* Application just run and exit, tests results do not displayed, even on [logcat](http://developer.android.com/tools/help/logcat.html) where be only info about application run process.
* To uninstall apk execute ```adb uninstall com.android.lantern.tests```.

~~##Build and run apk of empty_app
* Apk will support on [Android 2.3 (API Level 9)](https://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels) and above.
* In a terminal (console) locate into ```platforms/android-9/empty_app```.
* Execute ```ndk-build``` to build binaries.
* Enter ```ant debug``` to create apk package.
* And ```adb install -r "bin/empty_app-debug.apk"```. Flag -r is make sure that if apk already installed early it will be reinstalled by new.
* Now on your Android navigate to installed application and find ```empty_app```.
* You can also execute ```adb shell am start -a android.intent.action.MAIN -n com.android.lantern.empty_app/com.android.lantern.empty_app.LaternEmptyAppNativeActivity``` to run app.
* To uninstall apk execute ```adb uninstall com.android.lantern.empty_app```.

~~##Build and run apk of rotating_car_app
* Apk will support on [Android 2.3 (API Level 9)](https://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels) and above.
* In a terminal (console) locate into ```platforms/android-9/rotating_car_app```.
* First of all, do not forgot to include resource to your apk package:
* ```mkdir assets```
* ```mkdir assets/resources```
* ```copy ../../../examples/rotating_car_app/resources/car.obj assets/resources/car.obj```
* Execute ```ndk-build``` to build binaries.
* Enter ```ant debug``` to create apk package.
* And ```adb install -r "bin/rotating_car_app-debug.apk"```. Flag -r is make sure that if apk already installed early it will be reinstalled by new.
* Now on your Android navigate to installed application and find ```rotating_car_app```.
* You can also execute ```adb shell am start -a android.intent.action.MAIN -n com.android.lantern.rotating_car_app/com.android.lantern.rotating_car_app.LaternRotatingCarAppNativeActivity``` to run app.
* To uninstall apk execute ```adb uninstall com.android.lantern.rotating_car_app```.