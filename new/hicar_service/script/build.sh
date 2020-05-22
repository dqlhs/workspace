#!/bin/bash

function result_check {
	if [ "$?" != "0" ]; then 
		exit -1;
	fi
}

project_dir=$(dirname $(dirname $(realpath $0)))

if [ "$ARCH" != "arm" ]; then
	echo "!!!please source env!"
	exit -1;
fi

echo "project dir : ${project_dir}"
if [ ! -d "${project_dir}/build" ]; then
	mkdir -p build/ 
fi

git pull 
result_check

if [ ! -d "${project_dir}/build" ]; then
	echo "dir : ${project_dir}/build is not exit !!!"
	exit -1;
fi

pushd ${project_dir}
# into build dir and clean build dir
mkdir -p build/ 2>/dev/null
cd build/

if [ "$1" != "increment" ]
then
	rm -rf ../build/*
fi

cmake -DSYSROOT_DIR=${OECORE_TARGET_SYSROOT} -DCMAKE_BUILD_TYPE=Debug  ..
result_check 

make install DESTDIR=rootfs -j "$(nproc)"
result_check 
popd
