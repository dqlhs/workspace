#!/bin/bash

# this is release script

function result_check {
    if [ "$?" != "0" ]; then
        exit;
    fi
}

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

print_line() {
    echo ""
}

print_green() {
    print_line
    echo "${green}-> $1 ${reset}"
}

print_red() {
    print_line
    echo "${red}-> $1 ${reset}"
}

if [ -z "$1"  ]; then
    print_red "example : ./release.sh v0.1"
    exit -1;
fi

project_dir=$(dirname $(dirname $(realpath $0)))

echo "project dir : ${project_dir}"
if [ ! -d "${project_dir}/build/rootfs" ]; then
    print_line "!!!build dir is empty"
    exit -1;
fi

pushd ${project_dir} > /dev/null

cd build/rootfs
rm rootfs.tar.gz

print_green "tar rootfs files"
tar -zcvf rootfs$1.tar.gz *

print_green "move tar file to release dir "
mv rootfs$1.tar.gz ${project_dir}/../release/
cp ${project_dir}/script/inmachine/deploy.sh ${project_dir}/../release/

popd > /dev/null
