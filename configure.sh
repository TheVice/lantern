
if [ ! "$1" ]; then echo "$0 using with <gcc>|<Android>|<clean>"; fi

if [ "$1" = "gcc" ]; then
  command -v cmake >/dev/null 2>&1 || { echo "Path to cmake bin directory not in the PATH enviroment variable" >&2; exit 1; }
  CMAKE_VERSION=$(cmake -version)
  length=`expr match "$CMAKE_VERSION" 'cmake version [0-9.]*'`
  let "length = $length - 14"
  CMAKE_VERSION=cmake-${CMAKE_VERSION:14:$length}
  NUMBER_OF_PROCESSORS=$(nproc)
  CURRENT_DIR=$PWD
  GCC_VERSION=$(gcc -dumpversion)

  echo gcc_$GCC_VERSION

  if [ ! -d $CURRENT_DIR/build ]; then mkdir $CURRENT_DIR/build; fi
  if [ ! -d $CURRENT_DIR/build/$CMAKE_VERSION ]; then mkdir $CURRENT_DIR/build/$CMAKE_VERSION; fi
  if [ ! -d $CURRENT_DIR/build/$CMAKE_VERSION/linux ]; then mkdir $CURRENT_DIR/build/$CMAKE_VERSION/linux; fi
  if [ ! -d $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION ]; then mkdir $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION; fi
  if [ ! -d $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION/Debug ]; then mkdir $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION/Debug; fi
  if [ ! -d $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION/Release ]; then mkdir $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION/Release; fi
  if [ ! -d $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION/RelWithDebInfo ]; then mkdir $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION/RelWithDebInfo; fi

  cd $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION/Debug
  echo "********************************************************************************"
  cmake $CURRENT_DIR/. -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$NUMBER_OF_PROCESSORS -s"
  echo "********************************************************************************"

  cd $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION/Release
  echo "********************************************************************************"
  cmake $CURRENT_DIR/. -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE="Release" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$NUMBER_OF_PROCESSORS -s"
  echo "********************************************************************************"

  cd $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION/RelWithDebInfo
  echo "********************************************************************************"
  cmake $CURRENT_DIR/. -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE="RelWithDebInfo" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$NUMBER_OF_PROCESSORS -s"
  echo "********************************************************************************"
  cd $CURRENT_DIR
fi

if [ "$1" = "Android" ]; then
  command -v cmake >/dev/null 2>&1 || { echo "Path to cmake bin directory not in the PATH enviroment variable" >&2; exit 1; }
  CMAKE_VERSION=$(cmake -version)
  length=`expr match "$CMAKE_VERSION" 'cmake version [0-9.]*'`
  let "length = $length - 14"
  CMAKE_VERSION=cmake-${CMAKE_VERSION:14:$length}
  NUMBER_OF_PROCESSORS=$(nproc)
  CURRENT_DIR=$PWD
  if [ -z ${ANDROID_TOOLCHAIN+x} ]; then echo "Path to android.toolchain.cmake not set" ; exit 1; fi
  ANDROID_NATIVE_API_LEVEL=12
  ANDROID_ABI=armeabi

  if [ ! -d $CURRENT_DIR/build ]; then mkdir $CURRENT_DIR/build; fi
  if [ ! -d $CURRENT_DIR/build/$CMAKE_VERSION ]; then mkdir $CURRENT_DIR/build/$CMAKE_VERSION; fi
  if [ ! -d $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL ]; then mkdir $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL; fi
  if [ ! -d $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL/$ANDROID_ABI ]; then mkdir $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL/$ANDROID_ABI; fi
  if [ ! -d $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL/$ANDROID_ABI/Debug ]; then mkdir $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL/$ANDROID_ABI/Debug; fi
  if [ ! -d $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL/$ANDROID_ABI/Release ]; then mkdir $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL/$ANDROID_ABI/Release; fi
  if [ ! -d $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL/$ANDROID_ABI/RelWithDebInfo ]; then mkdir $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL/$ANDROID_ABI/RelWithDebInfo; fi

  cd $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL/$ANDROID_ABI/Debug
  echo "********************************************************************************"
  cmake $CURRENT_DIR -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$NUMBER_OF_PROCESSORS -s" -DCMAKE_TOOLCHAIN_FILE="$ANDROID_TOOLCHAIN" -DLIBRARY_OUTPUT_PATH_ROOT="." -DANDROID_NATIVE_API_LEVEL="$ANDROID_NATIVE_API_LEVEL" -DANDROID_ABI="$ANDROID_ABI"
  echo "********************************************************************************"

  cd $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL/$ANDROID_ABI/Release
  echo "********************************************************************************"
  cmake $CURRENT_DIR/. -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE="Release" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$NUMBER_OF_PROCESSORS -s" -DCMAKE_TOOLCHAIN_FILE="$ANDROID_TOOLCHAIN" -DLIBRARY_OUTPUT_PATH_ROOT="." -DANDROID_NATIVE_API_LEVEL="$ANDROID_NATIVE_API_LEVEL" -DANDROID_ABI="$ANDROID_ABI"
  echo "********************************************************************************"

  cd $CURRENT_DIR/build/$CMAKE_VERSION/Android-$ANDROID_NATIVE_API_LEVEL/$ANDROID_ABI/RelWithDebInfo
  echo "********************************************************************************"
  cmake $CURRENT_DIR/. -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE="RelWithDebInfo" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$NUMBER_OF_PROCESSORS -s" -DCMAKE_TOOLCHAIN_FILE="$ANDROID_TOOLCHAIN" -DLIBRARY_OUTPUT_PATH_ROOT="." -DANDROID_NATIVE_API_LEVEL="$ANDROID_NATIVE_API_LEVEL" -DANDROID_ABI="$ANDROID_ABI"
  echo "********************************************************************************"
  cd $CURRENT_DIR
fi

if [ "$1" = "clean" ]; then
  echo "********************************************************************************"
  CURRENT_DIR=$PWD
  find $CURRENT_DIR/build -type f -name *.a -exec rm -rf {} \;
  find $CURRENT_DIR/build -type f -executable -exec rm -rf {} \;
  find $CURRENT_DIR/build -type f -name CMakeCache.txt -exec rm -rf {} \;
  echo "********************************************************************************"
fi
