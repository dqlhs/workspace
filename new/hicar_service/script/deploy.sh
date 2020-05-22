#!/bin/bash
# this is deploy script

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
	print_red "example : ./deploy.sh 192.168.5.12"
	exit -1;
fi

function valid_ip()
{
    local  ip=$1
    local  stat=1

    if [[ $ip =~ ^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$ ]]; then
        OIFS=$IFS
        IFS='.'
        ip=($ip)
        IFS=$OIFS
        [[ ${ip[0]} -le 255 && ${ip[1]} -le 255 \
            && ${ip[2]} -le 255 && ${ip[3]} -le 255 ]]
        stat=$?
    fi
    return $stat
}

if ! valid_ip $1
then
        print_red "!!!ip addr is invalid"
        exit -1;
fi

project_dir=$(dirname $(dirname $(realpath $0)))

echo "project dir : ${project_dir}"
if [ ! -d "${project_dir}/build/rootfs" ]; then
	print_red "!!!build dir is empty"
	exit -1;
fi

pushd ${project_dir} > /dev/null

cd build/rootfs
rm rootfs.tar.gz

print_green "tar rootfs files"
tar -zcvf rootfs.tar.gz *

print_green "upload file to remote and deploy"
scp rootfs.tar.gz root@$1:/home/root && ssh root@$1 "mv /etc/bluetooth/TL.INI /home/; tar -zxvf /home/root/rootfs.tar.gz -C /; rm /home/root/rootfs.tar.gz; mv /home/TL.INI /etc/bluetooth/TL.INI; sync; sync; sync"

popd > /dev/null


