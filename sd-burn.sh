#!/bin/bash
reader_type1="/dev/sdb"
reader_type2="/dev/mmcblk0"

if [ -z $1 ]
then
    echo "usage: ./sd_fusing.sh <SD Reader's device file>"
    exit 0
fi

if [ $1 = $reader_type1 ]
then

echo "burning to SD card.............."
sudo dd iflag=dsync oflag=dsync if=webee210-uboot.bin of=/dev/mmcblk0 seek=1
echo "sync........."
sync
echo  "DONE-( ͡° ͜ʖ ͡° )✧-YAP!"

elif [ $1 = $reader_type2 ]
then
echo "burning to SD card.............."
sudo dd iflag=dsync oflag=dsync if=webee210-uboot.bin of=/dev/mmcblk0 seek=1
echo "sync........."
sync
echo  "DONE-( ͡° ͜ʖ ͡° )✧-YAP!"

else
	echo "unsupport SD reader"
	exit 0
fi


