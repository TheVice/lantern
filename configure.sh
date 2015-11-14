
if [ ! "$1" ]; then
  echo "$0 using with <clean>|<gcc>"
  echo "clean for deleting binaries files (*.a, executable) from bin directory"
  echo "gcc for Eclipse project with gcc"
fi

if [ "$1" = "gcc" ]; then
  command -v cmake >/dev/null 2>&1 || { echo "Path to cmake bin directory not in the PATH enviroment variable" >&2; exit 1; }
  CMAKE_VERSION=$(cmake -version)
  length=`expr match "$CMAKE_VERSION" 'cmake version [0-9.]*'`
  let "length = $length - 14"
  CMAKE_VERSION=cmake-${CMAKE_VERSION:14:$length}
  CMAKE_GENERATOR="Eclipse CDT4 - Unix Makefiles"
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
  cmake $(dirname $0) -G"$CMAKE_GENERATOR" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$NUMBER_OF_PROCESSORS -s"
  echo "********************************************************************************"

  cd $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION/Release
  echo "********************************************************************************"
  cmake $(dirname $0) -G"$CMAKE_GENERATOR" -DCMAKE_BUILD_TYPE="Release" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$NUMBER_OF_PROCESSORS -s"
  echo "********************************************************************************"

  cd $CURRENT_DIR/build/$CMAKE_VERSION/linux/gcc$GCC_VERSION/RelWithDebInfo
  echo "********************************************************************************"
  cmake $(dirname $0) -G"$CMAKE_GENERATOR" -DCMAKE_BUILD_TYPE="RelWithDebInfo" -DCMAKE_ECLIPSE_MAKE_ARGUMENTS="-j$NUMBER_OF_PROCESSORS -s"
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
