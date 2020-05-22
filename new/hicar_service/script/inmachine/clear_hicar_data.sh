#!/bin/bash

work_dir=$(pwd)
rm ${work_dir}/info*.data
rm ${work_dir}/hks_keystore
rm ${work_dir}/deviceInfo.txt

cp /etc/autocon_flg.txt /usr/bin/bluetooth/
