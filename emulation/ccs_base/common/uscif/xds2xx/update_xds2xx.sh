#!/bin/bash

xds200()
{
    if [ -f xds200_firmware_v1008.bin ];then
        echo .
        echo "Updating Firmware ..."
        ./xds2xx_conf update  xds2xxu 0 xds200_firmware_v1008.bin
        echo .
        echo "Rebooting, please wait ..."
        ./xds2xx_conf boot xds2xxu 0
    fi 

    if [ -f xds200_cpld_v1008.xsvf ];then
        echo .
        echo "Updating CPLD ..."
        ./xds2xx_conf program xds2xxu 0 xds200_cpld_v1008.xsvf
    fi
    
    if [ -f xds200_firmware_v1008.bin ];then
        echo .
        echo "Reading Configuration ..."
        echo .
        echo "Check swRev is 1.0.0.8 or higher"
        echo .
        ./xds2xx_conf get xds2xxu 0  
    fi 
}

if [ "$1" == "xds200" ];then
    xds200  
elif [ "$1" == "xds220" ];then
    echo "ERROR: Do not run this script on an XDS220."
    echo "Use the tools and firmware suppleid by the manufacturer."
else    
    echo "ERROR: Unknown option $1".
    echo "The only valid option is xds200."
    echo "Do not run this script on an XDS220."
fi

echo .
echo "Hit <Enter> to exit."
read

