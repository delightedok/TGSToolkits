#/bin/bash

BUILD_PATH=$1
OPT_BUILD="-DCMAKE_INSTALL_PREFIX=. -DCMAKE_BUILD_TYPE=RelWithDebInfo"
CUR_PATH=$(pwd)
echo $CUR_PATH

if [ "#"$BUILD_PATH != "#" ];
then
    cd $BUILD_PATH
fi

cmake $OPT_BUILD $CUR_PATH

make && make install

cd $CUR_PATH

