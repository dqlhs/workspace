#!/bin/bash

if [ -z "$1" ]; then
    echo "example : ./deploy.sh rootfsv0.1.tar.gz"
    exit -1
fi

cp /etc/bluetooth/TL.INI /home/TL.INI 

echo "stop background service ..."
systemctl stop wifi.service
systemctl stop hsaebtserver.service

# remove old audio manager
rm /usr/bin/audiomodecontrold
systemctl stop audiomodecontrold
systemctl disable audiomodecontrold

systemctl stop vr
systemctl disable vr

rm /usr/sbin/hostapd

echo "deploy update package ..."
tar -zxvf $1 -C /

echo "add avahi user ..."
adduser --system --shell /bin/false --home /var/run/avahi --disabled-password avahi

echo "enable avahi"
systemctl daemon-reload; systemctl enable avahi-daemon;

echo "reback bt file ..."
cp /home/TL.INI /etc/bluetooth/TL.INI

echo "wait for sync ..."
sync

echo "wait for reboot ..."
sleep 4

reboot

