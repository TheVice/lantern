lantern
=======

###Building for Android

##Prerequisits
* Install\unpack Java Platform (JDK) version 6 or above - http://www.oracle.com/technetwork/java/javase/downloads/index.html
* Unpack Apache Ant - http://ant.apache.org/bindownload.cgi
* Unpack Android SDK - http://developer.android.com/sdk/index.html
* It is also may require to download some addition package via SDK Manager.
* Unpack Android NDK - http://developer.android.com/tools/sdk/ndk/index.html
* Set environment variable ```ANDROID_HOME``` for Android SDK unpacked path.
* Set environment variable ```ANDROID_NDK``` for Android NDK unpacked path. It is highli recomended that path do not consist any space symbols.
* Added bin directory from Apache Ant into PATHs.
* Added directories ```ANDROID_NDK```, ```ANDROID_HOME/platform-tools``` and ```ANDROID_HOME/tools``` into PATHs.
* On Windows it may require to install MinGW ( http://sourceforge.net/projects/mingw or http://sourceforge.net/projects/mingw-w64 ) and set it bin directory into PATHs.

##Build and run tests
* Connect your Android device, start emulator by ```android avd``` or ```emulator -avd <avd_name>``` to run already created emulator directly. It is also possible to run Android x86 ( http://www.android-x86.org/download ) on VirtualBox.
* In a terminal (console) locate into ```platforms/android-9/tests```
* Type and execute ```ndk-build``` to build tests bunaries.
* Make sure ```adb devices``` see your Android device/emulator/OS on virtual.
* Type and execute ```adb push libs/armeabi/runnablelanterntests /data/local/tmp/``` to copy tests binary onto Android.
* Type and execute ```adb shell chmod 775 /data/local/tmp/runnablelanterntests``` to make binary executabled.
* Type and execute ```adb shell /data/local/tmp/runnablelanterntests``` to run tests on Android and see it result on your host OS terminal (console).
* It is also possible to create and run apk, that support Android 2.3 (API Level 9) and above. Just enter ```ant debug``` after you entered ```ndk-build```.
* To install apk on Android type and execute ```adb install -r "bin/Lantern tests-debug.apk"```. Flag -r is make sure that if apk already installed early it will be reinstalled by new.
* Now on your Android navigate to installed application and find ```Lantern tests```.
* Application just run and exit, tests results do not displayed, even on logcat ( http://developer.android.com/tools/help/logcat.html ) where be only info about application run process.
