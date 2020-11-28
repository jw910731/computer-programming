#!/bin/bash

id=40947030S

if [ -z ${1+x} ]; then
	echo "homework to zip is unset."
	exit
fi
ln -s hw0$1 HW$1_$id
zip -r HW$1_$id.zip HW$1_$id -x "*.DS_Store"
rm HW$1_$id