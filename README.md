lantern
=======

###Building for Android

##Prerequisits
* Install\unpack [Java Platform (JDK)](http://www.oracle.com/technetwork/java/javase/downloads/index.html) version 6 or above.
* Unpack [Apache Ant](http://ant.apache.org/bindownload.cgi).
* Unpack [Android SDK](http://developer.android.com/sdk/index.html).
* It is also may require to download some addition package via SDK Manager.
* Unpack [Android NDK](http://developer.android.com/tools/sdk/ndk/index.html).
* Add bin directory from JDK (not from JRE) into PATHs.
* Set environment variable ```ANDROID_HOME``` for Android SDK unpacked path.
* Set environment variable ```ANDROID_NDK``` for Android NDK unpacked path. It is highli recomended that path do not consist any space symbols.
* Add bin directory from Apache Ant into PATHs.
* Add directories ```ANDROID_NDK```, ```ANDROID_HOME/platform-tools``` and ```ANDROID_HOME/tools``` into PATHs.
* On Windows it may require to install [MinGW](http://sourceforge.net/projects/mingw) or [MinGW-w64](http://sourceforge.net/projects/mingw-w64) and set it bin directory into PATHs.

##Build and run tests
* Connect your Android device, start emulator by ```android avd``` or ```emulator -avd <avd_name>``` to run already created emulator directly.
* ~~It is also possible to run [Android x86](http://www.android-x86.org/download) on VirtualBox.~~ To be detailed.
* In a terminal (console) locate into ```platforms/android-9/tests```.
* Type and execute ```ndk-build``` to build tests bunaries.
* Make sure ```adb devices``` see your Android device/emulator/OS on virtual.
* Type and execute ```adb push libs/armeabi/libgnustl_shared.so /data/local/tmp/``` to copy required by tests library.
* Type and execute ```adb shell mkdir /data/local/tmp/resources``` for make folder for resorce file.
* Type and execute ```adb push ../../../tests/resources/unit_cube.obj /data/local/tmp/resources```
* Type and execute ```adb push libs/armeabi/runnablelanterntests /data/local/tmp/``` to copy tests binary onto Android.
* Type and execute ```adb shell chmod 775 /data/local/tmp/runnablelanterntests``` to make binary executabled.
* Type and execute ```adb shell "cd /data/local/tmp ; LD_LIBRARY_PATH=/data/local/tmp /data/local/tmp/runnablelanterntests"``` to run tests on Android and see it result on your host OS terminal (console).

##Build and run tests via apk package
* It is also possible to create and run apk, that support on [Android 2.3 (API Level 9)](https://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels) and above.
* In a terminal (console) locate into ```platforms/android-9/tests```.
* As in previous variant type and execute ```ndk-build``` to build tests bunaries.
* Do not forgot to include resource to your apk package:
* ```mkdir assets```
* ```mkdir assets/resources```
* ```copy ../../../tests/resources/unit_cube.obj assets/resources/unit_cube.obj```
* And now enter ```ant debug``` to create apk package.
* To install apk on Android type and execute ```adb install -r "bin/Lantern tests-debug.apk"```. Flag -r is make sure that if apk already installed early it will be reinstalled by new.
* Now on your Android navigate to installed application and find ```Lantern tests```.
* Application just run and exit, tests results do not displayed, even on [logcat](http://developer.android.com/tools/help/logcat.html) where be only info about application run process.
* To uninstall apk execute ```adb uninstall com.android.lantern.tests```.

##Build rotating_car_app
* Apk will support on [Android 2.3 (API Level 9)](https://developer.android.com/guide/topics/manifest/uses-sdk-element.html#ApiLevels) and above.
* In a terminal (console) locate into ```platforms/android-9/rotating_car_app```.
* First of all, do not forgot to include resource to your apk package:
* ```mkdir assets```
* ```mkdir assets/resources```
* ```copy ../../../examples/rotating_car_app/resources/car.obj assets/resources/car.obj```
* Execute ```ndk-build``` to build bunaries.
* Enter ```ant debug``` to create apk package.
* And ```adb install -r "bin/rotating_car_app-debug.apk"```. Flag -r is make sure that if apk already installed early it will be reinstalled by new.
* Now on your Android navigate to installed application and find ```rotating_car_app```.
* To uninstall apk execute ```adb uninstall com.android.lantern.rotating_car_app```.