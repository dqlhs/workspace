#!/bin/bash

java -jar /home/atropos/Application/smc_6_6_3/bin/Smc.jar -c++ *.sm
java -jar /home/atropos/Application/smc_6_6_3/bin/Smc.jar -graph -glevel 2  *.sm

dot -Tpdf *.dot -o smc.pdf

sed -i '15i#include "hicar_usb_def.h"' usb_status_smc_sm.h

mv *.h *.cpp ../
